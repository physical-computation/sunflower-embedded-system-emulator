/*									*/
/*	Adapted from							*/
/*		"fmref.c: C reference implementation of FM Radio	*/
/*		David Maze <dmaze@cag.lcs.mit.edu>			*/
/*		$Id: fmref.c,v 1.8 2002/07/29 14:05:48 aalamb Exp $"	*/
/*	by								*/
/*		pstanley@ece.cmu.edu					*/
/*									*/
#include <math.h>
#include "e-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include "devscc.h"
#include "../../../port/devnet.h"
#include "../../../port/devrand.h"
#include "../../../port/devrtc.h"
#include "../../../port/devloc.h"
#include "../../../port/devlog.h"
#include "../../../port/devexcp.h"
#include "../../../port/superHspecific.h"
#include "print.h"
#include "../../port/misc.h"
#include "fault.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "network-hitachi-sh.h"
#include "batt.h"
#include "swradio.h"


/*	    	8 GPRs + PR		*/
uchar			REGSAVESTACK[36];
volatile int		done_flag = 0;
volatile int		sample_flag = 0;
volatile int		syn_sent = 0;
volatile int		my_id = -1;
volatile FloatBuffer	fb3, fb4;

static void	hdlr_install(void);
extern int	pow10(int y);
extern long	strlen(uchar *s);
extern long	strcmp(uchar *s1, uchar *s2);
extern int 	strncmp(uchar *s1, uchar *s2, ulong n);
extern uchar*	strncpy(uchar *s1, uchar *s2, ulong n);
extern void* 	memmove(void *, void *, long);


float eq_cutoffs[EQUALIZER_BANDS + 1] =
	{
		55.00,
		77.78,
		110.00,
		155.56,
		220.00,
		311.12,
    		440.00,
		622.25,
		880.00,
		1244.50,
		1760.00
	};

int
main(void)
{
        /*      Monitor code is @ 0x8001000     */
        void    (*fptr)() = (void *)0x8001000;

	int		addrchars, i;
	uchar		sink_dst[16+1];
	EqualizerData	eq_data;



	/*	If we are the clone of someone, my_id is already set, keep it, for timeslot stuff	*/
	if (my_id == -1)
	{
		addrchars = strlen((char *)NIC_OUI);

		my_id = 0;

		for (i = addrchars-1; i >= 0; i--)
		{
			my_id +=  (*(NIC_OUI+i) - '0') * pow10(addrchars - 1 - i);
		}
	}
	sprint(&sink_dst[0], "%d", 11);

	hdlr_install();

	/*	Power up both RX interfaces	*/
	devnet_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, 0);
	devnet_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, 1);

	print("swradio-eq [%d] installing vector code...\n", my_id);



	/*	Make sure we're talking on regular segment		*/
	/*	uncommenmt if doing segmented network	*/
	sample_flag = 0;
	syn_sent = 0;


	init_equalizer(&eq_data);


	/*	Wait till intr_hdlr is done	*/
	done_flag = 0;
	while (!done_flag)
	{
		if (sample_flag)
		{
			run_equalizer((void *)&fb3, (void *)&fb4, &eq_data);


			/*
			print("swradio-eq (node [%d]) sending [%d] bytes to SINK (node [%s])\n",
				my_id, sizeof(fb4), sink_dst);
			print("fb4.rpos = [%d], fb4.rlen = [%d]\n", fb4.rpos, fb4.rlen);
			*/


			/*	Now send fb4 to be sink'd. We dont re-xmit	*/
			devnet_xmit((uchar *)&sink_dst, 0, (uchar *)&fb4, sizeof(fb4), 1);

			/*									*/
			/*	Since we are doing Call-By-Value, we must make this change 	*/
			/*	here. The corresponding calculation in write_floats is a No-op	*/
			/*									*/
			fb4.rpos++;

			sample_flag = 0;
		}

		sleep();
	}

	/*	Forcefully go to monitor	*/
	fptr();


	return 0;		
}

void
run_lpf(FloatBuffer *fbin, FloatBuffer *fbout, LPFData *data)
{
	float sum = 0.0;
	int i = 0;


	if (fbin->rpos + data->taps - 1 >= fbin->rlen)
	{
		print("WARNING: upcoming underflow in run_lpf()\n");
	}

	for (i = 0; i < data->taps; i++)
	{
		sum += fbin->buff[fbin->rpos + i] * data->coeff[i];
	}
	fbin->rpos += data->decimation + 1;
  
	/*								*/
	/*	Check that there's room in the output buffer; 		*/
	/*	move data if necessary.					*/
	/*								*/
	fb_ensure_writable(fbout, 1);
	fbout->buff[fbout->rlen++] = sum;

	return;
}


void
init_lpf_data(LPFData *data, float freq, int taps, int decimation)
{
	/*								*/
	/*	Assume that CUTOFF_FREQUENCY is non-zero. See		*/
	/*	comments in StreamIt LowPassFilter.java for origin.	*/
	/*								*/
	float w = 2 * M_PI * freq / SAMPLING_RATE;
	int i;
	float m = taps - 1.0;

	data->freq = freq;
	data->taps = taps;
	data->decimation = decimation;

	for (i = 0; i < taps; i++)
	{
		if (i - m/2 == 0.0)
		{
			data->coeff[i] = w / M_PI;
		}
    		else
      		{
			data->coeff[i] = sin(w * (i - m/2)) / M_PI / (i - m/2) *
        		(0.54 - 0.46 * cos(2 * M_PI * i / m));
		}
	}
}


void
init_equalizer(EqualizerData *data)
{
	int i;
  
	/*								*/
	/*	Equalizer structure: there are ten band-pass filters,	*/
	/*	with cutoffs as shown below.  The outputs of these 	*/
	/*	filters get added together.  Each band-pass filter 	*/
	/*	is LPF(high)-LPF(low).					*/
	/*								*/
	for (i = 0; i < EQUALIZER_BANDS + 1; i++)
	{
		init_lpf_data(&data->lpf[i], eq_cutoffs[i], 64, 0);
	}

	/*		Also initialize member buffers.			*/
	for (i = 0; i < EQUALIZER_BANDS + 1; i++)
	{
		data->fb[i].rpos = data->fb[i].rlen = 0;
	}

	for (i = 0; i < EQUALIZER_BANDS; i++)
	{
		data->gain[i] = 1.0;
	}

	return;
}


void
run_equalizer(FloatBuffer *fbin, FloatBuffer *fbout, EqualizerData *data)
{
	int i, rpos;
	float lpf_out[EQUALIZER_BANDS + 1];
	float sum = 0.0;

	/*								*/
	/*	Save the input read location; we can reuse the same 	*/
	/*	input data on all of the LPFs.				*/
	/*								*/
	rpos = fbin->rpos;
  
	/*	Run the child filters.		*/
	for (i = 0; i < EQUALIZER_BANDS + 1; i++)
	{
		fbin->rpos = rpos;
		run_lpf(fbin, &data->fb[i], &data->lpf[i]);
		lpf_out[i] = data->fb[i].buff[data->fb[i].rpos++];
	}

	/*								*/
	/*	Now process the results of the filters. Remember 	*/
	/*	that each band is output(hi)-output(lo).		*/
	/*								*/
	for (i = 0; i < EQUALIZER_BANDS; i++)
	{
		sum += (lpf_out[i+1] - lpf_out[i]) * data->gain[i];
	}

	/* Write that result.  */
	fb_ensure_writable(fbout, 1);
	fbout->buff[fbout->rlen++] = sum;

	return;
}

void
nic_hdlr(int evt)
{
	int	whichifc;
	uchar	src_addr[16+1];
	uchar	rxbuf[MAX_FRAMEBYTES];
	uchar	*tdataptr;
	ulong 	tframelen;
	ulong   rx_frame_size;


        /*      Lower 12 bits of interrupt code specify IFC #   */
        whichifc = evt & 0xFFF;
        rx_frame_size = devnet_getmaxfsz(whichifc);
        devnet_recv(&rxbuf[0], rx_frame_size, whichifc);
	memmove(&src_addr, (uchar *)&rxbuf[0], 16);

	/*	Get payload and length		*/
	tdataptr = (uchar *)&rxbuf[0]+37;
	tframelen = 0;
	tframelen |= rxbuf[32] << 24;
	tframelen |= rxbuf[33] << 16;
	tframelen |= rxbuf[34] << 8;
	tframelen |= rxbuf[35];

	memmove((void *)&fb3, tdataptr, sizeof(fb3));
	sample_flag = 1;

	/*
	print("swradio-eq [%s] : Just got frame from node [%s], length [%d]\n",\
		NIC_OUI, &src_addr[0], tframelen);
	print("fb3.rpos = [%d], fb3.rlen = [%d]\n", fb3.rpos, fb3.rlen);
	*/


	return;
}

void
intr_hdlr(void)
{
	int	evt = devexcp_getintevt();
	int	whichifc = evt & 0xFFF;


	/*	Only call nic_hdlr() for RX_OK interrupts	*/
	if ((evt >= NIC_RX_EXCP_CODE) && (evt < NIC_RX_EXCP_CODE_END))
	{
		nic_hdlr(evt);
	}

	return;
}


void
hdlr_install(void)
{
	extern	uchar	vec_stub_begin, vec_stub_end;
	uchar	*dstptr = (uchar *)0x8000600;
	uchar	*srcptr = &vec_stub_begin;


	/*	Copy the vector instructions to vector base	*/
	while (srcptr < &vec_stub_end)
	{
		/*	print("0x%x\n", *srcptr);	*/
		*dstptr++ = *srcptr++;
	}

	return;
}

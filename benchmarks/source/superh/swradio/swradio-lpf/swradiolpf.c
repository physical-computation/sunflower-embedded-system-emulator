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
#include "../../port/devnet.h"
#include "../../port/devrand.h"
#include "../../port/devrtc.h"
#include "../../port/devloc.h"
#include "../../port/devlog.h"
#include "../../port/devexcp.h"
#include "../../port/superHspecific.h"
#include "print.h"
#include "../../port/misc.h"
#include "fault.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "network-hitachi-sh.h"
#include "batt.h"
#include "swradio.h"


/*	    	8 GPRs + PR:			*/
uchar			REGSAVESTACK[36];
volatile int		done_flag = 0;
volatile int		sample_flag = 0;
volatile int		syn_sent = 0;
volatile int		my_id = -1;
volatile FloatBuffer	fb1, fb2;

static void	hdlr_install(void);
extern int	pow10(int y);
extern long	strlen(uchar *s);
extern long	strcmp(uchar *s1, uchar *s2);
extern int 	strncmp(uchar *s1, uchar *s2, ulong n);
extern uchar*	strncpy(uchar *s1, uchar *s2, ulong n);
extern void* 	memmove(void *, void *, long);


int
main(void)
{
        /*      Monitor code is @ 0x8001000     */
        void    	(*fptr)() = (void *)0x8001000;
	int		frames_rcvd = 0, addrchars, i;
	uchar		demod_dst[16+1];
	LPFData		lpf_data;


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
	sprint(&demod_dst[0], "%d", my_id+1);

	hdlr_install();

	/*	Power up both RX interfaces	*/
	devnet_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, 0);
	devnet_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, 1);

	print("swradio-lpf [%d] installing vector code...\n", my_id);



	/*	Make sure we're talking on regular segment		*/
	/*	uncommenmt if doing segmented network	*/
	sample_flag = 0;
	syn_sent = 0;

	fb1.rpos = fb1.rlen = 0;
	fb2.rpos = fb2.rlen = 0;
	init_lpf_data(&lpf_data, CUTOFF_FREQUENCY, NUM_TAPS, DECIMATION);

	/*	LPF needs at least NUM_TAPS+1 inputs; get_floats is fine.	*/
	while (!sample_flag)
	{
		sleep();
	}
	run_lpf((void *)&fb1, (void *)&fb2, &lpf_data);
	sample_flag = 0;

	/*									*/
	/*	run_demod needs 1 input, OK here. run_equalizer needs 51 	*/
	/*	inputs (same reason as for LPF).  This means running the 	*/
	/*	pipeline up to demod 50 times in advance: 			*/
	/*									*/
	while (frames_rcvd < NUM_TAPS)
	{
		while (!sample_flag)
		{
			sleep();
		}

		frames_rcvd++;
		sample_flag = 0;
		run_lpf((void *)&fb1, (void *)&fb2, &lpf_data);

			/*
			print("swradio-lpf (node [%d]) sending [%d] bytes to DEMOD (node [%s])\n",
				my_id, sizeof(fb1), demod_dst);
			print("fb2.rpos = [%d], fb2.rlen = [%d]\n", fb2.rpos, fb2.rlen);
			*/

		devnet_xmit((uchar *)&demod_dst, 0, (uchar *)&fb2, sizeof(fb2), 1);
		/*									*/
		/*	Since we are doing Call-By-Value, we must make this change 	*/
		/*	here. The corresponding calculation in run_demod is a No-op	*/
		/*									*/
		fb2.rpos++;
	}

	/*	Wait till intr_hdlr is done	*/
	done_flag = 0;
	while (!done_flag)
	{
		if (sample_flag)
		{
			run_lpf((void *)&fb1, (void *)&fb2, &lpf_data);

			
			/*
			print("swradio-lpf (node [%d]) sending [%d] bytes to DEMOD (node [%s])\n",
				my_id, sizeof(fb1), demod_dst);
			print("fb2.rpos = [%d], fb2.rlen = [%d]\n", fb2.rpos, fb2.rlen);
			*/


			/*	Now send fb2 to be Demod'd. We dont re-xmit	*/
			devnet_xmit((uchar *)&demod_dst, 0, (uchar *)&fb2, sizeof(fb2), 1);
			/*									*/
			/*	Since we are doing Call-By-Value, we must make this change 	*/
			/*	here. The corresponding calculation in run_demod is a No-op	*/
			/*									*/
			fb2.rpos++;

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

	memmove((void *)&fb1, tdataptr, sizeof(fb1));
	sample_flag = 1;

	/*
	print("swradio-lpf [%s] : Just got frame from node [%s], length [%d]\n",\
		NIC_OUI, &src_addr[0], tframelen);
	print("fb1.rpos = [%d], fb1.rlen = [%d]\n", fb1.rpos, fb1.rlen);
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

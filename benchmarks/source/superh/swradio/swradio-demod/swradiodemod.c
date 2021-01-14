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


/*	    	8 GPRs + PR		*/
uchar			REGSAVESTACK[36];
volatile int		done_flag = 0;
volatile int		sample_flag = 0;
volatile int		syn_sent = 0;
volatile int		my_id = -1;
volatile FloatBuffer	fb2, fb3;

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
        void		(*fptr)() = (void *)0x8001000;
	int		eq_base, frames_rcvd = 0, cur_eq = 0, addrchars, i;
	uchar		eq_dst[16+1];


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
	eq_base = my_id+1;

	hdlr_install();

	/*	Power up both RX interfaces	*/
	devnet_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, 0);
	devnet_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, 1);

	print("swradio-demod [%d] installing vector code...\n", my_id);



	/*	Make sure we're talking on regular segment		*/
	/*	uncommenmt if doing segmented network	*/
	sample_flag = 0;
	syn_sent = 0;

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
		run_demod((void *)&fb2, (void *)&fb3);
	}

	/*	Wait till intr_hdlr is done	*/
	done_flag = 0;
	while (!done_flag)
	{
		if (sample_flag)
		{
			run_demod((void *)&fb2, (void *)&fb3);


			/*
			print("swradio-demod (node [%d]) sending [%d] bytes to EQ (node [%s])\n",
				my_id, sizeof(fb3), eq_dst);
			print("fb3.rpos = [%d], fb3.rlen = [%d]\n", fb3.rpos, fb3.rlen);
			*/

			sprint(&eq_dst[0], "%d", eq_base + cur_eq++);
			cur_eq = cur_eq % (NUM_SUPERPIPE_EQ);

			devnet_xmit((uchar *)&eq_dst, 0, (uchar *)&fb3, sizeof(fb3), 1);
			/*									*/
			/*	Since we are doing Call-By-Value, we must make this change 	*/
			/*	here. The corresponding calculation in run_lpf is a No-op	*/
			/*									*/
			fb3.rpos++;

			sample_flag = 0;
		}

		sleep();
	}

	/*	Forcefully go to monitor	*/
	fptr();


	return 0;		
}


void
run_demod(FloatBuffer *fbin, FloatBuffer *fbout)
{
	float temp, gain;


	gain = MAX_AMPLITUDE * SAMPLING_RATE / (BANDWIDTH * M_PI);
	temp = fbin->buff[fbin->rpos] * fbin->buff[fbin->rpos + 1];
	temp = gain * atan(temp);
	fbin->rpos++;
	fb_ensure_writable(fbout, 1);
	fbout->buff[fbout->rlen++] = temp;


	return;
}


void
nic_hdlr(int evt)
{
	int	whichifc;
	uchar	rxbuf[MAX_FRAMEBYTES];
	uchar	src_addr[16+1];
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

	memmove((void *)&fb2, tdataptr, sizeof(fb2));
	sample_flag = 1;

	/*
	print("swradio-demod [%s] : Just got frame from node [%s], length [%d]\n",\
		NIC_OUI, &src_addr[0], tframelen);
	print("fb2.rpos = [%d], fb2.rlen = [%d]\n", fb2.rpos, fb2.rlen);
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

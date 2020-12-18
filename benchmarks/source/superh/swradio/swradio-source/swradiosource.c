/*									*/
/*	Adapted from							*/
/*		"fmref.c: C reference implementation of FM Radio	*/
/*		David Maze <dmaze@cag.lcs.mit.edu>			*/
/*		$Id: fmref.c,v 1.8 2002/07/29 14:05:48 aalamb Exp $"	*/
/*	by								*/
/*		pstanley@ece.cmu.edu					*/
/*									*/
#include "e-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include "devscc.h"
#include "../../../port/devnet.h"
#include "../../../port/devrtc.h"
#include "../../../port/devexcp.h"
#include "../../../port/devlog.h"
#include "../../../port/misc.h"
#include "../../../port/superHspecific.h"
#include "fault.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "network-hitachi-sh.h"
#include "print.h"
#include "batt.h"
#include "swradio.h"


/*	    8 GPRs + PR		*/
uchar		REGSAVESTACK[36];

volatile int	done_flag;
volatile int	syn_sent_flag = 0;

static void	hdlr_install(void);
extern int	pow10(int y);
extern long	strlen(uchar *s);
extern long	strcmp(uchar *s1, uchar *s2);
extern int 	strncmp(uchar *s1, uchar *s2, ulong n);
extern uchar*	strncpy(uchar *s1, uchar *s2, ulong n);
extern void* 	memmove(void *, void *, long);
extern void	xusleep(ulong usecs);

enum
{
	SLOWDOWN = 8,

	/*	Delay between sending out samples down pipeline		*/
	PERIOD = 125*SLOWDOWN,
};


int
main(void)
{
	FloatBuffer	fb1;
	char		lpf_dst[16+1];
	int		i, my_id = *NIC_OUI - '0';
	ulong		lastxmit = 0;


	/*	If we are the clone of someone, my_id is already set, keep it, for timeslot stuff	*/
	if (my_id == -1)
	{
		int addrchars = strlen((char *)NIC_OUI);

		my_id = 0;

		for (i = addrchars-1; i >= 0; i--)
		{
			my_id +=  (*(NIC_OUI+i) - '0') * pow10(addrchars - 1 - i);
		}
	}
	sprint(&lpf_dst[0], "%d", my_id+1);

	hdlr_install();
	print("swradio-source [%d] installing vector code...\n", my_id);


	fb1.rpos = fb1.rlen = 0;

	/*				Startup:				*/
	get_floats(&fb1);

	/*	LPF needs at least NUM_TAPS+1 inputs; get_floats is fine.	*/
	print("swradio-source (node [%d]) sending [%d] bytes (init0) to LPF (node [%s])\n",
		my_id, sizeof(fb1), lpf_dst);

	while (devnet_xmit((uchar *)&lpf_dst, 0, (uchar *)&fb1, sizeof(fb1), 0) < 0)
	{
		/*	print("swradio-source re-transmitting...\n");	*/
		sleep();
	}
	/*									*/
	/*	Since we are doing Call-By-Value, we must make this change 	*/
	/*	here. The corresponding calculation in run_lpf is a No-op	*/
	/*									*/
	fb1.rpos += DECIMATION + 1;


	/*	First bundle takes a while for LPF to process, and we also	*/
	/*	account for equalizer_init taking a long time.			*/
	xusleep(0.5E6);

	/*									*/
	/*	run_demod needs 1 input, OK here. run_equalizer needs 51 	*/
	/*	inputs (same reason as for LPF).  This means running the 	*/
	/*	pipeline up to demod 50 times in advance: 			*/
	/*									*/
	for (i = 0; i < 64; i++)
	{
		if (fb1.rlen - fb1.rpos < NUM_TAPS + 1)
		{
			get_floats(&fb1);
		}

		while (devnet_xmit((uchar *)&lpf_dst, 0, (uchar *)&fb1, sizeof(fb1), 0) < 0)
		{
			/*	print("swradio-source re-transmitting...\n");	*/
			sleep();
		}

		/*									*/
		/*	Since we are doing Call-By-Value, we must make this change 	*/
		/*	here. The corresponding calculation in run_lpf is a No-op	*/
		/*									*/
		fb1.rpos += DECIMATION + 1;
	
		/*	Wait until start of next sampling period	*/
		print("(init) time left = [%d]\n", PERIOD - (devrtc_getusecs() - lastxmit));
		while ((devrtc_getusecs() - lastxmit) < PERIOD)
		{
			sleep();
		}

		lastxmit = devrtc_getusecs();
	}

	while (1)
	{
    		/*								*/
		/*	The low-pass filter will need NUM_TAPS+1 items; read 	*/
		/*	them if we need to. 					*/
		/*								*/
    		if (fb1.rlen - fb1.rpos < NUM_TAPS + 1)
		{
      			get_floats(&fb1); 
		}


		/*	Now send fb1 to be LPF'd	*/
		print("swradio-source (node [%d]) sending [%d] bytes (steady) to LPF (node [%s])\n",
			my_id, sizeof(fb1), lpf_dst);
		print("fb1.rpos = [%d], fb1.rlen = [%d]\n", fb1.rpos, fb1.rlen);


		while (devnet_xmit((uchar *)&lpf_dst, 0, (uchar *)&fb1, sizeof(fb1), 0) < 0)
		{
			/*	print("swradio-source re-transmitting...\n");	*/
			sleep();
		}
		lprint("sentsample @ %dus", devrtc_getusecs());

		/*									*/
		/*	Since we are doing Call-By-Value, we must make this change 	*/
		/*	here. The corresponding calculation in run_lpf is a No-op	*/
		/*									*/
		fb1.rpos += DECIMATION + 1;


		/*	Wait until start of next sampling period	*/
		print("(steady) time left = [%d]\n", PERIOD - (devrtc_getusecs() - lastxmit));
		while ((devrtc_getusecs() - lastxmit) < PERIOD)
		{
			sleep();
		}
		lastxmit = devrtc_getusecs();
	}


	return 0;		
}


void
get_floats(FloatBuffer *fb)
{
	fb_compact(fb);
  
	/*	Fill the remaining space in fb with 1.0.	*/
	while (fb->rlen < IN_BUFFER_LEN)
	{
		fb->buff[fb->rlen++] = 1.0;
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
	memmove(&src_addr, &rxbuf[0], 16);

	/*	Get payload and length		*/
	tdataptr = &rxbuf[0]+37;
	tframelen = 0;
	tframelen |= rxbuf[32] << 24;
	tframelen |= rxbuf[33] << 16;
	tframelen |= rxbuf[34] << 8;
	tframelen |= rxbuf[35];

	/*	DEBUG	*/
	print("swradio-source : Just got frame from node [%s], length [%d]\n",
		&src_addr[0], tframelen);


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


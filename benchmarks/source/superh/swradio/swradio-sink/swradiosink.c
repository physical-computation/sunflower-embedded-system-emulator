/*									*/
/*	Adapted from							*/
/*		"fmref.c: C reference implementation of FM Radio	*/
/*		David Maze <dmaze@cag.lcs.mit.edu>			*/
/*		$Id: fmref.c,v 1.8 2002/07/29 14:05:48 aalamb Exp $"	*/
/*	by								*/
/*		pstanley@ece.cmu.edu					*/
/*									*/
#include <stdio.h>
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
#include "batt.h"
#include "print.h"
#include "swradio.h"


enum
{
	MAX_SAMPLES = 1024,
};

/*	    	8 GPRs + PR			*/
uchar			REGSAVESTACK[36];
uchar			rxbuf[MAX_FRAMEBYTES];
volatile int		done_flag;
volatile int		syn_sent_flag = 0;
volatile int		sample_flag = 0;
volatile int		my_id = -1;
volatile int		rx_sample_count = 0;
volatile FloatBuffer	fb4;

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
        void    	(*fptr)() = (void *)0x8001000;
	int		addrchars, i;


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

	hdlr_install();

	/*	Power up the RX interface	*/
	devnet_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, 0);

	print("swradio-sink [%d] installing vector code...\n", my_id);


	/*	Wait till intr_hdlr is done	*/
	done_flag = 0;
	while (!done_flag)
	{
		if (sample_flag)
		{
			write_floats((void *)&fb4);
			sample_flag = 0;
		}

		sleep();
	}

	/*	Forcefully go to monitor	*/
	fptr();



	return 0;		
}


void write_floats(FloatBuffer *fb)
{
	/*	printf() any data that's available:	*/
	while (fb->rpos < fb->rlen)
	{
		printf("%f\n", fb->buff[fb->rpos++]);
	}
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

	memmove((void *)&fb4, tdataptr, sizeof(fb4));
	sample_flag = 1;

	print("swradio-sink (node [%d]) : Just got frame from node [%s], length [%l]\n",
		my_id, &src_addr[0], tframelen);
	print("fb4.rpos = [%d], fb4.rlen = [%d]\n", fb4.rpos, fb4.rlen);

	if (rx_sample_count++ >= MAX_SAMPLES)
	{
		print("\n\n\nReceived %d SAMPLES, halting simulator using devlog_ctl()...\n\n", rx_sample_count);
		devlog_ctl("off\n");
	}


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
		/*	printf("0x%x\n", *srcptr);	*/
		*dstptr++ = *srcptr++;
	}

	return;
}


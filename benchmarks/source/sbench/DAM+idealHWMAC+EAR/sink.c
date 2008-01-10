/*										*/
/*	EAR-Sink.  Periodically query a random node in the network.		*/
/*										*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "e-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include "devscc.h"
#include "devnet.h"
#include "devmac.h"
#include "devrtc.h"
#include "devexcp.h"
#include "devlog.h"
#include "devloc.h"
#include "devrand.h"
#include "devsensor.h"
#include "misc.h"
#include "fault.h"
#include "physics.h"
#include "interrupts-hitachi-sh.h"
#include "network-hitachi-sh.h"
#include "print.h"
#include "batt.h"
#include "errors.h"
#include "ear-route.h"

enum
{
	ID_LEN 			= 16,
	EAR_SINK_PERIOD		= 1000000,
	NETWORK_SIZE 		= 25,
	PROTO_DAM_INTERNAL	= 0,
};


#ifdef LOGMARKERS
#	include "logmarkers.h"
#endif

/*	    8 GPRs + PR		*/
uchar			REGSAVESTACK[36];
EARState		*Ear;
volatile uchar		myID[ID_LEN+1];
char			src[ID_LEN+1];
int			period = 0;


/*						*/
/*	The bcast addr for idealized MAC.  	*/
/*	Route layer uses this variable 		*/
/*	when it needs to			*/
/*						*/
uchar			MAC_BCAST_ADDR[] = "::1";

static void		hdlr_install(void);
static void		write_log(ulong period, char *, int);
static ulong		uniform(ulong min, ulong max);

void			fatal(char *str);

extern int		pow10(int y);



static ulong
uniform(ulong min, ulong max)
{
	return min + (devrand_getrand() % ((max-min)+1));
}

void
startup(int argc, char *argv[])
{
	char		tmp;
	char		*ep = &tmp;
	ulong		start = 0;
	long		sluimer;
	int		ear_sink_period;
	int		tmpperiod, tmp_alpha, tmp_beta, tmp_ftable_alpha,
			tmp_eij, tmp_maxhops;


	strncpy(myID, (char *)NIC_OUI, ID_LEN);
	hdlr_install();
	print("EAR-sink node [%s] installed vector code.\n", myID);

	/*								*/
	/*	Since we are running over idealized MAC, we need to	*/
	/*	take care of this ourselves here.			*/
	/*								*/
	devmac_ctl(NIC_NCR_WRITE, NIC_CMD_POWERUP, 0);
	print("EAR-sink node [%s] powered up NIC.\n", myID);

	ear_sink_period = EAR_SINK_PERIOD;
	
	if (argc == 6)
	{
		tmpperiod = strtol(argv[0], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR-sink period (\"%s\") supplied as argument.\n",
				argv[0]);
		}
		else
		{
			ear_sink_period = tmpperiod;
			printf("Set ear_sink_period to [%d] usecs\n", ear_sink_period);
		}

		tmp_alpha = strtol(argv[1], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR alpha supplied as argument.\n");
		}
		
		tmp_beta = strtol(argv[2], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR beta supplied as argument.\n");
		}

		tmp_ftable_alpha = strtol(argv[3], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR ftable_alpha supplied as argument.\n");
		}

		tmp_eij = strtol(argv[4], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR eij supplied as argument.\n");
		}

		tmp_maxhops = strtol(argv[5], &ep, 0);
		if (*ep != '\0')
		{
			printf("Invalid EAR maxhops supplied as argument.\n");
		}

		/*								*/
		/*		Initalize, EAR routing state			*/
		/*								*/
		Ear = ear_init(myID, 0, tmp_alpha, tmp_beta, tmp_ftable_alpha,
				tmp_eij, tmp_maxhops,
			(uchar)devloc_getxloc(), (uchar)devloc_getyloc(), (uchar)devloc_getzloc());
		fprintf(stderr, "EAR Sink node [%s] completed EAR init with runtime args:\n"
			"\talpha=%d\n\tbeta=%d\n\tftable_alpha=%d\n\teij=%d\n\tmaxhops=%d\n\n\n\n", 
			myID, tmp_alpha, tmp_beta, tmp_ftable_alpha, tmp_eij, tmp_maxhops);

	
	}
	else
	{
		/*								*/
		/*		Initalize, EAR routing state			*/
		/*								*/
		Ear = ear_init(myID, 0, 1, 50, 1, 1 /* eij */, 8 /* maxhops */,
			(uchar)devloc_getxloc(), (uchar)devloc_getyloc(), (uchar)devloc_getzloc());
		print("EAR Sink node [%s] completed EAR init using defaults.\n\n\n", myID);
	}

	while (1)
	{
		start = devrtc_getusecs();

		snprintf(src, ID_LEN, "%d", uniform(0, NETWORK_SIZE-1));
		printf("\t\tSending out \"pull\" request to node [%s]\n", src);

NETTRACEMARK(14);
LOGMARK(22);
		ear_request(Ear, src, NULL, 0, 0);
LOGMARK(23);
NETTRACEMARK(15);

		sluimer = ear_sink_period - (devrtc_getusecs() - start);
		sluimer = max(sluimer, 0);
LOGMARK(2);
		xusleep(sluimer);
LOGMARK(3);
		period++;
	}

	return;		
}

void
write_log(ulong period, char *src, int leader)
{
#	define	buflen	1024
	char	buf[buflen];


	snprintf(buf, buflen,
		"\t\t@ %lu:\tdevrtc_getusecs = [%lu]\n", period, devrtc_getusecs());
	lprint(buf);
	snprintf(buf, buflen,
		"\t\t@ %lu:\tPeak source = [%s], Peak's leader = [%d]\n", period, src, leader);
	lprint(buf);
	snprintf(buf, buflen, "\n");
	lprint(buf);

	return;
}

void
nic_hdlr(int evt)
{
	int			whichifc;
	char			proto;
	uchar			src_addr[16+1];
	uchar			*tdataptr;
	ulong 			tframelen;
	ulong   		rx_frame_size;
	EARSrcPayloadTuple	*x;
	uchar			*rxbuf;


        /*      Lower 12 bits of interrupt code specify IFC #   */
        whichifc = evt & 0xFFF;

NETTRACEMARK(8);
LOGMARK(0);
	rx_frame_size = devnet_getrxfsz(whichifc);
//@printf("\tRX frame size is %d\n", rx_frame_size);

	rxbuf = (uchar *)calloc(rx_frame_size, sizeof(uchar));
	if (rxbuf == NULL)
	{
		fatal(Emalloc);
	}


        devnet_recv(rxbuf, rx_frame_size, whichifc);
	memmove(&src_addr[0], rxbuf, 16);

	/*	Get payload and length		*/
	tdataptr = &rxbuf[0]+37;
	tframelen = 0;
	tframelen |= rxbuf[32] << 24;
	tframelen |= rxbuf[33] << 16;
	tframelen |= rxbuf[34] << 8;
	tframelen |= rxbuf[35];

	proto = rxbuf[36];
LOGMARK(1);
NETTRACEMARK(9);

//@printf("EAR-sink got packet\n");


	/*	DEBUG	*/
//BUG: the src_addr is not a null terminated string, so we need to be careful
//(e.g., print at most 2 chars of it we really should be using routines like
//netbyte2int etc to canonicalize

	printf("\t\tEAR-sink node [%s]: Just got frame from node [%s], length [%lu], proto [%s]\n",
		myID, src_addr, tframelen,
		(proto == PROTO_DAM_INTERNAL ? "DAM" : (proto == PROTO_EAR ? "EAR" : "UNKNOWN")));

	if (proto == PROTO_EAR)
	{
NETTRACEMARK(12);
LOGMARK(18);
		x = ear_recv(Ear, tdataptr, tframelen);
LOGMARK(19);
NETTRACEMARK(13);
		if (x != NULL)
		{
			write_log(period, x->src, x->nxthdr);
		}
		free(x);

		return;
	}


	return;
}

void
intr_hdlr(void)
{
	int	evt = devexcp_getintevt();


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
		*dstptr++ = *srcptr++;
	}

	return;
}

//TODO: move this into an "ideal-mac" directory, and use the
//	same interfaces we use for sift/smac/etc. to channel
//	data over devnet_xmit, using the idealized MAC layer
int
mac_xmit(uchar *ear_dst_addr, uchar *ear_hdr, int hdrlen, uchar *data, int datalen)
{
//BUG: for some reason, any access to the arg 'hdrlen' freaks the CPU out...
// could it be because of where on the stack it is ?
//@fprintf(stderr, "\n\nmac_xmit(), ear_dst_addr=[%s]\n", ear_dst_addr);
//@fprintf(stderr, "hdrlen=%d\n\n", hdrlen);

	return devnet_xmit(ear_dst_addr, PROTO_EAR, ear_hdr, hdrlen, 0);
}


void
fatal(char *str)
{
	lprint("Node [%s] Fatal: %s\n", myID, str);
	exit(-1);
}

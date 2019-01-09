/*										*/
/*	EAR-Sink.  Periodically query a random node in the network.		*/
/*										*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "sf-types.h"
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
#include "s-mac.h"

enum
{
	ID_LEN 			= 16,
	EAR_DEFAULT_SINK_PERIOD	= 1000000,
	EAR_MAX_TRIES		= 5,
	NETWORK_SIZE 		= 25,
	PROTO_DAM_INTERNAL	= 0,
};


/*	    8 GPRs + PR		*/
uchar			REGSAVESTACK[36];
EARState		*Ear;
SMACstate		*S_MAC;

volatile uchar		myID[ID_LEN+1];
char			src[ID_LEN+1];
int			period = 0;


/*						*/
/*	The bcast addr for idealized MAC.  	*/
/*	Route layer uses this variable 		*/
/*	when it needs to			*/
/*						*/
uchar			MAC_BCAST_ADDR[2] = {~0, ~0};

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
main(int argc, char *argv[])
{
	char		tmp;
	char		*ep = &tmp;
	ulong		start = 0;
	long		sluimer;
	int		ear_sink_period;
	int		tmpperiod, tmp_alpha, tmp_beta, tmp_ftable_alpha,
			tmp_eij, tmp_maxhops;
	SMACstate	*smac_tmp;


	S_MAC = NULL;
	strncpy(myID, (char *)NIC_OUI, ID_LEN);
	hdlr_install();

	print("EAR Sink node [%s] done installing vector code...\n", myID);

	print("EAR Sink node [%s] initializing S-MAC...", myID);

	/*									*/
	/*	->	syncsleep must be much smaller than initsleep		*/
	/*		so that the chances of the SYNC going out		*/
	/*		from a node before the other node has finished		*/
	/*		its initsleep are high.  Also, keeping the		*/
	/*		"sleep frame" (listen_usecs + sleep_usecs) small	*/
	/*		keeps latencies smaller.				*/
	/*									*/
	/*	NOTE: Make sure S_MAC is not set till init and reset done.	*/
	/*									*/
	smac_tmp = smac_init(&myID[0], 0,  /*	ID and IFC #0		*/
			100000,		/*	listen_usecs		*/
			0,		/*	sleep_usecs		*/
			60000000,	/*	sync_usecs		*/
			500000,		/*	extension_usecs 	*/
			1000,		/*	max_initsleep_slots	*/
			100,		/*	max_syncsleep_slots	*/
			10000,		/*	slot_usecs		*/
			2,		/*	sync_slots		*/
			8);		/*	data_slots		*/
	smac_reset(smac_tmp);
	S_MAC = smac_tmp;
	print("done.\n");

	ear_sink_period = EAR_DEFAULT_SINK_PERIOD;
	
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
		ear_request(Ear, src, NULL, 0, 0);

		sluimer = ear_sink_period - (devrtc_getusecs() - start);
		sluimer = max(sluimer, 0);
		xusleep(sluimer);
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
	EARSrcPayloadTuple	*x;
	int			whichifc;
	SMACframe		*payload;
	

	/*      Lower 12 bits of interrupt code specify IFC #   	*/
	whichifc = evt & 0xFFF;
	
	/*	Let the MAC layer take care of whatever it needs to	*/
	smac_nichdlr(S_MAC, whichifc);

	/*								*/
	/*	Now, retrieve the payload from the MAC layer, if any	*/
	/*								*/
	payload = smac_receive(S_MAC, T_DATA);
	if (payload == NULL)
	{
		printf("\t\tEAR Sink node [%s]: Just got non-data frame\n", myID);

		return;
	}

	/*	DEBUG	*/
	printf("\t\tEAR Sink node [%s]: Just got frame from node [%s], length [%d], proto [%s/%d]\n",
		myID, payload->src, payload->len,
		(payload->nxthdr == PROTO_DAM_INTERNAL ? "DAM" : (payload->nxthdr == PROTO_EAR ? "EAR" : "UNKNOWN")), payload->nxthdr);

	if (payload->nxthdr == PROTO_EAR)
	{
		if ((x = ear_recv(Ear, payload->data, payload->len)) != NULL)
		{
			write_log(period, x->src, x->nxthdr);
		}
		free(x);

		return;
	}
	smac_freeframe(payload);


	return;
}

void
intr_hdlr(void)
{
	int	evt = devexcp_getintevt();


	/*	Only call nic_hdlr() for RX_OK interrupts	*/
	if ((evt >= NIC_RX_EXCP_CODE) && (evt < NIC_RX_EXCP_CODE_END))
	{
		/*	Only begin triggering when smac_init is done	*/
		if (S_MAC != NULL)
		{
			nic_hdlr(evt);
		}
	}
	else if (evt == TMU0_TUNI0_EXCP_CODE)
	{
		/*	Only begin triggering when smac_init is done	*/
		if (S_MAC != NULL)
		{
			smac_timerhdlr(S_MAC);
		}
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

//	return devnet_xmit(ear_dst_addr, PROTO_EAR, ear_hdr, hdrlen, 0);

	return smac_transmit(S_MAC, ear_dst_addr, ear_hdr, hdrlen, EAR_MAX_TRIES, PROTO_EAR);
}


void
fatal(char *str)
{
	lprint("Node [%s] Fatal: %s\n", myID, str);
	exit(-1);
}

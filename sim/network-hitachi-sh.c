/*										*/
/*		Copyright (C) 1999-2006 Phillip Stanley-Marbell.		*/
/*										*/
/*	You may redistribute and/or copy the source and binary			*/
/*	versions of this software provided:					*/
/*										*/
/*	1.	The source retains the copyright notice above, 			*/
/*		this list of conditions, and the following 			*/
/*		disclaimer.							*/
/*										*/
/*	2.	Redistributed binaries must reproduce the above			*/
/*		copyright notice, this list of conditions, and			*/
/*		the following disclaimer.					*/
/*										*/
/*	3.	The source and binaries are used for non-commercial 		*/
/*		purposes only.							*/
/*										*/
/*	4.	Permission is granted to all others, to redistribute		*/
/*		or make copies of the source or binaries, either		*/
/*		as it was received or with modifications, provided		*/
/*		the above three conditions are enforced and 			*/
/*		preserved, and that permission is granted for 			*/
/*		further redistribution, under the conditions of this		*/
/*		document.							*/
/*										*/
/*	The source is provided "as is", and no warranty is either		*/
/*	implied or explicitly granted. The authors will not be liable 		*/
/*	for any damages arising in any way out of the use or misuse 		*/
/*	of this software, even if advised of the possibility of such 		*/
/*	damage.									*/
/*										*/
/*	Contact: phillip Stanley-Marbell <pstanley@ece.cmu.edu>			*/
/*										*/	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "sf.h"
#include "mmalloc.h"
#include "mextern.h"

static double		check_snr(Netsegment *curseg, State *src_node, State *dst_node);
static int		fifo_enqueue(State *S, Fifo fifo_name, int whichifc);
static int		lookup_id(uchar*);
static int		seg_enqueue(State *S, int whichifc);
static uchar *		fifo_dequeue(State *S, Fifo fifo_name, int whichifc);
static void		netsegcircbuf(Segbuf *segbuf);
static void		seg_dequeue(Netsegment *curseg, Segbuf *tptr);

enum
{
	OK = 1,

	Ecoll,
	Eaddrerr,
	Ecsense,
	Etxovrrun,
	Erxovrrun,
	Eframeerr,
	Etxunderrun,
};

/*									*/
/*	The fifo_enqueue() and fifo_dequeue() functions just update	*/
/*	the FIFO indeces.						*/
/*									*/
tuck int
fifo_enqueue(State *S, Fifo which_fifo, int whichifc)
{
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if (which_fifo == TX_FIFO)
	{
		if (ifcptr->tx_fifo_size <= 0)
		{
			mexit("Invalid TX FIFO size", -1);
		}

		if (ifcptr->IFC_TXFIFO_LEVEL == ifcptr->tx_fifo_size)
		{
			ifcptr->IFC_CNTR_TXOVRRUN_ERR++;
			ifcptr->IFC_NSR |= (1 << 0);
	
			if (S->superH->txovrrunerr_intrenable_flag)
			{
				pic_intr_enqueue(S, S->superH->nicintrQ, NIC_TXOVRRUNERR_INTR, whichifc, 0);
			}
		
			return Etxovrrun;
		}
		else
		{
			ifcptr->IFC_TXFIFO_LEVEL++;
		}

		ifcptr->tx_fifo_curidx++;
		ifcptr->tx_fifo_curidx %= ifcptr->tx_fifo_size;

		if (ifcptr->IFC_TXFIFO_LEVEL > ifcptr->tx_fifo_maxoccupancy)
		{
			ifcptr->tx_fifo_maxoccupancy = ifcptr->IFC_TXFIFO_LEVEL;
		}
	}
	else
	{
		if (ifcptr->rx_fifo_size <= 0)
		{
			mexit("Invalid TX FIFO size", -1);
		}

		if (ifcptr->IFC_RXFIFO_LEVEL == ifcptr->rx_fifo_size)
		{
			ifcptr->IFC_CNTR_RXOVRRUN_ERR++;
			ifcptr->IFC_NSR |= (1 << 0);
	
			/*							*/
			/*	We must remove the oldest queued RX intr 	*/
			/*	whose data is going to get clobbered, o/w	*/
			/*	We'd eventually have a RXUNDERRUN as a 		*/
			/*	result of that interrupt being raised and no	*/
			/*	data to consume.				*/
			/*							*/
			pic_intr_clear(S, S->superH->nicintrQ, NIC_RXOK_INTR, 0 /* clear just one intr */);

			/*	RX err intrs if enbld.	*/
			if (S->superH->rxovrrunerr_intrenable_flag)
			{
				pic_intr_enqueue(S, S->superH->nicintrQ, NIC_RXOVRRUNERR_INTR, whichifc, 0);
			}
		
			return Erxovrrun;
		}
		else
		{
			ifcptr->IFC_RXFIFO_LEVEL++;
		}

		ifcptr->rx_fifo_curidx++;
		ifcptr->rx_fifo_curidx %= ifcptr->rx_fifo_size;

		if (ifcptr->IFC_RXFIFO_LEVEL > ifcptr->rx_fifo_maxoccupancy)
		{
			ifcptr->rx_fifo_maxoccupancy = ifcptr->IFC_RXFIFO_LEVEL;
		}
	}


	return	OK;
}

/*									*/
/*	The fifo_enqueue() and fifo_dequeue() functions just update	*/
/*	the FIFO indeces.						*/
/*									*/
tuck uchar *
fifo_dequeue(State *S, Fifo which_fifo, int whichifc)
{
	uchar	*ret_ptr;
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if (which_fifo == TX_FIFO)
	{
		if (ifcptr->tx_fifo_size <= 0)
		{
			mexit("Invalid TX FIFO size", -1);
		}

		if (ifcptr->IFC_TXFIFO_LEVEL == 0)
		{
			ifcptr->IFC_CNTR_TXUNDRRUN_ERR++;
			ifcptr->IFC_NSR |= (1 << 0);
	
			if (S->superH->txundrrunerr_intrenable_flag)
			{
				pic_intr_enqueue(S, S->superH->nicintrQ, NIC_TXUNDRRUNERR_INTR, whichifc, 0);
			}

			/*	No soup for you		*/
			sfatal(S, "fifo_dequeue() returning NULL (TX UNDERRRUN)");
			return NULL;
		}
		else
		{
			ifcptr->IFC_TXFIFO_LEVEL--;
		}
		
		/*	   Caller of fifo_dequeue will copy data immediately	*/
		ret_ptr = ifcptr->tx_fifo[ifcptr->tx_fifo_oldestidx];
		ifcptr->tx_fifo_oldestidx++;
		ifcptr->tx_fifo_oldestidx %= ifcptr->tx_fifo_size;
	}
	else
	{
		if (ifcptr->rx_fifo_size <= 0)
		{
			mexit("Invalid TX FIFO size", -1);
		}

		if (ifcptr->IFC_RXFIFO_LEVEL == 0)
		{
			ifcptr->IFC_CNTR_RXUNDRRUN_ERR++;
			ifcptr->IFC_NSR |= (1 << 0);
	
			if (S->superH->rxundrrunerr_intrenable_flag)
			{
				pic_intr_enqueue(S, S->superH->nicintrQ, NIC_RXUNDRRUNERR_INTR, whichifc, 0);
			}

			/*	No soup for you		*/
			sfatal(S, "fifo_dequeue() returning NULL (RX UNDERRUN)");
			return NULL;
		}
		else
		{
			ifcptr->IFC_RXFIFO_LEVEL--;
		}

		/*	   Caller of fifo_dequeue will copy data immediately	*/
		ret_ptr = ifcptr->rx_fifo[ifcptr->rx_fifo_oldestidx];
		ifcptr->rx_fifo_oldestidx++;
		ifcptr->rx_fifo_oldestidx %= ifcptr->rx_fifo_size;
	}


	return ret_ptr;
}

int
seg_enqueue(State *S, int whichifc)
{
	int			i, si, di;
	Ifc			*ifcptr = &S->superH->NIC_IFCS[whichifc];
	Netsegment		*Seg = &SIM_NET_SEGMENTS[ifcptr->segno];
	uchar			*tptr;
	ulong			cksum = 0;
	int			curwidth = Seg->cur_queue_width;
	char			srcstr[16], dststr[16];		


	si = S->NODE_ID;
	di = lookup_id(S->superH->NIC_IFCS[whichifc].IFC_DST);
	
	snprintf(srcstr, sizeof(srcstr), "%d/%d", si, whichifc);
	snprintf(dststr, sizeof(dststr), "%d", di);
	if (di == -2)
	{
		snprintf(dststr, sizeof(dststr), "(broadcast)");
	}
	if (si == -3)
	{
		snprintf(srcstr, sizeof(srcstr), "(remote)/%d", whichifc);
	}
	
	mprint(S, nodeinfo,
		"Frame from %s --> %s, %Es since epoch\n",
		srcstr, dststr, SIM_GLOBAL_TIME);

	if (!Seg->valid)
	{
		mprint(S, nodeinfo, "Carrier sense error\n");

		S->superH->NIC_IFCS[whichifc].IFC_CNTR_CSENSE_ERR++;
		S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);
		if (S->superH->csenseerr_intrenable_flag)
		{
			pic_intr_enqueue(S, S->superH->nicintrQ, NIC_CSENSEERR_INTR, whichifc, 0);
		}

		return Ecsense;
	}


	if (Seg->busy)
	{
		mprint(S, nodeinfo,
			"Frame collision error (bandwidth = %d, occupancy = %d)\n",
			Seg->queue_max_width, Seg->cur_queue_width);

		S->superH->NIC_IFCS[whichifc].IFC_CNTR_COLLS_ERR++;
		S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);
		if (S->superH->collserr_intrenable_flag)
		{
			pic_intr_enqueue(S, S->superH->nicintrQ, NIC_COLLSERR_INTR, whichifc, 0);
		}

		return Ecoll;
	}

	if (ifcptr->frame_bits != Seg->frame_bits)
	{
		mprint(S, nodeinfo, "Frame error\n");
		
		S->superH->NIC_IFCS[whichifc].IFC_CNTR_FRAME_ERR++;
		S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);
		if (S->superH->frameerr_intrenable_flag)
		{
			pic_intr_enqueue(S, S->superH->nicintrQ, NIC_FRAMEERR_INTR, whichifc, 0);
		}

		return Eframeerr;
	}

	if ((si == -1) || (di == -1))
	{
		mprint(S, nodeinfo,
			"lookup() on either frame source or destination failed\n");
		mprint(S, nodeinfo, "Frame MAC layer address error\n");

		S->superH->NIC_IFCS[whichifc].IFC_CNTR_ADDR_ERR++;
		S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);	
		if (S->superH->addrerr_intrenable_flag)
		{
			pic_intr_enqueue(S, S->superH->nicintrQ, NIC_ADDRERR_INTR, whichifc, 0);
		}

		return Eaddrerr;
	}

	if (di != -2)
	{
		for (i = 0; i < MAX_SEGNODES; i++)
		{
			if (Seg->node_ids[i] == di)
			{
				break;
			}
		}

		/*	If src and dst are not on segment, ADDRERR	*/
		if (i == MAX_SEGNODES)
		{
			mprint(S, nodeinfo, "Destination not on same segment (segment %d)\n",\
				S->superH->NIC_IFCS[whichifc].segno);
			mprint(S, nodeinfo, "Frame MAC layer address error\n");

			S->superH->NIC_IFCS[whichifc].IFC_CNTR_ADDR_ERR++;
			S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);
			if (S->superH->addrerr_intrenable_flag)
			{
				pic_intr_enqueue(S, S->superH->nicintrQ, NIC_ADDRERR_INTR, whichifc, 0);
			}

			return Eaddrerr;
		}
	}
	
	tptr = fifo_dequeue(S, TX_FIFO, whichifc);
	if (tptr == NULL)
	{
		/*	Node interrupt is raised in fifo_dequeue()	*/
		return Etxunderrun;
	}

	memmove(Seg->segbufs[curwidth].data, tptr, ifcptr->frame_bits/8);
	cksum = checksum((uchar *)&Seg->segbufs[curwidth].data, ifcptr->frame_bits/8);
	memmove(&Seg->segbufs[curwidth].data[ifcptr->frame_bits/8], &cksum, sizeof(ulong));

	Seg->segbufs[curwidth].timestamp = SIM_GLOBAL_TIME;
	Seg->segbufs[curwidth].bits_left = ifcptr->frame_bits;
	Seg->segbufs[curwidth].src_node = S;
	Seg->segbufs[curwidth].src_ifc = whichifc;
	Seg->segbufs[curwidth].parent_netsegid = Seg->NETSEG_ID;

	Seg->cur_queue_width++;
	
	/*	Reached medium's bandwidth capacity	*/
	if (Seg->cur_queue_width == Seg->queue_max_width)
	{
		Seg->busy = 1;
	}
		
	
	/*	'-2' is the broadcast destination OUI		*/
	if (di == -2)
	{
		Seg->segbufs[curwidth].bcast = 1;
	}
	else
	{
		Seg->segbufs[curwidth].dst_node = SIM_STATE_PTRS[di];
		Seg->segbufs[curwidth].bcast = 0;
	}

	/*	If netseg is bound to a file, o/p Seg->head	*/
	if (Seg->num_seg2files > 0)
	{
		int i;

		for (i = 0; i < Seg->num_seg2files; i++)
		{
			netsegdump(Seg->seg2filenames[i], &Seg->segbufs[curwidth]);
		}
	}

	if (!S->from_remote)
	{
		netsegcircbuf(&Seg->segbufs[curwidth]);
	}

	S->superH->NIC_IFCS[whichifc].IFC_NSR &= ~(1 << 0);
	SIM_NIC_BYTES_SENT += S->superH->NIC_IFCS[whichifc].frame_bits/8;


	return OK;
}

void
seg_dequeue(Netsegment *curseg, Segbuf *tptr)
{
	curseg->cur_queue_width--;
	if (curseg->cur_queue_width < curseg->queue_max_width)
	{
		curseg->busy = 0;
	}

	return;
}


void
remote_seg_enqueue(Segbuf *segbuf)
{
	State	*S = (State *)segbuf->src_node;
	int	whichifc = segbuf->src_ifc;
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];


	S->from_remote = 1;

	/*	dst_node will be NULL if it was a bcast		*/
	if (segbuf->dst_node != NULL)
	{
		snprintf(ifcptr->IFC_DST, NIC_ADDR_LEN, "%d",
			((State *)segbuf->dst_node)->NODE_ID);
	}
	else
	{
		snprintf(ifcptr->IFC_DST, NIC_ADDR_LEN, "::1");
	}

	memmove(ifcptr->tx_fifo[ifcptr->tx_fifo_curidx], segbuf->data, segbuf->bits_left/8);
	fifo_enqueue(S, TX_FIFO, whichifc);


	return;
}

void
nic_tx_enqueue(State *S, uchar data, int whichifc)
{
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if (!SF_NETWORK)
	{
		mprint(S, nodeinfo,
			"Networking disabled, and you want to do what !!! Exiting !\n");
		mexit("See above messages.", -1);
	}

	if (ifcptr->tx_fifo == NULL || ifcptr->tx_fifo_size <= 0)
	{
		sfatal(S, 
			"Node has bogus tx_fifo. You might have incorrectly configured the IFC");
		return;
	}


	ifcptr->tx_fifo[ifcptr->tx_fifo_curidx][ifcptr->tx_fifo_h2o++] = data;


	/*	Set bit in NSR to indicate data in TDR	*/
	ifcptr->IFC_NSR |= 1 << 2;
	
	if (ifcptr->tx_fifo_h2o == ifcptr->frame_bits/8)
	{
		/*							*/
		/*	We write directly into curidx of FIFO. The	*/
		/*	fifo_enqueue() manages circularity of FIFO.	*/
		/*	All necessary interrupts, if any, are raised	*/
		/*	by fifo_enqueue().				*/
		/*							*/
		fifo_enqueue(S, TX_FIFO, whichifc);
		ifcptr->tx_fifo_h2o = 0;
	}

	return;
}


uchar
nic_rx_dequeue(State *S, int whichifc)
{
	uchar	retchar = 0;
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];
	int	framesize = (int)(ifcptr->frame_bits/8);


	if (!SF_NETWORK)
	{
		mprint(S, nodeinfo,
			"Networking disabled, and you want to do what !!! Exiting !\n");
		mexit("See above messages.", -1);
	}

	if (!ifcptr->rx_fifo)
	{
		sfatal(S,
			"Node is not equipped with an IFC.\nWhat do you mean you want to receive ?!");
	}

	/*									*/
	/*	The structure of the receive action is slightly assymetric	*/
	/*	from that for transmit. For receive, we grab an entry from 	*/
	/*	the FIFO into local buffer whenever local buffer is empty.	*/
	/*									*/
	if (ifcptr->rx_localbuf_h2o == 0)
	{
		uchar *tptr = fifo_dequeue(S, RX_FIFO, whichifc);

		/*	Intrs / marking NSR done in fifo_dequeue	*/
		if (tptr == NULL)
		{
			return (uchar) 0;
		}

		memmove(ifcptr->rx_localbuf, tptr, framesize);
		ifcptr->rx_localbuf_h2o = framesize;
	}

	retchar = ifcptr->rx_localbuf[framesize - ifcptr->rx_localbuf_h2o];
	ifcptr->rx_localbuf_h2o--;


	return retchar;
}

void
tx_retryalg_binexp(void *x, int whichifc)
{
	/*								*/
	/*	This routine is called by network_clock(), and it	*/
	/*	provides the intelligent network interface: It will	*/
	/*	try to move data out of the IFC's FIFO onto a netseg.	*/
	/*								*/

	State		*S = (State *)x;
	double		timeslot, range, delay;
	Ifc		*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if ((ifcptr->IFC_TXFIFO_LEVEL == 0) ||
		(ifcptr->fifo_nextretry_time > SIM_GLOBAL_TIME))
	{
		return;
	}

	/*								*/
	/*	Add in the cost of trying to grab network: 		*/
	/*	tx_pwr watts for 1 clock cycle				*/
	/*								*/
	S->E.current_draw += S->superH->NIC_IFCS[whichifc].tx_pwr / S->VDD;

	/*	The seg_enqueue() failed, so calc. a new retry time	*/
	if (seg_enqueue(S, whichifc) != OK)
	{
		timeslot = (double)ifcptr->frame_bits/
					(double)SIM_NET_SEGMENTS[ifcptr->segno].bitrate;
		range = timeslot * (pow(2, min(ifcptr->tx_alg_retries, 10)) + 1);
		delay = fmod((double)mrandom(), range);

		mprint(S, nodeinfo,
			"Binary exponential backoff for %E seconds, node %d ifc %d\n",
			delay, S->NODE_ID, whichifc);

		ifcptr->fifo_nextretry_time = SIM_GLOBAL_TIME + delay;
		ifcptr->tx_alg_retries++;
	}
	else
	{
		/*							*/
		/*	Interrupt is raised when an entry was sent 	*/
		/*	from FIFO onto network, so if device was	*/
		/*	waiting on a full FIFO, it can do its thing	*/
		/*							*/
		ifcptr->IFC_CNTR_TXOK++;
		if (S->superH->txok_intrenable_flag)
		{
			/*	Enqueue a TX interrupt on the sender	*/
			pic_intr_enqueue(S, S->superH->nicintrQ, NIC_TXOK_INTR, whichifc, 0);
		}

		ifcptr->tx_alg_retries = 0;
	}

	return;
}


void
tx_retryalg_random(void *x, int whichifc)
{
	/*								*/
	/*	This routine is called by network_clock(), and it	*/
	/*	provides the intelligent network interface: It will	*/
	/*	try to move data out of the IFC's FIFO onto a netseg.	*/
	/*								*/

	State		*S = (State *)x;
	double		timeslot, range, delay;
	Ifc		*ifcptr = &S->superH->NIC_IFCS[whichifc];
	int		MAXRANDOMSLOTS = 10;


	if ((ifcptr->IFC_TXFIFO_LEVEL == 0) ||
		(ifcptr->fifo_nextretry_time > SIM_GLOBAL_TIME))
	{
		return;
	}

	/*								*/
	/*	Add in the cost of trying to grab network: 		*/
	/*	tx_pwr watts for 1 clock cycle				*/
	/*								*/
	S->E.current_draw += S->superH->NIC_IFCS[whichifc].tx_pwr / S->VDD;

	if (seg_enqueue(S, whichifc) != OK)
	{
		timeslot = (double)ifcptr->frame_bits/
					(double)SIM_NET_SEGMENTS[ifcptr->segno].bitrate;
		range = timeslot*(MAXRANDOMSLOTS + 1);
		delay = fmod((double)mrandom(), range);

		mprint(S, nodeinfo,
			"Random backoff for %E seconds (range=%E, max=%d slots), node %d ifc %d\n",
			delay, range, MAXRANDOMSLOTS, S->NODE_ID, whichifc);

		ifcptr->fifo_nextretry_time = SIM_GLOBAL_TIME + delay;
		ifcptr->tx_alg_retries++;
	}
	else
	{
		/*							*/
		/*	Interrupt is raised when an entry was sent 	*/
		/*	from FIFO onto network, so if device was	*/
		/*	waiting on a full FIFO, it can do its thing	*/
		/*							*/
		ifcptr->IFC_CNTR_TXOK++;
		if (S->superH->txok_intrenable_flag)
		{
			/*	Enqueue a TX interrupt on the sender	*/
			pic_intr_enqueue(S, S->superH->nicintrQ, NIC_TXOK_INTR, whichifc, 0);
		}

		ifcptr->tx_alg_retries = 0;
	}

	return;
}


void
tx_retryalg_linear(void *x, int whichifc)
{
	//State *S = (State *)x;
}


void
tx_retryalg_asap(void *x, int whichifc)
{
	/*								*/
	/*	This routine is called by network_clock(), and it	*/
	/*	provides the intelligent network interface: It will	*/
	/*	try to move data out of the IFC's FIFO onto a netseg.	*/
	/*								*/

	State		*S = (State *)x;
	double		timeslot;
	Ifc		*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if ((ifcptr->IFC_TXFIFO_LEVEL == 0) ||
		(ifcptr->fifo_nextretry_time > SIM_GLOBAL_TIME))
	{
		return;
	}

	/*								*/
	/*	Add in the cost of trying to grab network: 		*/
	/*	tx_pwr watts for 1 clock cycle				*/
	/*								*/
	S->E.current_draw += S->superH->NIC_IFCS[whichifc].tx_pwr / S->VDD;

	if (seg_enqueue(S, whichifc) != OK)
	{
		timeslot = (double)ifcptr->frame_bits/
					(double)SIM_NET_SEGMENTS[ifcptr->segno].bitrate;

		mprint(S, nodeinfo,
			"Asap backoff for %E seconds (1 timeslot), node %d ifc %d\n",
			timeslot, S->NODE_ID, whichifc);

		ifcptr->fifo_nextretry_time = SIM_GLOBAL_TIME + timeslot;
		ifcptr->tx_alg_retries++;
	}
	else
	{
		/*							*/
		/*	Interrupt is raised when an entry was sent 	*/
		/*	from FIFO onto network, so if device was	*/
		/*	waiting on a full FIFO, it can do its thing	*/
		/*							*/
		ifcptr->IFC_CNTR_TXOK++;
		if (S->superH->txok_intrenable_flag)
		{
			/*	Enqueue a TX interrupt on the sender	*/
			pic_intr_enqueue(S, S->superH->nicintrQ, NIC_TXOK_INTR, whichifc, 0);
		}

		ifcptr->tx_alg_retries = 0;
	}

	return;
}

void
network_clock()
{
	int 			whichbuf, i, j, k;
	Netsegment		*curseg;


	for (i = 0; i < SIM_NUM_ACTIVE_NETSEGS; i++)
	{
		curseg = &SIM_NET_SEGMENTS[SIM_ACTIVE_NETSEGS[i]];
		if (( curseg->cur_queue_width == 0) ||
			!eventready(SIM_GLOBAL_TIME, curseg->lastactivate, curseg->bytedelay))
		{
			continue;
		}

		for (whichbuf = 0; whichbuf < curseg->cur_queue_width; whichbuf++)
		{
			Segbuf		*tptr = &curseg->segbufs[whichbuf];
			State*		dptr = (State *)tptr->dst_node;
			State*		sptr = (State *)tptr->src_node;
			int		frame_bytes = curseg->frame_bits/8;
			int		src_ifc = tptr->src_ifc;


			/*	Timestamp might be in future if spliced from remote host.	*/
			if (tptr->bits_left == 0 || tptr->timestamp > SIM_GLOBAL_TIME)
			{
				continue;
			}

			/*									*/
			/*	Seg -> Nodes: Broadcast. Nodes do not receive a copy of		*/
			/*	their own data on a broadcast.					*/
			/*									*/
			if (tptr->bcast) for (j = 0; j < curseg->num_attached; j++)
			{
				dptr = SIM_STATE_PTRS[curseg->node_ids[j]];
				if (!dptr->runnable || (dptr == tptr->src_node))
				{
					continue;
				}

				/*							*/
				/*	All interfaces attached to this segment get 	*/
				/*	the data					*/
				/*							*/
				for (k = 0; k < dptr->superH->NIC_NUM_IFCS; k++)
				{
					if (dptr->superH->NIC_IFCS[k].segno == SIM_ACTIVE_NETSEGS[i])
					{
						int	offset = frame_bytes - (tptr->bits_left/8);
						int	idx = dptr->superH->NIC_IFCS[k].rx_fifo_curidx;

					
						/*	If we have an associate physical model		*/
						if (curseg->sigsrc != NULL)
						{
							double snr = check_snr(curseg, sptr, dptr);

							if (snr < curseg->minsnr)
							{
								/*	SNR too low. Dest. gets noise	*/
								dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] =
									tptr->data[offset] & mrandom();
							}
							else
							{
								dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] =
									tptr->data[offset];
							}
						}
						else
						{
							dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] =
								tptr->data[offset];
						}

						/*					*/
						/*	Destination power consumption 	*/
						/*	(source handled just once).	*/
						/*					*/
						if (SF_POWER_ANALYSIS)
						{
							dptr->E.current_draw +=
								dptr->superH->NIC_IFCS[k].rx_pwr /
								dptr->VDD;
						}

						/*	If about to become empty:	*/
						if (tptr->bits_left == 8)
						{
							char *ptr = dptr->superH->NIC_IFCS[k].rx_fifo[idx];

							if (checksum(ptr, frame_bytes) !=
								*((ulong *)&tptr->data[frame_bytes]))
							{
								dptr->superH->NIC_IFCS[k].IFC_CNTR_CSUM_ERR++;

								if (dptr->superH->csumerr_intrenable_flag)
								{
									pic_intr_enqueue(dptr,
										dptr->superH->nicintrQ,
										NIC_CSUMERR_INTR,
										k, 0);
								}
							}
							else if (fifo_enqueue(dptr, RX_FIFO, k) == OK)
							{
								if (dptr->superH->rxok_intrenable_flag)
								{
									pic_intr_enqueue(dptr,
									dptr->superH->nicintrQ,
									NIC_RXOK_INTR, k, 0);
								}

								dptr->superH->NIC_IFCS[k].IFC_CNTR_RXOK++;
							}
						}
					}
				}
			}

			if (tptr->bcast)
			{
				tptr->bits_left -= 8;

				if (SF_DEBUG && (tptr->bits_left < 0))
				{
					mprint(NULL, siminfo,
						"tptr->bits_left is < 0 (on a bcast): Should ne'er happen");
					mexit("See above messages.", -1);
				}

				if (tptr->bits_left == 0)
				{
					seg_dequeue(curseg, tptr);
				}

				/*	Source power consumption	*/
				if (SF_POWER_ANALYSIS)
				{
					sptr->E.current_draw +=
						sptr->superH->NIC_IFCS[src_ifc].tx_pwr / sptr->VDD;
				}

				/*	Continue to next segbuf on netseg	*/
				continue;
			}

			/*									*/
			/*	 Was not bcast, therefore only other option is unicast.		*/
			/*	 *(maybe one day we'll implement anycast and multicast)*	*/
			/*	All interfaces of dst node attached to this segment get 	*/
			/*	the data. 							*/
			/*									*/
			for (k = 0; (dptr->runnable) && (k < dptr->superH->NIC_NUM_IFCS); k++)
			{
				if (dptr->superH->NIC_IFCS[k].segno == SIM_ACTIVE_NETSEGS[i])
				{
					int	idx = dptr->superH->NIC_IFCS[k].rx_fifo_curidx;
					int	offset = frame_bytes - (tptr->bits_left/8);

					/*							*/
					/*	Subscription to segment enforces framesize	*/
					/*	is integral multiple of bytes, and receive	*/
					/*	buf is resized for framesize.			*/
					/*							*/

					/*							*/
					/*	If we are assoc. w/a sigsrc, measure SNR	*/
					/*	and corrupt data if SNR is too low.		*/
					/*							*/
					if (curseg->sigsrc != NULL)
					{
						double snr = check_snr(curseg, sptr, dptr);
						if (snr < curseg->minsnr)
						{
							/*	SNR is too low. Destination gets noise	*/
							dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] =
								tptr->data[offset] & mrandom();
							mprint(NULL, siminfo,
							 "Data corrupted in xmit due to low SNR\n");
						}
						else
						{
							dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] =
								tptr->data[offset];
						}
					}
					else
					{
						dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] = tptr->data[offset];
					}

					/*							*/
					/*	Destination power consumption (source handled	*/
					/*	just once).					*/
					/*							*/
					if (SF_POWER_ANALYSIS)
					{
						dptr->E.current_draw +=
							dptr->superH->NIC_IFCS[k].rx_pwr / dptr->VDD;
					}

					/*	If about to become empty:	*/
					if (tptr->bits_left == 8)
					{
						char *ptr = dptr->superH->NIC_IFCS[k].rx_fifo[idx];

						if (checksum(ptr, frame_bytes) !=
							*((ulong *)&tptr->data[frame_bytes]))
						{
							dptr->superH->NIC_IFCS[k].IFC_CNTR_CSUM_ERR++;

							if (dptr->superH->csumerr_intrenable_flag)
							{
								pic_intr_enqueue(dptr,
									dptr->superH->nicintrQ,
									NIC_CSUMERR_INTR, k, 0);
							}
						}
						else if (fifo_enqueue(dptr, RX_FIFO, k) == OK)
						{
							if (dptr->superH->rxok_intrenable_flag)
							{
								pic_intr_enqueue(dptr,
								dptr->superH->nicintrQ,
								NIC_RXOK_INTR, k, 0);
							}

							dptr->superH->NIC_IFCS[k].IFC_CNTR_RXOK++;
						}
					}
				}
			}

			tptr->bits_left -= 8;

			if (SF_DEBUG && (tptr->bits_left < 0))
			{
				mprint(NULL, siminfo,
					"tptr->bits_left is < 0 (for a unicast): should ne'er happen");
				mexit("See above messages.", -1);
			}

			if (tptr->bits_left == 0)
			{
				seg_dequeue(curseg, tptr);
			}

			/*	Source power consumption	*/
			if (SF_POWER_ANALYSIS)
			{
				sptr->E.current_draw += sptr->superH->NIC_IFCS[src_ifc].tx_pwr / sptr->VDD;
			}
		}
		curseg->lastactivate = SIM_GLOBAL_TIME;
	}

	/*	Nodes -> Seg		*/
	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		State* nptr = SIM_STATE_PTRS[i];

		for (k = 0; k < nptr->superH->NIC_NUM_IFCS; k++)
		{
			if (nptr->superH->NIC_IFCS[k].IFC_TXFIFO_LEVEL > 0)
			{
				nptr->superH->NIC_IFCS[k].tx_fifo_retry_fxn(nptr, k);
			}
		}
	}


	return;
}

double
check_snr(Netsegment *curseg, State *src_node, State *dst_node)
{
	int		whichbuf, i;
	double		noise_signal = DBL_MIN, desired_signal = DBL_MIN;
	Signalsrc	*s;


	for (whichbuf = 0; whichbuf < curseg->cur_queue_width; whichbuf++)
	{
		Segbuf		*tptr = &curseg->segbufs[whichbuf];
		int		src_ifc = tptr->src_ifc;

		/*		For calc., place sigsrc at sender	*/
		curseg->sigsrc->xloc = ((State *)tptr->src_node)->xloc;
		curseg->sigsrc->yloc = ((State *)tptr->src_node)->yloc;
		curseg->sigsrc->zloc = ((State *)tptr->src_node)->zloc;

		/*	Strength of src is senders xmit power		*/
		curseg->sigsrc->sample = ((State *)tptr->src_node)->superH->NIC_IFCS[src_ifc].tx_pwr;

		if ((tptr->src_node == src_node) && ((tptr->dst_node == dst_node) || tptr->bcast))
		{
			desired_signal += physics_propagation(curseg->sigsrc,
				dst_node->xloc, dst_node->yloc, dst_node->zloc);
		}
		else
		{
			noise_signal += physics_propagation(curseg->sigsrc,
				dst_node->xloc, dst_node->yloc, dst_node->zloc);
		}
	}

	/*	Current seg shouldnt be added in what follows		*/
	curseg->sigsrc->sample = 0;

	/*	Add in all present signals of same type as curseg	*/
	for (i = 0; i < SIM_NUM_SIGSRCS; i++)
	{
		s = &SIM_SIGSRCS[i];

		if (s->nsamples <= 0 || s->type != curseg->sigsrc->type)
		{
			continue;
		}

		noise_signal += physics_propagation(s,
					dst_node->xloc, dst_node->yloc, dst_node->zloc);
	}

	return desired_signal/noise_signal;
}


ulong
checksum(uchar *data, int datalen)
{
	int	i;
	ulong	cksum = 0;

	
	for (i = 0; i < datalen; i++)
	{
		cksum += data[i];
	}

	return cksum;
}


int
lookup_id(uchar *addr)
{
	int 		i, j;
			
	if (!SF_NETWORK)
	{
		mprint(NULL, siminfo,
			"Networking disabled, and you want to do what !!! Exiting!\n");
		mexit("See above messages.", -1);
	}

	if (!strcmp(addr, "::1"))
	{
		/*								*/
		/*	I can't convince myself that this is not disgusting. 	*/
		/*	What am I coming to ?					*/
		/*								*/
		return -2;
	}

	for (i = 0; i < SIM_NUM_NODES; i++)
	{
		for (j = 0; j < SIM_STATE_PTRS[i]->superH->NIC_NUM_IFCS; j++)
		{
			if (!strcmp(addr, SIM_STATE_PTRS[i]->superH->NIC_IFCS[j].IFC_OUI))
			{
				return i;
			}
		}
	}

	return -1;
}

void
netsegcircbuf(Segbuf *segbuf)
{
	char	*buf;
	int	i, n = 0;


	buf = &SIM_NETIO_BUF[SIM_NETIO_H2O][0];	

	n += sprintf(&buf[n], "Timestamp: %E\n", segbuf->timestamp);

	n += sprintf(&buf[n], "Data: ");
	for (i = 0; i < segbuf->bits_left/8; i++)
	{
		n += sprintf(&buf[n], "%02X ", segbuf->data[i]);
		if (!((i+1) % 24))
		{
			n += sprintf(&buf[n], "\n");
		}
	}
	n += sprintf(&buf[n], ".\n");

	n += sprintf(&buf[n], "Bits left: 0x%08X\n", segbuf->bits_left);
	n += sprintf(&buf[n], "Src node: 0x%08X\n", ((State *)segbuf->src_node)->NODE_ID);

	if (segbuf->bcast)
	{
		n += sprintf(&buf[n], "Dst node: 0x%08X\n", -2);
	}
	else
	{
		n += sprintf(&buf[n], "Dst node: 0x%08X\n",
			((State *)segbuf->dst_node)->NODE_ID);
	}

	n += sprintf(&buf[n], "Bcast flag: 0x%08X\n", segbuf->bcast);
	n += sprintf(&buf[n], "Src ifc: 0x%08X\n", segbuf->src_ifc);
	n += sprintf(&buf[n], "Parent netseg ID: 0x%08X\n", segbuf->parent_netsegid);
	n += sprintf(&buf[n], "from_remote flag: 0x%08X\n", segbuf->from_remote);
	n += sprintf(&buf[n], "\n\n\n\n");

 	SIM_NETIO_H2O++;

	if (SIM_NETIO_H2O == MAX_NETIO_NBUFS)
	{
		memmove(SIM_NETIO_BUF[0], SIM_NETIO_BUF[1],
			(MAX_NETIO_NBUFS-1)*MAX_SEGBUF_TEXT);
		SIM_NETIO_H2O--;
	}


	return;
}

void
netsegdump(char *dumpname, Segbuf *segbuf)
{
	int	i, fd;
	char	buf[MAX_SEGBUF_TEXT];


	/*	Create if not there, append if there:	*/
	fd = mcreate(dumpname, M_OWRITE);
	if (fd < 0)
	{
		mprint(NULL, siminfo, 
			"Could not open destination \"%s\" for NETSEG2FILE\n",
			dumpname);

		return;
	}

	/*	The fd was created w/ mopen(), so must write w/ mprintfd	*/
	sprintf(buf, "Timestamp: %E\n", segbuf->timestamp);
	mprintfd(fd, buf);

	sprintf(buf, "Data: ");
	mprintfd(fd, buf);
	for (i = 0; i < segbuf->bits_left/8; i++)
	{
		sprintf(buf, "%02X ", segbuf->data[i]);
		mprintfd(fd, buf);
		if (!((i+1) % 24))
		{
			sprintf(buf, "\n");
			mprintfd(fd, buf);
		}
	}
	sprintf(buf, ".\n");

	mprintfd(fd, buf);
	sprintf(buf, "Bits left: 0x%08X\n", segbuf->bits_left);
	mprintfd(fd, buf);
	sprintf(buf, "Src node: 0x%08X\n", ((State *)segbuf->src_node)->NODE_ID);
	mprintfd(fd, buf);

	if (segbuf->bcast)
	{
		sprintf(buf, "Dst node: 0x%08X\n", -2);
		mprintfd(fd, buf);
	}
	else
	{
		sprintf(buf, "Dst node: 0x%08X\n", ((State *)segbuf->dst_node)->NODE_ID);
		mprintfd(fd, buf);
	}

	sprintf(buf, "Bcast flag: 0x%08X\n", segbuf->bcast);
	mprintfd(fd, buf);
	sprintf(buf, "Src ifc: 0x%08X\n", segbuf->src_ifc);
	mprintfd(fd, buf);
	sprintf(buf, "Parent netseg ID: 0x%08X\n", segbuf->parent_netsegid);
	mprintfd(fd, buf);
	sprintf(buf, "from_remote flag: 0x%08X\n", segbuf->from_remote);
	mprintfd(fd, buf);
	sprintf(buf, "\n\n\n\n");
	mprintfd(fd, buf);

	mclose(fd);


	return;
}

int
parsenetsegdump(char *buf, Segbuf *segbuf)
{
	int	i = 0, off = 0, tmp;


	sscanf(buf+off, "Timestamp: %le\n", &segbuf->timestamp);
	off += strlen("Timestamp: X.XXXXXXE-XX\n");
	sscanf(buf+off, "Data: ");
	off += strlen("Data: ");

	/*	MAXFRAMEBYTES for payload, + 4 for cksum	*/
	for ( ; buf[off] != '.' && i < MAX_FRAMEBYTES + 4; i++)
	{
		sscanf(buf+off, "%2X ", &tmp);
		segbuf->data[i] = (uchar)tmp;
		off += strlen("XX ");

		if (!((i+1) % 24))
		{
			off++;
		}
	}
	off += strlen(".\n");

	sscanf(buf+off, "Bits left: 0x%8X\n", &segbuf->bits_left);
	off += strlen("Bits left: 0xXXXXXXXX\n");

	sscanf(buf+off, "Src node: 0x%8X\n", &tmp);
	off += strlen("Src node: 0xXXXXXXXX\n");
	if (tmp < SIM_NUM_NODES && tmp >= 0)
	{
		segbuf->src_node = SIM_STATE_PTRS[tmp];
	}

	sscanf(buf+off, "Dst node: 0x%8X\n", &tmp);
	off += strlen("Dst node: 0xXXXXXXXX\n");

	if (tmp < SIM_NUM_NODES && tmp >= 0)
	{
		segbuf->dst_node = SIM_STATE_PTRS[tmp];
	}

	sscanf(buf+off, "Bcast flag: 0x%8X\n", &segbuf->bcast);
	off += strlen("Bcast flag: 0xXXXXXXXX\n");

	sscanf(buf+off, "Src ifc: 0x%8X\n", &segbuf->src_ifc);
	off += strlen("Src ifc: 0xXXXXXXXX\n");

	sscanf(buf+off, "Parent netseg ID: 0x%8X\n", &segbuf->parent_netsegid);
	off += strlen("Parent netseg ID: 0xXXXXXXXX\n");

	sscanf(buf+off, "from_remote flag: 0x%8X\n", &segbuf->from_remote);
	off += strlen("from_remote flag: 0xXXXXXXXX\n");

	//netsegdump("/tmp/parsenetsegdebug", segbuf);


	return off;
}

void
network_netseg2file(int which, char *filename)
{
	Netsegment	*tptr;


	if (which >= SIM_NUM_NET_SEGMENTS)
	{
		merror("Network segment, [%d] is out of range.", which);

		return;
	}

	tptr = &SIM_NET_SEGMENTS[which];
	tptr->seg2filenames[tptr->num_seg2files] = 
		mcalloc(strlen(filename)+1,
		sizeof(char),
		"mcalloc() for tptr->seg2filenames[n] in shasm.y");

	if (tptr->seg2filenames[tptr->num_seg2files] == NULL)
	{
		merror("Could not allocate memory for tptr->seg2filenames[].");
	}
	else
	{
		strcpy(tptr->seg2filenames[tptr->num_seg2files], filename);
		tptr->num_seg2files++;
	}

	return;
}


void
network_file2netseg(char *file, int whichseg)
{
	return;
}

void
network_netsegpropmodel(int whichseg, int whichmodel, double minsnr)
{
	Netsegment	*tptr;

	if (whichseg >= MAX_NETSEGMENTS)
	{
		merror("Invalid netseg ID [%d] specified in \"netsegpropmodel\" command.",
			whichseg);
		return;
	}
	else if (whichmodel >= MAX_SIGNAL_SRCS)
	{
		merror("Invalid sigsrc ID [%d] specified in \"netsegpropmodel\" command\n",
			whichmodel);
		return;
	}

	tptr = &SIM_NET_SEGMENTS[whichseg];
	tptr->sigsrc = &SIM_SIGSRCS[whichmodel];
	tptr->minsnr = minsnr;

	return;
}

void
network_netnewseg(int which, int framebits, int pgnspeed, int bitrate, int width, 
		int fpdist, double fpdistmu, double fpdistsigma, double fpdistlambda,
		int fddist, double fddistmu, double fddistsigma, double fddistlambda)
{
	Netsegment 		*tptr;


	if (which >= MAX_NETSEGMENTS)
	{
		merror("Segment # > max. number of network segments.");
		return;
	}

	if (framebits/8 > MAX_FRAMEBYTES)
	{
		merror("Segment frame size is larger than max allowed.");
		return;
	}

	if (!SIM_NET_SEGMENTS[which].valid)
	{
		which = SIM_NUM_NET_SEGMENTS++;

		if (which >= MAX_NETSEGMENTS)
		{
			merror("Maximum number of network segments reached.");
			return;
		}

		SIM_NET_SEGMENTS[which].valid = 1;
		SIM_ACTIVE_NETSEGS[SIM_NUM_ACTIVE_NETSEGS++] = which;
	}

	tptr = &SIM_NET_SEGMENTS[which];

	tptr->frame_bits = framebits;
	tptr->propagation_speed = pgnspeed;
	tptr->bitrate = bitrate;

	tptr->queue_max_width = width;
	if (tptr->queue_max_width == 0)
	{
		mprint(NULL, siminfo,
			"Queue width of 0 deprecated. Upgraded to width 1\n");
		tptr->queue_max_width = 1;
	}

	tptr->NETSEG_ID = which; 

	tptr->segbufs = (Segbuf *)mcalloc(tptr->queue_max_width, sizeof(Segbuf),
				"(Segbuf *)tptr->segbufs in shasm.y");
	if (tptr->segbufs == NULL)
	{
		merror("mcalloc failed for (Segbuf *)tptr->segbufs in shasm.y.");
		return;
	}

	tptr->failure_dist.distribution = fpdist;
	tptr->failure_dist.mu = fpdistmu;
	tptr->failure_dist.sigma = fpdistsigma;
	tptr->failure_dist.lambda = fpdistlambda;

	tptr->failure_duration_dist.distribution = fddist;
	tptr->failure_duration_dist.mu = fddistmu;
	tptr->failure_duration_dist.sigma = fddistsigma;
	tptr->failure_duration_dist.lambda = fddistlambda;

	fault_setnetsegpfun(tptr, "urnd");
	tptr->num_attached = 0;
	tptr->num_seg2files = 0;
	
	tptr->lastactivate = 0;
	tptr->bytedelay = 8.0/((double)tptr->bitrate);

	SIM_NETWORK_PERIOD = min(SIM_NETWORK_PERIOD, tptr->bytedelay);

	return;
}

void
network_netsegnicattach(State *S, int whichifc, int whichseg)
{
	int i;


	if (whichifc >= NIC_MAX_IFCS)
	{
		merror("IFC # > max. number of IFCs.");
		return;
	}

	if (whichseg >= MAX_NETSEGMENTS)
	{
		merror("Segment # > max. number of network segments.");
		return;
	}

	if (SIM_NET_SEGMENTS[whichseg].num_attached >= MAX_SEGNODES)
	{
		merror("Maximum number of node IFCs attached to segment.");
		return;
	}

	SIM_NET_SEGMENTS[whichseg].node_ids[SIM_NET_SEGMENTS[whichseg].num_attached++] =
		S->NODE_ID;

	S->superH->NIC_IFCS[whichifc].segno = whichseg;
 	S->superH->NIC_IFCS[whichifc].frame_bits = SIM_NET_SEGMENTS[whichseg].frame_bits;

	/*	NIC_BRR contains network speed in Kb/s		*/
	S->superH->NIC_IFCS[whichifc].IFC_BRR = SIM_NET_SEGMENTS[whichseg].bitrate/1024;


	S->superH->NIC_IFCS[whichifc].rx_fifo =
		(uchar **)mcalloc(S->superH->NIC_IFCS[whichifc].rx_fifo_size,
		sizeof(uchar *), "shasm.y, (uchar **) for T_NETSEGNICATTACH");

	if (S->superH->NIC_IFCS[whichifc].rx_fifo == NULL)
	{
		merror("Could not allocate memory for rx_fifo[].");
		return;
	}

	for (i = 0; i < S->superH->NIC_IFCS[whichifc].rx_fifo_size; i++)
	{
		S->superH->NIC_IFCS[whichifc].rx_fifo[i] =
			(uchar *)mcalloc(SIM_NET_SEGMENTS[whichseg].frame_bits/8,
			sizeof(uchar), "shasm.y, (uchar *) for T_NETSEGNICATTACH");

		if (S->superH->NIC_IFCS[whichifc].rx_fifo[i] == NULL)
		{
			merror("Could not allocate memory for rx_fifo entry.");
			return;
		}
	}


	S->superH->NIC_IFCS[whichifc].tx_fifo =
		(uchar **)mcalloc(S->superH->NIC_IFCS[whichifc].tx_fifo_size, sizeof(uchar *),
		"shasm.y, (uchar **) for T_NETSEGNICATTACH");
				
	if (S->superH->NIC_IFCS[whichifc].tx_fifo == NULL)
	{
		merror("Could not allocate memory for tx_fifo[].");
		return;
	}

	for (i = 0; i < S->superH->NIC_IFCS[whichifc].tx_fifo_size; i++)
	{
		S->superH->NIC_IFCS[whichifc].tx_fifo[i] =\
			(uchar *)mcalloc(SIM_NET_SEGMENTS[whichseg].frame_bits/8, sizeof(uchar),
			"shasm.y, (uchar *) for T_NETSEGNICATTACH");

		if (S->superH->NIC_IFCS[whichifc].tx_fifo[i] == NULL)
		{
			merror("Could not allocate memory for tx_fifo entry.");
			return;
		}
	}

	/*	Local buffer for RX only	*/
	S->superH->NIC_IFCS[whichifc].rx_localbuf =
		(uchar *)mcalloc(S->superH->NIC_IFCS[whichifc].frame_bits/8, sizeof(uchar),
		"shasm.y, (uchar *) for S->superH->NIC_IFCS[whichifc].rx_localbuf");
			
	if (S->superH->NIC_IFCS[whichifc].rx_localbuf == NULL)
	{
		merror("Could not allocate memory for rx_localbuf[].");
		return;
	}

	S->superH->NIC_IFCS[whichifc].valid = 1;
		

	return;	
}

void
network_netnodenewifc(
	State *S, int which, double txpwr, double rxpwr, double idlepwr,
	int fpdist, int fpdistmu, int fpdistsigma, int fpdistlambda,
	int txfifosz, int rxfifosz)
{
	if (which >= NIC_MAX_IFCS)
	{
		merror("Invalid node IFC number.");
		return;
	}

	if (!S->superH->NIC_IFCS[which].valid)
	{
		which = S->superH->NIC_NUM_IFCS++;
	}

	/*	Skip marking it valid and sizing bufs etc. 'till attach time	*/
	S->superH->NIC_IFCS[which].tx_pwr = txpwr;
	S->superH->NIC_IFCS[which].rx_pwr = rxpwr;
	S->superH->NIC_IFCS[which].idle_pwr = idlepwr;
	S->superH->NIC_IFCS[which].failure_dist.distribution = fpdist;
	S->superH->NIC_IFCS[which].failure_dist.mu = fpdistmu;
	S->superH->NIC_IFCS[which].failure_dist.sigma = fpdistsigma;
	S->superH->NIC_IFCS[which].failure_dist.lambda = fpdistlambda;
	S->superH->NIC_IFCS[which].tx_fifo_size = txfifosz;
	S->superH->NIC_IFCS[which].rx_fifo_size = rxfifosz;

	S->superH->NIC_IFCS[which].tx_fifo_curidx = 0;
	S->superH->NIC_IFCS[which].rx_fifo_curidx = 0;
	S->superH->NIC_IFCS[which].tx_fifo_oldestidx = 0;
	S->superH->NIC_IFCS[which].rx_fifo_oldestidx = 0;
	S->superH->NIC_IFCS[which].tx_fifo_h2o = 0;
	S->superH->NIC_IFCS[which].rx_localbuf_h2o = 0;
	S->superH->NIC_IFCS[which].tx_alg_retries = 0;
	S->superH->NIC_IFCS[which].tx_fifo_maxoccupancy = 0;
	S->superH->NIC_IFCS[which].rx_fifo_maxoccupancy = 0;

	/*	The default alg. is binary exp.		*/
	S->superH->NIC_IFCS[which].tx_fifo_retry_fxn = tx_retryalg_binexp;
	snprintf(&S->superH->NIC_IFCS[which].IFC_OUI[0], NIC_ADDR_LEN, "%d", S->NODE_ID);
		

	return;	
}

void
network_netsegdelete(int whichseg)
{
	int	i;

	if (whichseg >= MAX_NETSEGMENTS)
	{
		merror("Segment # (%d) > max. number of network segments.", whichseg);
		return;
	}

	SIM_NET_SEGMENTS[whichseg].valid = 0;

	for (i = 0; i < SIM_NUM_ACTIVE_NETSEGS; i++)
	{
		if (SIM_ACTIVE_NETSEGS[i] == whichseg)
		{
			SIM_ACTIVE_NETSEGS[i] = SIM_ACTIVE_NETSEGS[SIM_NUM_ACTIVE_NETSEGS-1];
			SIM_ACTIVE_NETSEGS[SIM_NUM_ACTIVE_NETSEGS-1] = 0;
			SIM_NUM_ACTIVE_NETSEGS--;
		}
	}

	return;
}


void
network_netdebug(State *S)
{
	Ifc	*ifcptr;
	int	i;


	mprint(S, nodeinfo, "\nNode has [%d] active interfaces\n",
		S->superH->NIC_NUM_IFCS);

	for (i = 0; i < S->superH->NIC_NUM_IFCS; i++)
	{
		ifcptr = &S->superH->NIC_IFCS[i];

		mprint(S, nodeinfo, "\t\tIFC [%d] \"NIC_OUI\" is [%s]\n",
			i, ifcptr->IFC_OUI);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"NIC_DST\" is [%s]\n",
			i, ifcptr->IFC_DST);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"valid\" is [%d]\n",
			i, ifcptr->valid);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"tx_pwr\" is [%E]\n",
			i, ifcptr->tx_pwr);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"rx_pwr\" is [%E]\n",
			i, ifcptr->rx_pwr);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"idle_pwr\" is [%E]\n",
			i, ifcptr->idle_pwr);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"fail_prob\" is [%E]\n",
			i, ifcptr->fail_prob);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"fail_clocks_left\" is [" UVLONGFMT "]\n",
			i, ifcptr->fail_clocks_left);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"frame_bits\" is [%d]\n",
			i, ifcptr->frame_bits);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"segno\" is [%d]\n",
			i, ifcptr->segno);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"rx_localbuf_h2o\" is [%d]\n",
			i, ifcptr->rx_localbuf_h2o);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_size\" is [%d]\n",
			i, ifcptr->tx_fifo_size);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"rx_fifo_size\" is [%d]\n",
			i, ifcptr->rx_fifo_size);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_curidx\" is [%ud]\n",
			i, ifcptr->tx_fifo_curidx);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"rx_fifo_curidx\" is [%ud]\n",
			i, ifcptr->rx_fifo_curidx);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_oldestidx\" is [%ud]\n",
			i, ifcptr->tx_fifo_oldestidx);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"rx_fifo_oldestidx\" is [%ud]\n",
			i, ifcptr->rx_fifo_oldestidx);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_h2o\" is [%ud]\n",
			i, ifcptr->tx_fifo_h2o);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_maxoccupancy\" is [%ud]\n",
			i, ifcptr->tx_fifo_maxoccupancy);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"rx_fifo_maxoccupancy\" is [%ud]\n",
			i, ifcptr->rx_fifo_maxoccupancy);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"tx_alg_retries\" is [%ud]\n",
			i, ifcptr->tx_alg_retries);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"fifo_nextretry_time\" is [%E]\n",
			i, ifcptr->fifo_nextretry_time);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_FSZ\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_FSZ);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_RDR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_RDR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_NSR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_NSR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_TDR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_TDR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_NCR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_NCR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_BRR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_BRR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_NMR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_NMR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_TXOK\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_TXOK);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_RXOK\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_RXOK);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_ADDR_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_ADDR_ERR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_FRAME_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_FRAME_ERR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_COLL_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_COLLS_ERR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_CSENSE_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_CSENSE_ERR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_RXOVRRUN_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_RXOVRRUN_ERR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_RXUNDRRUN_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_RXUNDRRUN_ERR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_TXOVRRUN_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_TXOVRRUN_ERR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_TXUNDRRUN_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_TXUNDRRUN_ERR);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_TXFIFO_LEVEL\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_TXFIFO_LEVEL);
		mprint(S, nodeinfo, "\t\tIFC [%d] \"IFC_RXFIFO_LEVEL\" is [" ULONGFMT "]\n\n",
			i, ifcptr->IFC_RXFIFO_LEVEL);
	}

	mprint(S, nodeinfo, "\nSIM_GLOBAL_TIME = %E\n", SIM_GLOBAL_TIME);

	mprint(S, nodeinfo,
		"Total of " UVLONGFMT " bytes sent in simulation (all nodes)\n\n",
		SIM_NIC_BYTES_SENT);
		

	return;	
}

void
network_setretryalg(State *S, int which, char *alg)
{
	if (!strcmp(alg, "asap"))
	{
		S->superH->NIC_IFCS[which].tx_fifo_retry_fxn = tx_retryalg_asap;
	}
	else if (!strcmp(alg, "random"))
	{
		S->superH->NIC_IFCS[which].tx_fifo_retry_fxn = tx_retryalg_random;
	}
	else if (!strcmp(alg, "binexp"))
	{
		S->superH->NIC_IFCS[which].tx_fifo_retry_fxn = tx_retryalg_binexp;
	}
	else
	{
		merror("Invalid Retry Algorithm supplied\n");
	}

	return;
}

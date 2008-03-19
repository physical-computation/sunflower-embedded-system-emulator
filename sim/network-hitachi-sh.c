/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)
 
	All rights reserved.

	Redistribution and use in source and binary forms, with or without 
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written 
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "sf.h"
#include "mextern.h"

static int		lookup_id(Engine *, uchar*);
static int		seg_enqueue(Engine *E, State *S, int whichifc);
static uchar *		fifo_dequeue(Engine *, State *S, Fifo fifo_name, int whichifc);
static void		netsegcircbuf(Engine *, Segbuf *segbuf);
static void		seg_dequeue(Netsegment *curseg, int whichbuf);

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
fifo_enqueue(Engine *E, State *S, Fifo which_fifo, int whichifc)
{
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if (which_fifo == TX_FIFO)
	{
		if (ifcptr->tx_fifo_size <= 0)
		{
			mexit(E, "Invalid TX FIFO size", -1);
		}

		if (ifcptr->IFC_TXFIFO_LEVEL == ifcptr->tx_fifo_size)
		{
			ifcptr->IFC_CNTR_TXOVRRUN_ERR++;
			ifcptr->IFC_NSR |= (1 << 0);
	
			if (S->superH->txovrrunerr_intrenable_flag)
			{
				pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_TXOVRRUNERR_INTR, whichifc, 0);
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
			mexit(E, "Invalid TX FIFO size", -1);
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
			pic_intr_clear(E, S, S->superH->nicintrQ, NIC_RXOK_INTR, 0 /* clear just one intr */);

			/*	RX err intrs if enbld.	*/
			if (S->superH->rxovrrunerr_intrenable_flag)
			{
				pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_RXOVRRUNERR_INTR, whichifc, 0);
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
fifo_dequeue(Engine *E, State *S, Fifo which_fifo, int whichifc)
{
	uchar	*ret_ptr;
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if (which_fifo == TX_FIFO)
	{
		if (ifcptr->tx_fifo_size <= 0)
		{
			mexit(E, "Invalid TX FIFO size", -1);
		}

		if (ifcptr->IFC_TXFIFO_LEVEL == 0)
		{
			ifcptr->IFC_CNTR_TXUNDRRUN_ERR++;
			ifcptr->IFC_NSR |= (1 << 0);
	
			if (S->superH->txundrrunerr_intrenable_flag)
			{
				pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_TXUNDRRUNERR_INTR, whichifc, 0);
			}

			/*	No soup for you		*/
			sfatal(E, S, "fifo_dequeue() returning NULL (TX UNDERRRUN)");
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
			mexit(E, "Invalid TX FIFO size", -1);
		}

		if (ifcptr->IFC_RXFIFO_LEVEL == 0)
		{
			ifcptr->IFC_CNTR_RXUNDRRUN_ERR++;
			ifcptr->IFC_NSR |= (1 << 0);
	
			if (S->superH->rxundrrunerr_intrenable_flag)
			{
				pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_RXUNDRRUNERR_INTR, whichifc, 0);
			}

			/*	No soup for you		*/
			sfatal(E, S, "fifo_dequeue() returning NULL (RX UNDERRUN)");
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
seg_enqueue(Engine *E, State *S, int whichifc)
{
	int			actual_framesize, i, si, di;
	Ifc			*ifcptr = &S->superH->NIC_IFCS[whichifc];
	Netsegment		*Seg = &E->netsegs[ifcptr->segno];
	uchar			*tptr;
	int			curwidth = Seg->cur_queue_width;
	char			srcstr[16], dststr[16];


	/*										*/
	/*	TODO: if we move from using the node ID as the identical OUI,		*/
	/*	must change the line below to do lookup_id() to get the si		*/
	/*										*/
	/*	*	The dest node should be determined from the contents 		*/
	/*	of the frame that we get from fifo_dequeue (below) rather than		*/
	/*	from  the node's IFC_DST. The whole IFC_DST business should be 		*/
	/*	gotten rid of, like we did for curifc					*/
	/*										*/



	si = S->NODE_ID;
	di = lookup_id(E, S->superH->NIC_IFCS[whichifc].IFC_DST);
	
	msnprint(srcstr, sizeof(srcstr), "%d/%d", si, whichifc);
	msnprint(dststr, sizeof(dststr), "%d", di);
	if (di == -2)
	{
		msnprint(dststr, sizeof(dststr), "(broadcast)");
	}
	if (si == -3)
	{
		msnprint(srcstr, sizeof(srcstr), "(remote)/%d", whichifc);
	}
	
	mprint(E, S, nodeinfo,
		"%d byte frame from %s --> %s, %Es since epoch\n",
		ifcptr->tx_fifo_framesizes[ifcptr->tx_fifo_oldestidx],
		srcstr, dststr, E->globaltimepsec);

	if (!Seg->valid)
	{
		mprint(E, S, nodeinfo, "Carrier sense error\n");

		S->superH->NIC_IFCS[whichifc].IFC_CNTR_CSENSE_ERR++;
		S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);
		if (S->superH->csenseerr_intrenable_flag)
		{
			pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_CSENSEERR_INTR, whichifc, 0);
		}

		return Ecsense;
	}

	/*										*/
	/*	TODO: if we want to have a collision defined as two nodes attempt	*/
	/*	to transmit at _exactly_ same time, then we should check below and	*/
	/*	signal a collision only if the Seg->bits_left equals the frame_bits	*/
	/*										*/

	if (Seg->busy)
	{
		mprint(E, S, nodeinfo,
			"Channel busy... (bandwidth = %d, occupancy = %d)\n",
			Seg->queue_max_width, Seg->cur_queue_width);

		S->superH->NIC_IFCS[whichifc].IFC_CNTR_COLLS_ERR++;
		S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);
		if (S->superH->collserr_intrenable_flag)
		{
			pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_COLLSERR_INTR, whichifc, 0);
		}

		return Ecoll;
	}

	if (ifcptr->frame_bits != Seg->frame_bits)
	{
		mprint(E, S, nodeinfo, "Frame error\n");
		
		S->superH->NIC_IFCS[whichifc].IFC_CNTR_FRAME_ERR++;
		S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);
		if (S->superH->frameerr_intrenable_flag)
		{
			pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_FRAMEERR_INTR, whichifc, 0);
		}

		return Eframeerr;
	}

	if ((si == -1) || (di == -1))
	{
		mprint(E, S, nodeinfo,
			"lookup() on either frame source or destination failed\n");
		mprint(E, S, nodeinfo, "Frame MAC layer address error\n");

		S->superH->NIC_IFCS[whichifc].IFC_CNTR_ADDR_ERR++;
		S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);	
		if (S->superH->addrerr_intrenable_flag)
		{
			pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_ADDRERR_INTR, whichifc, 0);
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
			mprint(E, S, nodeinfo, "Destination not on same segment (segment %d)\n",\
				S->superH->NIC_IFCS[whichifc].segno);
			mprint(E, S, nodeinfo, "Frame MAC layer address error\n");

			S->superH->NIC_IFCS[whichifc].IFC_CNTR_ADDR_ERR++;
			S->superH->NIC_IFCS[whichifc].IFC_NSR |= (1 << 0);
			if (S->superH->addrerr_intrenable_flag)
			{
				pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_ADDRERR_INTR, whichifc, 0);
			}

			return Eaddrerr;
		}
	}
	
	/*								*/
	/*	fifo_dequeue() below will be taking the oldest fifo	*/
	/*	entry, so that is the one whose size we want to get	*/
	/*								*/
	actual_framesize = ifcptr->tx_fifo_framesizes[ifcptr->tx_fifo_oldestidx];

	tptr = fifo_dequeue(E, S, TX_FIFO, whichifc);
	if (tptr == NULL)
	{
		/*	Node interrupt is raised in fifo_dequeue()	*/
		return Etxunderrun;
	}

	memmove(Seg->segbufs[curwidth].data, tptr, ifcptr->frame_bits/8);

	Seg->segbufs[curwidth].timestamp = E->globaltimepsec;
	Seg->segbufs[curwidth].bits_left = actual_framesize * 8;
	Seg->segbufs[curwidth].actual_nbytes = actual_framesize;
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
		Seg->segbufs[curwidth].dst_node = E->sp[di];
		Seg->segbufs[curwidth].bcast = 0;
	}

	/*	If netseg is bound to a file, o/p Seg->head	*/
	if (Seg->num_seg2files > 0)
	{
		int i;

		for (i = 0; i < Seg->num_seg2files; i++)
		{
			netsegdump(E, Seg->seg2filenames[i], &Seg->segbufs[curwidth]);
		}
	}

	if (!S->from_remote)
	{
		netsegcircbuf(E, &Seg->segbufs[curwidth]);
	}

	S->superH->NIC_IFCS[whichifc].IFC_NSR &= ~(1 << 0);
	E->nicsimbytes += S->superH->NIC_IFCS[whichifc].frame_bits/8;


	return OK;
}

void
seg_dequeue(Netsegment *curseg, int whichbuf)
{
	memmove(&curseg->segbufs[whichbuf],
		&curseg->segbufs[curseg->cur_queue_width - 1],
		sizeof(Segbuf));

	curseg->cur_queue_width--;
	if (curseg->cur_queue_width < curseg->queue_max_width)
	{
		curseg->busy = 0;
	}

	return;
}


void
remote_seg_enqueue(Engine *E, Segbuf *segbuf)
{
	State	*S = (State *)segbuf->src_node;
	int	whichifc = segbuf->src_ifc;
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];


	S->from_remote = 1;

	/*	dst_node will be NULL if it was a bcast		*/
	if (segbuf->dst_node != NULL)
	{
		msnprint((char *)ifcptr->IFC_DST, NIC_ADDR_LEN, "%d",
			((State *)segbuf->dst_node)->NODE_ID);
	}
	else
	{
		msnprint((char *)ifcptr->IFC_DST, NIC_ADDR_LEN, "::1");
	}

	memmove(ifcptr->tx_fifo[ifcptr->tx_fifo_curidx], segbuf->data, segbuf->bits_left/8);
	ifcptr->tx_fifo_framesizes[ifcptr->tx_fifo_curidx] = segbuf->actual_nbytes;
	fifo_enqueue(E, S, TX_FIFO, whichifc);


	return;
}

void
nic_tx_enqueue(Engine *E, State *S, uchar data, int whichifc)
{
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if (!SF_NETWORK)
	{
		mprint(E, S, nodeinfo,
			"Networking disabled, and you want to do what !!! Exiting !\n");
		mexit(E, "See above messages.", -1);
	}

	if (ifcptr->tx_fifo == NULL || ifcptr->tx_fifo_size <= 0)
	{
		sfatal(E, S, 
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
		ifcptr->tx_fifo_framesizes[ifcptr->tx_fifo_curidx] = ifcptr->tx_fifo_h2o;
		fifo_enqueue(E, S, TX_FIFO, whichifc);
		ifcptr->tx_fifo_h2o = 0;
	}

	return;
}


uchar
nic_rx_dequeue(Engine *E, State *S, int whichifc)
{
	uchar	retchar = 0;
	Ifc	*ifcptr = &S->superH->NIC_IFCS[whichifc];
	int	framesize = ifcptr->rx_localbuf_framesize;


	if (!SF_NETWORK)
	{
		mprint(E, S, nodeinfo,
			"Networking disabled, and you want to do what !!! Exiting !\n");
		mexit(E, "See above messages.", -1);
	}

	if (!ifcptr->rx_fifo)
	{
		sfatal(E, S,
			"Node is not equipped with an IFC.\nWhat do you mean you want to receive ?!");
	}

	/*									*/
	/*	The structure of the receive action is slightly assymetric	*/
	/*	from that for transmit. For receive, we grab an entry from 	*/
	/*	the FIFO into local buffer whenever local buffer is empty.	*/
	/*									*/
	if (ifcptr->rx_localbuf_h2o == 0)
	{
		uchar *tptr;
	
		/*								*/
		/*	fifo_dequeue will be taking the item at oldestidx	*/
		/*								*/
		framesize = ifcptr->rx_localbuf_framesize = ifcptr->rx_fifo_framesizes[ifcptr->rx_fifo_oldestidx];

		tptr = fifo_dequeue(E, S, RX_FIFO, whichifc);

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
tx_retryalg_binexp(void *e, void *x, int whichifc)
{
	Engine	*E = (Engine *)e;

	/*								*/
	/*	This routine is called by network_clock(), and it	*/
	/*	provides the intelligent network interface: It will	*/
	/*	try to move data out of the IFC's FIFO onto a netseg.	*/
	/*								*/

	State		*S = (State *)x;
	double		timeslot, range, delay;
	Ifc		*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if ((ifcptr->IFC_TXFIFO_LEVEL == 0) ||
		(ifcptr->fifo_nextretry_time > E->globaltimepsec))
	{
		return;
	}

	/*								*/
	/*	Add in the cost of trying to grab network: 		*/
	/*	tx_pwr watts for 1 clock cycle				*/
	/*								*/
	S->energyinfo.current_draw += S->superH->NIC_IFCS[whichifc].tx_pwr / S->VDD;

	/*	The seg_enqueue() failed, so calc. a new retry time	*/
	if (seg_enqueue(E, S, whichifc) != OK)
	{
		timeslot = (double)ifcptr->frame_bits/
					(double)E->netsegs[ifcptr->segno].bitrate;
		range = timeslot * (pow(2, min(ifcptr->tx_alg_retries, 10)) + 1);
		delay = fmod((double)mrandom(E), range);

		mprint(E, S, nodeinfo,
			"Binary exponential backoff for %E seconds, node %d ifc %d\n",
			delay, S->NODE_ID, whichifc);

		ifcptr->fifo_nextretry_time = E->globaltimepsec + delay;
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
			pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_TXOK_INTR, whichifc, 0);
		}

		ifcptr->tx_alg_retries = 0;
	}

	return;
}

void
tx_retryalg_random(void *e, void *x, int whichifc)
{
	Engine	*E = (Engine *)e;

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
		(ifcptr->fifo_nextretry_time > E->globaltimepsec))
	{
		return;
	}

	/*								*/
	/*	Add in the cost of trying to grab network: 		*/
	/*	tx_pwr watts for 1 clock cycle				*/
	/*								*/
	S->energyinfo.current_draw += S->superH->NIC_IFCS[whichifc].tx_pwr / S->VDD;

	if (seg_enqueue(E, S, whichifc) != OK)
	{
		timeslot = (double)ifcptr->frame_bits/
					(double)E->netsegs[ifcptr->segno].bitrate;
		range = timeslot*(MAXRANDOMSLOTS + 1);
		delay = fmod((double)mrandom(E), range);

		mprint(E, S, nodeinfo,
			"Random backoff for %E seconds (range=%E, max=%d slots), node %d ifc %d\n",
			delay, range, MAXRANDOMSLOTS, S->NODE_ID, whichifc);

		ifcptr->fifo_nextretry_time = E->globaltimepsec + delay;
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
			pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_TXOK_INTR, whichifc, 0);
		}

		ifcptr->tx_alg_retries = 0;
	}

	return;
}


void
tx_retryalg_linear(void *e, void *x, int whichifc)
{
	USED(e);
	USED(x)
	USED(whichifc);
}

void
tx_retryalg_asap(void *e, void *x, int whichifc)
{
	Engine	*E = (Engine *)e;


	/*								*/
	/*	This routine is called by network_clock(), and it	*/
	/*	provides the intelligent network interface: It will	*/
	/*	try to move data out of the IFC's FIFO onto a netseg.	*/
	/*								*/

	State		*S = (State *)x;
	double		timeslot;
	Ifc		*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if ((ifcptr->IFC_TXFIFO_LEVEL == 0) ||
		(ifcptr->fifo_nextretry_time > E->globaltimepsec))
	{
		return;
	}

	/*								*/
	/*	Add in the cost of trying to grab network: 		*/
	/*	tx_pwr watts for 1 clock cycle				*/
	/*								*/
	S->energyinfo.current_draw += S->superH->NIC_IFCS[whichifc].tx_pwr / S->VDD;

	if (seg_enqueue(E, S, whichifc) != OK)
	{
		timeslot = (double)ifcptr->frame_bits/
					(double)E->netsegs[ifcptr->segno].bitrate;

		mprint(E, S, nodeinfo,
			"Asap backoff for %E seconds (1 timeslot), node %d ifc %d\n",
			timeslot, S->NODE_ID, whichifc);

		ifcptr->fifo_nextretry_time = E->globaltimepsec + timeslot;
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
			pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_TXOK_INTR, whichifc, 0);
		}

		ifcptr->tx_alg_retries = 0;
	}

	return;
}

void
tx_retryalg_none(void *e, void *x, int whichifc)
{
	Engine	*E = (Engine *)e;

	/*								*/
	/*	This routine is called by network_clock(), and will	*/
	/*	try to move data out of the IFC's FIFO onto a netseg.	*/
	/*								*/
	State		*S = (State *)x;
	Ifc		*ifcptr = &S->superH->NIC_IFCS[whichifc];


	if ((ifcptr->IFC_TXFIFO_LEVEL == 0) ||
		(ifcptr->fifo_nextretry_time > E->globaltimepsec))
	{
		return;
	}

	/*								*/
	/*	Add in the cost of trying to grab network: 		*/
	/*	tx_pwr watts for 1 clock cycle				*/
	/*								*/
	S->energyinfo.current_draw += S->superH->NIC_IFCS[whichifc].tx_pwr / S->VDD;

	if (seg_enqueue(E, S, whichifc) == OK)
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
			pic_intr_enqueue(E, S, S->superH->nicintrQ, NIC_TXOK_INTR, whichifc, 0);
		}
	}
	else
	{
		/*							*/
		/*	Grabbing the medium failed for some reason	*/
		/*	for retryalg_none, we just junk the FIFO	*/
		/*	entry, no retries for sending it onto medium	*/
		/*							*/
		fifo_dequeue(E, S, TX_FIFO, whichifc);
	}

	ifcptr->tx_alg_retries = 0;


	return;
}

void
network_clock(Engine *E)
{
	int 			whichbuf, i, j, k;
	Netsegment		*curseg;


	for (i = 0; i < E->nactivensegs; i++)
	{
		curseg = &E->netsegs[E->activensegs[i]];
		if (( curseg->cur_queue_width == 0) ||
			!eventready(E->globaltimepsec, curseg->lastactivate, curseg->bytedelay))
		{
			continue;
		}

		for (whichbuf = 0; whichbuf < curseg->cur_queue_width; whichbuf++)
		{
			Segbuf		*tptr = &curseg->segbufs[whichbuf];
			State*		dptr = (State *)tptr->dst_node;
			State*		sptr = (State *)tptr->src_node;
			int		frame_bytes = tptr->actual_nbytes;
			int		src_ifc = tptr->src_ifc;


			/*	Timestamp might be in future if spliced from remote host.	*/
			if (tptr->bits_left == 0 || tptr->timestamp > E->globaltimepsec)
			{
				continue;
			}

			/*									*/
			/*	Seg -> Nodes: Broadcast. Nodes do not receive a copy of		*/
			/*	their own data on a broadcast.					*/
			/*									*/
			if (tptr->bcast) for (j = 0; j < curseg->num_attached; j++)
			{
				dptr = E->sp[curseg->node_ids[j]];
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
					if (	(dptr->superH->NIC_IFCS[k].segno == E->activensegs[i]) &&
						(dptr->superH->NIC_IFCS[k].IFC_STATE & NIC_STATE_LISTEN))
					{
						int	offset = frame_bytes - (tptr->bits_left/8);
						int	idx = dptr->superH->NIC_IFCS[k].rx_fifo_curidx;

					
						/*	Gets reset for all recipients when we dequeue segbuf	*/
						dptr->superH->NIC_IFCS[k].IFC_STATE |= NIC_STATE_RX;


						/*	If we have an associate physical model		*/
						if (curseg->sigsrc != NULL)
						{
							double snr = check_snr(E, curseg, sptr, dptr);

							if (snr > curseg->minsnr)
							{
								dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] =
									tptr->data[offset];
							}
							else if (snr == curseg->minsnr)
							{
								/*	SNR too low. Dest. gets noise	*/
								dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] =
									tptr->data[offset] & mrandom(E);
							}
							else
							{
								/*	when snr too low, dst gets nothing	*/
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
							dptr->energyinfo.current_draw +=
								dptr->superH->NIC_IFCS[k].rx_pwr /
								dptr->VDD;
						}

						/*	If about to become empty:	*/
						if (tptr->bits_left == 8)
						{
							dptr->superH->NIC_IFCS[k].rx_fifo_framesizes[idx] = tptr->actual_nbytes;

							/*	We trigger the dst to eat if minsnr exceeds at point of end of frame */
							if (curseg->sigsrc != NULL)
							{
								if (check_snr(E, curseg, sptr, dptr) > curseg->minsnr)
								{
									fifo_enqueue(E, dptr, RX_FIFO, k);

									if (dptr->superH->rxok_intrenable_flag)
									{
										pic_intr_enqueue(E, dptr,
											dptr->superH->nicintrQ,
											NIC_RXOK_INTR, k, 0);
									}

									dptr->superH->NIC_IFCS[k].IFC_CNTR_RXOK++;
								}
								else
								{
									/* snr too low, print debug warning if want to show these... */
								}
							}
							else
							{
								fifo_enqueue(E, dptr, RX_FIFO, k);

								if (dptr->superH->rxok_intrenable_flag)
								{
									pic_intr_enqueue(E, dptr,
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
					mprint(E, NULL, siminfo,
						"tptr->bits_left is < 0 (on a bcast): Should ne'er happen");
					mexit(E, "See above messages.", -1);
				}

				if (tptr->bits_left == 0)
				{
					/*							*/
					/*	Clear the RX bit in state of all recepients on	*/
					/*	this netseg.  There may be other buffers still	*/
					/*	emptying out, but that is Ok since we keep 	*/
					/*	setting the RX flag in the state repeatedly	*/
					/*							*/
					for (j = 0; j < curseg->num_attached; j++)
					{
						dptr = E->sp[curseg->node_ids[j]];
						if (!dptr->runnable || (dptr == tptr->src_node))
						{
							continue;
						}

						// BUG: why k ?! we should be looping over all the IFCs (with index k)
						dptr->superH->NIC_IFCS[k].IFC_STATE &= ~NIC_STATE_RX;
					}

					seg_dequeue(curseg, whichbuf);
				}

				/*	Source power consumption	*/
				if (SF_POWER_ANALYSIS)
				{
					sptr->energyinfo.current_draw +=
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
				if ((dptr->superH->NIC_IFCS[k].segno == E->activensegs[i]) &&
						(dptr->superH->NIC_IFCS[k].IFC_STATE & NIC_STATE_LISTEN))
				{
					int	idx = dptr->superH->NIC_IFCS[k].rx_fifo_curidx;
					int	offset = frame_bytes - (tptr->bits_left/8);


					/*	Gets reset for all recipients when we dequeue segbuf	*/
					dptr->superH->NIC_IFCS[k].IFC_STATE |= NIC_STATE_RX;


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
						double snr = check_snr(E, curseg, sptr, dptr);

						if (snr > curseg->minsnr)
						{
							dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] =
								tptr->data[offset];
						}
						else if (snr == curseg->minsnr)
						{
							/*	SNR at brink. Destination gets noise	*/
							dptr->superH->NIC_IFCS[k].rx_fifo[idx][offset] =
								tptr->data[offset] & mrandom(E);
						}
						else
						{
							/*	when snr is too low, dst gets nothing	*/
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
						dptr->energyinfo.current_draw +=
							dptr->superH->NIC_IFCS[k].rx_pwr / dptr->VDD;
					}

					/*	If about to become empty:	*/
					if (tptr->bits_left == 8)
					{
						dptr->superH->NIC_IFCS[k].rx_fifo_framesizes[idx] = tptr->actual_nbytes;

						if (curseg->sigsrc != NULL)
						{
							if (check_snr(E, curseg, sptr, dptr) > curseg->minsnr)
							{
								fifo_enqueue(E, dptr, RX_FIFO, k);

								if (dptr->superH->rxok_intrenable_flag)
								{
									pic_intr_enqueue(E, dptr,
									dptr->superH->nicintrQ,
									NIC_RXOK_INTR, k, 0);
								}

								dptr->superH->NIC_IFCS[k].IFC_CNTR_RXOK++;
							}
							else
							{	
								// snr too low, print debug warning if want to show these...
							}
						}
						else
						{
							fifo_enqueue(E, dptr, RX_FIFO, k);

							if (dptr->superH->rxok_intrenable_flag)
							{
								pic_intr_enqueue(E, dptr,
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
				mprint(E, NULL, siminfo,
					"tptr->bits_left is < 0 (for a unicast): should ne'er happen");
				mexit(E, "See above messages.", -1);
			}

			if (tptr->bits_left == 0)
			{
				/*							*/
				/*	Clear the RX bit in state of all recepients on	*/
				/*	this netseg.  There may be other buffers still	*/
				/*	emptying out, but that is Ok since we keep 	*/
				/*	setting the RX flag in the state repeatedly	*/
				/*							*/
				dptr->superH->NIC_IFCS[k].IFC_STATE &= ~NIC_STATE_RX;

				seg_dequeue(curseg, whichbuf);
			}

			/*	Source power consumption	*/
			if (SF_POWER_ANALYSIS)
			{
				sptr->energyinfo.current_draw += sptr->superH->NIC_IFCS[src_ifc].tx_pwr / sptr->VDD;
			}
		}
		curseg->lastactivate = E->globaltimepsec;
	}

	/*								*/
	/*	Handle Nodes -> Seg transmission, and also handle	*/
	/*	LISTEN and IDLE power.					*/
	/*								*/
	for (i = 0; i < E->nnodes; i++)
	{
		State* nptr = E->sp[i];

		for (k = 0; k < nptr->superH->NIC_NUM_IFCS; k++)
		{
			if (nptr->superH->NIC_IFCS[k].IFC_TXFIFO_LEVEL > 0)
			{
				nptr->superH->NIC_IFCS[k].tx_fifo_retry_fxn(E, nptr, k);
			}

			if (SF_POWER_ANALYSIS)
			{
				if (nptr->superH->NIC_IFCS[k].IFC_STATE == NIC_STATE_IDLE)
				{
					nptr->energyinfo.current_draw +=
						nptr->superH->NIC_IFCS[k].idle_pwr / nptr->VDD;
				}
				else if (nptr->superH->NIC_IFCS[k].IFC_STATE == NIC_STATE_LISTEN)
				{
					nptr->energyinfo.current_draw +=
						nptr->superH->NIC_IFCS[k].listen_pwr / nptr->VDD;
				}
			}
		}
	}


	return;
}

double
check_snr(Engine *E, Netsegment *curseg, State *src_node, State *dst_node)
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
	for (i = 0; i < E->nsigsrcs; i++)
	{
		s = &E->sigsrcs[i];

		/*	NOTE: for fixedsamples, nsamples will be 1	*/
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
lookup_id(Engine *E, uchar *addr)
{
	int 		i, j;
			
	if (!SF_NETWORK)
	{
		mprint(E, NULL, siminfo,
			"Networking disabled, and you want to do what !!! Exiting!\n");
		mexit(E, "See above messages.", -1);
	}

	if (!strcmp((char*)addr, "::1") || (addr[0] == 255))
	{
		/*								*/
		/*	I can't convince myself that this is not disgusting. 	*/
		/*	What am I coming to ?					*/
		/*								*/
		return -2;
	}

	for (i = 0; i < E->nnodes; i++)
	{
		for (j = 0; j < E->sp[i]->superH->NIC_NUM_IFCS; j++)
		{
			if (!strcmp((char *)addr, (char *)E->sp[i]->superH->NIC_IFCS[j].IFC_OUI))
			{
				return i;
			}
		}
	}

	return -1;
}

void
netsegcircbuf(Engine *E, Segbuf *segbuf)
{
	char	*buf;
	int	bufsz, i, n = 0;


	buf = &E->netiobuf[E->netioh2o][0];
	bufsz = sizeof(E->netiobuf[0]);

	n += msnprint(&buf[n], bufsz, "Timestamp: %E\n", segbuf->timestamp);

	n += msnprint(&buf[n], bufsz, "Data: ");
	for (i = 0; i < segbuf->bits_left/8; i++)
	{
		n += msnprint(&buf[n], bufsz, "%02X ", segbuf->data[i]);
		if (!((i+1) % 24))
		{
			n += msnprint(&buf[n], bufsz, "\n");
		}
	}
	n += msnprint(&buf[n], bufsz, ".\n");

	n += msnprint(&buf[n], bufsz, "Bits left: 0x%08X\n", segbuf->bits_left);
	n += msnprint(&buf[n], bufsz, "Src node: 0x%08X\n", ((State *)segbuf->src_node)->NODE_ID);

	if (segbuf->bcast)
	{
		n += msnprint(&buf[n], bufsz, "Dst node: 0x%08X\n", -2);
	}
	else
	{
		n += msnprint(&buf[n], bufsz, "Dst node: 0x%08X\n",
			((State *)segbuf->dst_node)->NODE_ID);
	}

	n += msnprint(&buf[n], bufsz, "Bcast flag: 0x%08X\n", segbuf->bcast);
	n += msnprint(&buf[n], bufsz, "Src ifc: 0x%08X\n", segbuf->src_ifc);
	n += msnprint(&buf[n], bufsz, "Parent netseg ID: 0x%08X\n", segbuf->parent_netsegid);
	n += msnprint(&buf[n], bufsz, "from_remote flag: 0x%08X\n", segbuf->from_remote);
	n += msnprint(&buf[n], bufsz, "\n\n\n\n");

 	E->netioh2o++;

	if (E->netioh2o == MAX_NETIO_NBUFS)
	{
		memmove(E->netiobuf[0], E->netiobuf[1],
			(MAX_NETIO_NBUFS-1)*MAX_SEGBUF_TEXT);
		E->netioh2o--;
	}


	return;
}

void
netsegdump(Engine *E, char *dumpname, Segbuf *segbuf)
{
	int	bufsz, i, fd;
	char	*buf;


	bufsz = MAX_SEGBUF_TEXT;
        buf = (char *) mmalloc(E, bufsz,
			"(char *)buf segbuf text in network-hitachi-sh.c");
        if (buf == NULL)
        {
		mexit(E, "Malloc failed in network-hitachi-sh.c for \"buf\"...", -1);
        }

	/*	Create if not there, append if there:	*/
	fd = mcreate(dumpname, M_OWRITE);
	if (fd < 0)
	{
		mprint(E, NULL, siminfo, 
			"Could not open destination \"%s\" for NETSEG2FILE\n",
			dumpname);

		return;
	}

	/*	The fd was created w/ mopen(), so must write w/ mprintfd	*/
	msnprint(buf, bufsz, "Timestamp: %E\n", segbuf->timestamp);
	mprintfd(fd, buf);

	msnprint(buf, bufsz, "Data: ");
	mprintfd(fd, buf);
	for (i = 0; i < segbuf->bits_left/8; i++)
	{
		msnprint(buf, bufsz, "%02X ", segbuf->data[i]);
		mprintfd(fd, buf);
		if (!((i+1) % 24))
		{
			msnprint(buf, bufsz, "\n");
			mprintfd(fd, buf);
		}
	}
	msnprint(buf, bufsz, ".\n");

	mprintfd(fd, buf);
	msnprint(buf, bufsz, "Bits left: 0x%08X\n", segbuf->bits_left);
	mprintfd(fd, buf);
	msnprint(buf, bufsz, "Src node: 0x%08X\n", ((State *)segbuf->src_node)->NODE_ID);
	mprintfd(fd, buf);

	if (segbuf->bcast)
	{
		msnprint(buf, bufsz, "Dst node: 0x%08X\n", -2);
		mprintfd(fd, buf);
	}
	else
	{
		msnprint(buf, bufsz, "Dst node: 0x%08X\n", ((State *)segbuf->dst_node)->NODE_ID);
		mprintfd(fd, buf);
	}

	msnprint(buf, bufsz, "Bcast flag: 0x%08X\n", segbuf->bcast);
	mprintfd(fd, buf);
	msnprint(buf, bufsz, "Src ifc: 0x%08X\n", segbuf->src_ifc);
	mprintfd(fd, buf);
	msnprint(buf, bufsz, "Parent netseg ID: 0x%08X\n", segbuf->parent_netsegid);
	mprintfd(fd, buf);
	msnprint(buf, bufsz, "from_remote flag: 0x%08X\n", segbuf->from_remote);
	mprintfd(fd, buf);
	msnprint(buf, bufsz, "\n\n\n\n");
	mprintfd(fd, buf);

	mclose(fd);


	return;
}

int
parsenetsegdump(Engine *E, char *buf, Segbuf *segbuf)
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
	if (tmp < E->nnodes && tmp >= 0)
	{
		segbuf->src_node = E->sp[tmp];
	}

	sscanf(buf+off, "Dst node: 0x%8X\n", &tmp);
	off += strlen("Dst node: 0xXXXXXXXX\n");

	if (tmp < E->nnodes && tmp >= 0)
	{
		segbuf->dst_node = E->sp[tmp];
	}

	sscanf(buf+off, "Bcast flag: 0x%8X\n", &segbuf->bcast);
	off += strlen("Bcast flag: 0xXXXXXXXX\n");

	sscanf(buf+off, "Src ifc: 0x%8X\n", &segbuf->src_ifc);
	off += strlen("Src ifc: 0xXXXXXXXX\n");

	sscanf(buf+off, "Parent netseg ID: 0x%8X\n", &segbuf->parent_netsegid);
	off += strlen("Parent netseg ID: 0xXXXXXXXX\n");

	sscanf(buf+off, "from_remote flag: 0x%8X\n", &segbuf->from_remote);
	off += strlen("from_remote flag: 0xXXXXXXXX\n");

	//netsegdump(E, "/tmp/parsenetsegdebug", segbuf);


	return off;
}

void
network_netseg2file(Engine *E, int which, char *filename)
{
	Netsegment	*tptr;


	if (which >= E->nnetsegs)
	{
		merror(E, "Network segment, [%d] is out of range.", which);

		return;
	}

	tptr = &E->netsegs[which];
	tptr->seg2filenames[tptr->num_seg2files] = 
		mcalloc(E, strlen(filename)+1,
		sizeof(char),
		"mcalloc() for tptr->seg2filenames[n] in shasm.y");

	if (tptr->seg2filenames[tptr->num_seg2files] == NULL)
	{
		merror(E, "Could not allocate memory for tptr->seg2filenames[].");
	}
	else
	{
		strcpy(tptr->seg2filenames[tptr->num_seg2files], filename);
		tptr->num_seg2files++;
	}

	return;
}


void
network_file2netseg(Engine *E, char *file, int whichseg)
{
	return;
}

void
network_netsegpropmodel(Engine *E, int whichseg, int whichmodel, double minsnr)
{
	Netsegment	*tptr;

	if (whichseg >= MAX_NETSEGMENTS)
	{
		merror(E, "Invalid netseg ID [%d] specified in \"netsegpropmodel\" command.",
			whichseg);
		return;
	}
	else if (whichmodel >= MAX_SIGNAL_SRCS)
	{
		merror(E, "Invalid sigsrc ID [%d] specified in \"netsegpropmodel\" command\n",
			whichmodel);
		return;
	}

	tptr = &E->netsegs[whichseg];
	tptr->sigsrc = &E->sigsrcs[whichmodel];
	tptr->minsnr = minsnr;

	return;
}

void
network_netnewseg(Engine *E, int which, int framebits, int pgnspeed, int bitrate, int width, 
		int fpdist, double fpdistmu, double fpdistsigma, double fpdistlambda,
		int fddist, double fddistmu, double fddistsigma, double fddistlambda)
{
	Netsegment 		*tptr;


	if (which >= MAX_NETSEGMENTS)
	{
		merror(E, "Segment # > max. number of network segments.");
		return;
	}

	if (framebits/8 > MAX_FRAMEBYTES)
	{
		merror(E, "Segment frame size is larger than max allowed.");
		return;
	}

	if (!E->netsegs[which].valid)
	{
		which = E->nnetsegs++;

		if (which >= MAX_NETSEGMENTS)
		{
			merror(E, "Maximum number of network segments reached.");
			return;
		}

		E->netsegs[which].valid = 1;
		E->activensegs[E->nactivensegs++] = which;
	}

	tptr = &E->netsegs[which];

	tptr->frame_bits = framebits;
	tptr->propagation_speed = pgnspeed;
	tptr->bitrate = bitrate;

	tptr->queue_max_width = width;
	if (tptr->queue_max_width == 0)
	{
		mprint(E, NULL, siminfo,
			"Queue width of 0 deprecated. Upgraded to width 1\n");
		tptr->queue_max_width = 1;
	}

	tptr->NETSEG_ID = which; 

	tptr->segbufs = (Segbuf *)mcalloc(E, tptr->queue_max_width, sizeof(Segbuf),
				"(Segbuf *)tptr->segbufs in shasm.y");
	if (tptr->segbufs == NULL)
	{
		merror(E, "mcalloc failed for (Segbuf *)tptr->segbufs in shasm.y.");
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

	fault_setnetsegpfun(E, tptr, "urnd");
	tptr->num_attached = 0;
	tptr->num_seg2files = 0;
	
	tptr->lastactivate = 0;
	tptr->bytedelay = 8.0/((double)tptr->bitrate);

	E->netperiodpsec = min(E->netperiodpsec, tptr->bytedelay);

	return;
}

void
network_netsegnicattach(Engine *E, State *S, int whichifc, int whichseg)
{
	int i;


	if (whichifc >= NIC_MAX_IFCS)
	{
		merror(E, "IFC # > max. number of IFCs.");
		return;
	}

	if (whichseg >= MAX_NETSEGMENTS)
	{
		merror(E, "Segment # > max. number of network segments.");
		return;
	}

	if (E->netsegs[whichseg].num_attached >= MAX_SEGNODES)
	{
		merror(E, "Maximum number of node IFCs attached to segment.");
		return;
	}

	E->netsegs[whichseg].node_ids[E->netsegs[whichseg].num_attached++] =
		S->NODE_ID;

	S->superH->NIC_IFCS[whichifc].segno = whichseg;
 	S->superH->NIC_IFCS[whichifc].frame_bits = E->netsegs[whichseg].frame_bits;

	/*	NIC_BRR contains network speed in Kb/s		*/
	S->superH->NIC_IFCS[whichifc].IFC_BRR = E->netsegs[whichseg].bitrate/1024;


	S->superH->NIC_IFCS[whichifc].rx_fifo =
		(uchar **)mcalloc(E, S->superH->NIC_IFCS[whichifc].rx_fifo_size,
		sizeof(uchar *), "shasm.y, (uchar **) for T_NETSEGNICATTACH");

	if (S->superH->NIC_IFCS[whichifc].rx_fifo == NULL)
	{
		merror(E, "Could not allocate memory for rx_fifo[].");
		return;
	}

	for (i = 0; i < S->superH->NIC_IFCS[whichifc].rx_fifo_size; i++)
	{
		S->superH->NIC_IFCS[whichifc].rx_fifo[i] =
			(uchar *)mcalloc(E, E->netsegs[whichseg].frame_bits/8,
			sizeof(uchar), "shasm.y, (uchar *) for T_NETSEGNICATTACH");

		if (S->superH->NIC_IFCS[whichifc].rx_fifo[i] == NULL)
		{
			merror(E, "Could not allocate memory for rx_fifo entry.");
			return;
		}
	}


	S->superH->NIC_IFCS[whichifc].tx_fifo =
		(uchar **)mcalloc(E, S->superH->NIC_IFCS[whichifc].tx_fifo_size, sizeof(uchar *),
		"shasm.y, (uchar **) for T_NETSEGNICATTACH");
				
	if (S->superH->NIC_IFCS[whichifc].tx_fifo == NULL)
	{
		merror(E, "Could not allocate memory for tx_fifo[].");
		return;
	}

	for (i = 0; i < S->superH->NIC_IFCS[whichifc].tx_fifo_size; i++)
	{
		S->superH->NIC_IFCS[whichifc].tx_fifo[i] =\
			(uchar *)mcalloc(E, E->netsegs[whichseg].frame_bits/8, sizeof(uchar),
			"shasm.y, (uchar *) for T_NETSEGNICATTACH");

		if (S->superH->NIC_IFCS[whichifc].tx_fifo[i] == NULL)
		{
			merror(E, "Could not allocate memory for tx_fifo entry.");
			return;
		}
	}

	S->superH->NIC_IFCS[whichifc].tx_fifo_framesizes =
		(int *)mcalloc(E, S->superH->NIC_IFCS[whichifc].tx_fifo_size,
		sizeof(int), "network-hitachi-sh.c, (int *) for tx_fifo_framesizes");
	if (S->superH->NIC_IFCS[whichifc].tx_fifo_framesizes == NULL)
	{
		merror(E, "Could not allocate memory for tx_fifo_framesizes[].");
		return;
	}

	S->superH->NIC_IFCS[whichifc].rx_fifo_framesizes =
		(int *)mcalloc(E, S->superH->NIC_IFCS[whichifc].rx_fifo_size,
		sizeof(int), "network-hitachi-sh.c, (int *) for rx_fifo_framesizes");
	if (S->superH->NIC_IFCS[whichifc].rx_fifo_framesizes == NULL)
	{
		merror(E, "Could not allocate memory for rx_fifo_framesizes[].");
		return;
	}

	/*	Local buffer for RX only	*/
	S->superH->NIC_IFCS[whichifc].rx_localbuf =
		(uchar *)mcalloc(E, S->superH->NIC_IFCS[whichifc].frame_bits/8, sizeof(uchar),
		"shasm.y, (uchar *) for S->superH->NIC_IFCS[whichifc].rx_localbuf");
			
	if (S->superH->NIC_IFCS[whichifc].rx_localbuf == NULL)
	{
		merror(E, "Could not allocate memory for rx_localbuf[].");
		return;
	}

	S->superH->NIC_IFCS[whichifc].valid = 1;
		

	return;	
}

void
network_netnodenewifc(
	Engine *E, State *S, int which, double txpwr, double rxpwr, double idlepwr, double listenpwr,
	int fpdist, int fpdistmu, int fpdistsigma, int fpdistlambda,
	int txfifosz, int rxfifosz)
{
	if (which >= NIC_MAX_IFCS)
	{
		merror(E, "Invalid node IFC number.");
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
	S->superH->NIC_IFCS[which].listen_pwr = listenpwr;
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
	msnprint((char*)&S->superH->NIC_IFCS[which].IFC_OUI[0], NIC_ADDR_LEN, "%d", S->NODE_ID);
		

	return;	
}

void
network_netsegdelete(Engine *E, int whichseg)
{
	int	i;

	if (whichseg >= MAX_NETSEGMENTS)
	{
		merror(E, "Segment # (%d) > max. number of network segments.", whichseg);
		return;
	}

	E->netsegs[whichseg].valid = 0;

	for (i = 0; i < E->nactivensegs; i++)
	{
		if (E->activensegs[i] == whichseg)
		{
			E->activensegs[i] = E->activensegs[E->nactivensegs - 1];
			E->activensegs[E->nactivensegs - 1] = 0;
			E->nactivensegs--;
		}
	}

	return;
}


void
network_netdebug(Engine *E, State *S)
{
	Ifc	*ifcptr;
	int	i;


	mprint(E, S, nodeinfo, "\nNode has [%d] active interfaces\n",
		S->superH->NIC_NUM_IFCS);

	for (i = 0; i < S->superH->NIC_NUM_IFCS; i++)
	{
		ifcptr = &S->superH->NIC_IFCS[i];

		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"NIC_OUI\" is [%s]\n",
			i, ifcptr->IFC_OUI);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"NIC_DST\" is [%s]\n",
			i, ifcptr->IFC_DST);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"valid\" is [%d]\n",
			i, ifcptr->valid);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"tx_pwr\" is [%E]\n",
			i, ifcptr->tx_pwr);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"rx_pwr\" is [%E]\n",
			i, ifcptr->rx_pwr);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"idle_pwr\" is [%E]\n",
			i, ifcptr->idle_pwr);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"fail_prob\" is [%E]\n",
			i, ifcptr->fail_prob);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"fail_clocks_left\" is [" UVLONGFMT "]\n",
			i, ifcptr->fail_clocks_left);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"frame_bits\" is [" INTFMT "]\n",
			i, ifcptr->frame_bits);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"segno\" is [" INTFMT "]\n",
			i, ifcptr->segno);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"rx_localbuf_h2o\" is [" INTFMT "]\n",
			i, ifcptr->rx_localbuf_h2o);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_size\" is [" UINTFMT "]\n",
			i, ifcptr->tx_fifo_size);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"rx_fifo_size\" is [" UINTFMT "]\n",
			i, ifcptr->rx_fifo_size);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_curidx\" is [" UINTFMT "]\n",
			i, ifcptr->tx_fifo_curidx);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"rx_fifo_curidx\" is [" UINTFMT "]\n",
			i, ifcptr->rx_fifo_curidx);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_oldestidx\" is [" UINTFMT "]\n",
			i, ifcptr->tx_fifo_oldestidx);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"rx_fifo_oldestidx\" is [" UINTFMT "]\n",
			i, ifcptr->rx_fifo_oldestidx);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_h2o\" is [" UINTFMT "]\n",
			i, ifcptr->tx_fifo_h2o);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"tx_fifo_maxoccupancy\" is [" UINTFMT "]\n",
			i, ifcptr->tx_fifo_maxoccupancy);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"rx_fifo_maxoccupancy\" is [" UINTFMT "]\n",
			i, ifcptr->rx_fifo_maxoccupancy);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"tx_alg_retries\" is [" UINTFMT "]\n",
			i, ifcptr->tx_alg_retries);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"fifo_nextretry_time\" is [%E]\n",
			i, ifcptr->fifo_nextretry_time);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_FSZ\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_FSZ);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_RDR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_RDR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_NSR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_NSR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_TDR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_TDR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_NCR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_NCR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_BRR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_BRR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_NMR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_NMR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_TXOK\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_TXOK);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_RXOK\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_RXOK);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_ADDR_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_ADDR_ERR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_FRAME_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_FRAME_ERR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_COLL_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_COLLS_ERR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_CSENSE_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_CSENSE_ERR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_RXOVRRUN_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_RXOVRRUN_ERR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_RXUNDRRUN_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_RXUNDRRUN_ERR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_TXOVRRUN_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_TXOVRRUN_ERR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_CNTR_TXUNDRRUN_ERR\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_CNTR_TXUNDRRUN_ERR);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_TXFIFO_LEVEL\" is [" ULONGFMT "]\n",
			i, ifcptr->IFC_TXFIFO_LEVEL);
		mprint(E, S, nodeinfo, "\t\tIFC [%d] \"IFC_RXFIFO_LEVEL\" is [" ULONGFMT "]\n\n",
			i, ifcptr->IFC_RXFIFO_LEVEL);
	
	}

	/*											*/
	/*	If sim quantum is > 0, this might be Inf (which is OK), however, in that	*/
	/*	case we may not want to print it out. (Or maybe somehow ensure that time	*/
	/*	is always valid even in case of large quantum.					*/
	/*											*/
	if (E->globaltimepsec < PICOSEC_MAX)
	{
		mprint(E, S, nodeinfo, "\nGlobal time = "UVLONGFMT"\n", E->globaltimepsec);
	}

	mprint(E, S, nodeinfo,
		"Total of " UVLONGFMT " bytes sent in simulation (all nodes)\n\n",
		E->nicsimbytes);
		

	return;	
}

void
network_setretryalg(Engine *E, State *S, int which, char *alg)
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
	else if (!strcmp(alg, "none"))
	{
		S->superH->NIC_IFCS[which].tx_fifo_retry_fxn = tx_retryalg_none;
	}
	else
	{
		merror(E, "Invalid Retry Algorithm supplied\n");
	}

	return;
}

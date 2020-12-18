#include "e-types.h"
#include "devsim7708.h"
#include "sh7708.h"
#include "fault.h"
#include "physics.h"
#include "network-hitachi-sh.h"
#include "devlog.h"
#include "print.h"
#include "devnet.h"
#include "misc.h"


extern void	splhi(void);
extern void	spldone(ulong);
extern ulong	getpsr(void);


int
devnet_getcurifc(void)
{
	/*	TODO: need to add either a mmap reg, or include this in NSR	*/
	return 0;
}

ulong
devnet_getncr(int whichifc)
{
	ulong	ncr = 0;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	((uchar *)&ncr)[3] = *(NIC_NCR+ifc_shift_offset+0);
	((uchar *)&ncr)[2] = *(NIC_NCR+ifc_shift_offset+1);
	((uchar *)&ncr)[1] = *(NIC_NCR+ifc_shift_offset+2);
	((uchar *)&ncr)[0] = *(NIC_NCR+ifc_shift_offset+3);

	return	ncr;
}

void
devnet_framedelay(int nframes, int whichifc)
{
	int frame_usecs = (1E6*nframes*devnet_getmaxfsz(whichifc)*8) / (devnet_getspeed(whichifc)*1024);

	xudelay(frame_usecs);
}

ulong
devnet_getmaxfsz(int whichifc)
{
	ulong	fsz = 0;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	((uchar *)&fsz)[3] = *(NIC_MAXFSZ+ifc_shift_offset+0);
	((uchar *)&fsz)[2] = *(NIC_MAXFSZ+ifc_shift_offset+1);
	((uchar *)&fsz)[1] = *(NIC_MAXFSZ+ifc_shift_offset+2);
	((uchar *)&fsz)[0] = *(NIC_MAXFSZ+ifc_shift_offset+3);


	return	fsz;
}

ulong
devnet_getrxfsz(int whichifc)
{
	ulong	fsz = 0;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	((uchar *)&fsz)[3] = *(NIC_RXFSZ+ifc_shift_offset+0);
	((uchar *)&fsz)[2] = *(NIC_RXFSZ+ifc_shift_offset+1);
	((uchar *)&fsz)[1] = *(NIC_RXFSZ+ifc_shift_offset+2);
	((uchar *)&fsz)[0] = *(NIC_RXFSZ+ifc_shift_offset+3);


	return	fsz;
}

ulong
devnet_getncolls(int whichifc)
{
	ulong	ncolls = 0;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	((uchar *)&ncolls)[3] = *(NIC_NCOLLS+ifc_shift_offset+0);
	((uchar *)&ncolls)[2] = *(NIC_NCOLLS+ifc_shift_offset+1);
	((uchar *)&ncolls)[1] = *(NIC_NCOLLS+ifc_shift_offset+2);
	((uchar *)&ncolls)[0] = *(NIC_NCOLLS+ifc_shift_offset+3);


	return	ncolls;
}

ulong
devnet_getncsense(int whichifc)
{
	ulong	ncsense = 0;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	((uchar *)&ncsense)[3] = *(NIC_NCSENSE+ifc_shift_offset+0);
	((uchar *)&ncsense)[2] = *(NIC_NCSENSE+ifc_shift_offset+1);
	((uchar *)&ncsense)[1] = *(NIC_NCSENSE+ifc_shift_offset+2);
	((uchar *)&ncsense)[0] = *(NIC_NCSENSE+ifc_shift_offset+3);


	return	ncsense;
}

ulong
devnet_getspeed(int whichifc)
{
	ulong	speed = 0;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	((uchar *)&speed)[3] = *(NIC_BRR+ifc_shift_offset+0);
	((uchar *)&speed)[2] = *(NIC_BRR+ifc_shift_offset+1);
	((uchar *)&speed)[1] = *(NIC_BRR+ifc_shift_offset+2);
	((uchar *)&speed)[0] = *(NIC_BRR+ifc_shift_offset+3);

	return	speed;
}

int
devnet_xmit(uchar *dst, uchar proto, uchar *data, int nbytes, int whichifc)
{
	int	framesize;
	int 	n = 0, i;
	uchar 	*tptr;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;
	ulong	savedSR;


//@@printf("in devnet_xmit, sending [%d] bytes to [%s], proto [%d], via ifc [%d]\n", nbytes, dst, proto, whichifc);

	/*	Save SR, then disable intrs:	*/
	savedSR = getpsr();
	splhi();


	/*	Save the dst for future use	*/
	for (i = 0; i < 16; i++)
	{
		*(NIC_DST+ifc_shift_offset+i) = *(dst+i);
	}

	/*	Write 16 byte src_addr	    	*/
	for (i = 0; i < 16; i++)
	{
		*(NIC_TDR+ifc_shift_offset) = *(NIC_OUI+i);
	}
	n += 16;

	/*	Write 16 byte dst_addr	    	*/
	for (i = 0; i < 16; i++)
	{
		*(NIC_TDR+ifc_shift_offset) = *(dst+i);
//printf("devnet_xmit(), *(dst+i) = [%d]\n", *(dst+i));
	}
	n += 16;

	/*	Write 4 byte payloadlen	    	*/
	*(NIC_TDR+ifc_shift_offset) = (nbytes>>24)&0xFF;
	*(NIC_TDR+ifc_shift_offset) = (nbytes>>16)&0xFF;
	*(NIC_TDR+ifc_shift_offset) = (nbytes>>8)&0xFF;
	*(NIC_TDR+ifc_shift_offset) = (nbytes>>0)&0xFF;
	n += 4;

	/*	Write 1 byte nexthdr		*/
	*(NIC_TDR+ifc_shift_offset) = proto;
	n += 1;


	/*					*/
	/*	The 32 bit cksum added by hw	*/
	/*	is not counted as part of fsz	*/
	/*					*/
	framesize = devnet_getmaxfsz(whichifc);


	/*	Write data	    	*/
	tptr = data;
	for (i = 0; (i < nbytes) && (n < framesize); i++)
	{
		*(NIC_TDR+ifc_shift_offset) = *tptr++;
		n++;
	}

	/*	if nbytes+header size was more than a frame, recurse	*/
	if (nbytes+16+16+4+1 > framesize)
	{
		print("Warning!!! devnet_xmit recursing: nbytes+header = %d, framesize = %d\n", nbytes+16+16+4+1, framesize);
		devnet_xmit(dst, proto, tptr, nbytes - i, whichifc);
	}
	else while (n < framesize)
	{
		/*	Send padding	*/
		*(NIC_TDR+ifc_shift_offset) = 0;
		n++;
	}



	/*	Restore SR	*/
	spldone(savedSR);


	/*					*/
	/*    If frame could'nt be sent, LSB	*/
	/*    of NIC_NSR will be 1, return -1	*/
	/*					*/
	return -((*(NIC_NSR+ifc_shift_offset)) & 0x1);
}

void
devnet_recv(uchar *recvbuf, int nbytes, int whichifc)
{
	int	i;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	for (i = 0; i < nbytes; i++)
	{
		*recvbuf++ = *(NIC_RDR+ifc_shift_offset);
	}

	return;
}

int
devnet_ctl(int cmd, int val, int whichifc)
{
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	if (cmd == NIC_NCR_WRITE)
	{
		*(NIC_NCR+ifc_shift_offset) = val;
	}
	else if (cmd == NIC_NCR_READ)
	{
		return *(NIC_NCR+ifc_shift_offset);
	}


	return -1;
}

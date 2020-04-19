#include "e-types.h"
#include "devsim7708.h"
#include "sh7708.h"
#include "fault.h"
#include "physics.h"
#include "network-hitachi-sh.h"
#include "devlog.h"
#include "print.h"
#include "devmac.h"
#include "misc.h"


extern void	splhi(void);
extern void	spldone(ulong);
extern ulong	getpsr(void);


void
devmac_cksum(uchar *header, uchar *data, int hdrlen, int datalen, uchar *frame_cksum)
{
	int	i;

	frame_cksum[0] = 0;
	frame_cksum[1] = 1;

//print("devmac_cksum: hdrlen = %d, datalen = %d\n\t", hdrlen, datalen);
	for (i = 0; i < hdrlen; i++)
	{
		frame_cksum[i & 1] ^= header[i]; 
//print("%d ", header[i]);
	}

	for (i = 0; i < datalen; i++)
	{
		frame_cksum[i & 1] ^= data[i]; 
//print("%d ", data[i]);
	}
//print("\n\ndone devmac_cksum, cksum = [%d][%d].\n", frame_cksum[0], frame_cksum[1]);

	return;
}


int
devmac_getcurifc(void)
{
	/*	TODO: need to add either a mmap reg, or include this in NSR	*/
	return 0;
}

ulong
devmac_getncr(int whichifc)
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
devmac_framedelay(int nframes, int whichifc)
{
	int frame_usecs = (1E6*nframes*devmac_getmaxfsz(whichifc)*8) / (devmac_getspeed(whichifc)*1024);

	xudelay(frame_usecs);
}

ulong
devmac_getmaxfsz(int whichifc)
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
devmac_getrxfsz(int whichifc)
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
devmac_getncolls(int whichifc)
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
devmac_getncsense(int whichifc)
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
devmac_getspeed(int whichifc)
{
	ulong	speed = 0;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	((uchar *)&speed)[3] = *(NIC_BRR+ifc_shift_offset+0);
	((uchar *)&speed)[2] = *(NIC_BRR+ifc_shift_offset+1);
	((uchar *)&speed)[1] = *(NIC_BRR+ifc_shift_offset+2);
	((uchar *)&speed)[0] = *(NIC_BRR+ifc_shift_offset+3);

	return	speed;
}

//BUG: we curretnly make the mac layer pass in the length of the destination address, so we can support multiple
//	mac layers with different address sizes.  Currently, the simulator maintains a 16+1 (NIC_ADDR_LEN) character
//	string format address.  When the MAC layer probes the NIC_OUI, it currently gets a string, which is
//	fine for now, but we neeed to clean this implementation up
int
devmac_xmit(uchar *dst, int dstaddrlen, uchar *header, uchar *data, int headerlen, int nbytes, int whichifc)
{
	int	max_framesize;
	int 	n = 0, i;
	uchar 	frame_cksum[2];
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;
	ulong	savedSR;


//print("in devmac_xmit, sending [%d] bytes to [%s], proto [%d], via ifc [%d]\n", nbytes, dst, proto, whichifc);

	/*	Save SR, then disable intrs:	*/
	savedSR = getpsr();
	splhi();


	/*					*/
	/*	The 32 bit cksum added by hw	*/
	/*	is not counted as part of fsz	*/
	/*					*/
	max_framesize = devmac_getmaxfsz(whichifc);

	/*	Save the dst for future use	*/
	for (i = 0; i < min(16, dstaddrlen); i++)
	{
		*(NIC_DST+ifc_shift_offset+i) = dst[i];
	}
	*(NIC_DST+ifc_shift_offset+i) = '\0';

	/*	Write MAC layer header	    	*/
	for (i = 0; i < headerlen && (n < max_framesize); i++)
	{
		*(NIC_TDR+ifc_shift_offset) = header[i];
		n++;
	}

	/*		Write data	    	*/
	for (i = 0; (i < nbytes) && (n < max_framesize); i++)
	{
		*(NIC_TDR+ifc_shift_offset) = data[i];
		n++;
	}

	/*		Write checksum	    	*/
	devmac_cksum(header, data, headerlen, nbytes, &frame_cksum[0]);
	for (i = 0; i < 2; i++)
	{
		*(NIC_TDR+ifc_shift_offset) = frame_cksum[i];
		n++;
	}


	/*							*/
	/*	The current frame size is calculated from 	*/
	/*	number of bytes in the TDR at this point	*/
	/*							*/
	devmac_ctl(NIC_NCR_WRITE, NIC_CMD_TRANSMIT, whichifc);


	/*	Restore SR	*/
	spldone(savedSR);


	/*					*/
	/*    If frame could'nt be sent, LSB	*/
	/*    of NIC_NSR will be 1, return -1	*/
	/*					*/
	return -((*(NIC_NSR+ifc_shift_offset)) & 0x1);
}

void
devmac_recv(uchar *recvbuf, int nbytes, int whichifc)
{
	int	i;
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;


	for (i = 0; i < nbytes; i++)
	{
		recvbuf[i] = *(NIC_RDR+ifc_shift_offset);
	}

	return;
}

int
devmac_ctl(int cmd, int val, int whichifc)
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

char
devmac_csense(int whichifc)
{
	ulong	ifc_shift_offset = (whichifc & 0xFFF) << 4;

//print("--- devmac_csense() returning [%d]\n", *(NIC_CSENSE+ifc_shift_offset));

	return	*(NIC_CSENSE+ifc_shift_offset);
}

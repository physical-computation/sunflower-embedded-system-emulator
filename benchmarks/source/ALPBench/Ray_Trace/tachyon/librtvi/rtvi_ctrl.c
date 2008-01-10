#include <stdio.h>
#include <stdlib.h>
#include <mcos.h>
#include "etvi_globals.h"
#include "etvi_hw.h"
#include "rtvi_ctrl.h"

static unsigned char *rc_buf=NULL;
static unsigned char *my_draw_buf=NULL;
static int            my_buf_num=1;

#define _race_assert(func) { \
    long rc=func; \
    if (rc) { \
        fprintf(stdout,"CEID %d: in file %s: line %d: Fatal error: ", \
                ce_getid(), __FILE__, __LINE__); \
        fflush(stdout); \
        err_print (rc,ERR_GET_ALL); \
        exit(1); } }

/***********************************************************************
 ***********************************************************************
  All of RTVI Setup will be done in this routine
  The default is to currently set to 1024x768, should
  allow for 32-bit color and double-buffering.
 ***********************************************************************
 ***********************************************************************/
void setup_buffers(void) {
	unsigned long i;

	/*****************************************************************/
	/* initialization that is needed only one time, by one rank only */
	/*****************************************************************/
	rc_buf = (unsigned char *)etvi_map(0,1); /* map in RTVI */

	/* Initialize the video mode here, from this list:
	 Valid values of the bank_num
	     Bank # 1 110 MHz 1280x1024 non-interlaced
	     Bank # 2  12 MHz RS-170 640x480 for ttvm
	     Bank # 3  12 MHz RS-170 640x480 display
	     Bank # 4  12 Mhz RS-170 640x480 frame capture
	     Bank # 5  64 MHz 1Kx768 non-interlaced
	     Bank # 6  is blank 
	     Bank # 7  12 MHz 512x256 non-interlaced for Condor
	     Bank # 8  50 MHz E&S 1280x1024 interlaced for ttvm
	     Bank # 9  50 MHz E&S 1280x1024 interlaced for display
	*/
	etvi_init(5); 
        setdot(75.0);

	/* Additional setup here, default font, LUT, that kind of stuff */
	etvi_setup() ;

        /* now clear the memory, by setting to some value */
	for(i=0; i<SCREEN_HI*LLSEP*2*sizeof(PIXPTR); i++) rc_buf[i]=0x00ff00ff;

	/*****************************************************************/
	/* The following code will allow additional ranks to attach to RTVI
	rc_buf = (unsigned char*)etvi_map(1,1);  */
	/*****************************************************************/

	/* set drawing to new back buffer */
	gdsel_draw(my_buf_num);

	my_draw_buf=rc_buf+buf_offset;
}

static SMB_handle  rtvi_handle;
static SMB_handle  buff_handle[2];
static DX_template xfer;
static DX_transfer xfer_r[2];

/***********************************************************************
 ***********************************************************************
  Here is the routine that will swap buffers for double-buffering
 ***********************************************************************
 ***********************************************************************/
void swap_buffers(void) {
	DX_status status;

	/* make sure all DMA activity is completed before swapping disp buffer */
        while (DX_QUEUED==dx_get_status(xfer_r[0],DX_POLL,&status));
        while (DX_QUEUED==dx_get_status(xfer_r[1],DX_POLL,&status));

	/* wait for next frame to make a clean display switch */
	wtnf();

	/* set display to previous back buffer */
	gdsel_disp(my_buf_num);

	/* now switch buffer number */
	my_buf_num=1-my_buf_num;

	/* set drawing to new back buffer */
	gdsel_draw(my_buf_num);

	my_draw_buf=rc_buf+buf_offset;
}

static int needinit=1;

/***********************************************************************
  Here is the routine that creates memory to DMA xfer to the RTVI
 ***********************************************************************/
PIXPTR *get_scanbuff(int buff_num) {
	char *scanline;
	unsigned long actual_size;
	unsigned long nchar;
	char smb_name[16];

	if ((buff_num>1)||(buff_num<0)) {
		fprintf(stderr,"valid buffer # values are 0 and 1\n");
		exit(1);
	}
	if (needinit) {
		nchar=80*1024*1024;
		_race_assert(smb_attach("rtvi",SMB_GLOBAL,nchar,0,
			        	&actual_size,&rtvi_handle))
	}
	sprintf(smb_name,"rtvi_buff_%d",buff_num);
	nchar=LLSEP*sizeof(PIXPTR);
	_race_assert(smb_open(smb_name,SMB_LOCAL|SMB_CREATE|SMB_ZERO,0,nchar,
			      0,&actual_size,&scanline,&(buff_handle[buff_num])))
	if (needinit) {
		_race_assert(dx_create(buff_handle[buff_num],rtvi_handle,0,0,0,0,0,&xfer))
		needinit=0;
	}
	_race_assert(dx_copy(xfer,0,0,SCREEN_WIDE*sizeof(PIXPTR),NULL,
			     NULL,0,&(xfer_r[buff_num])))
	return (PIXPTR*)scanline;
}

extern unsigned long my_rtvi_offset;

/***********************************************************************
 ***********************************************************************
  Here is the routine that will copy a scan line to the backbuffer
 ***********************************************************************
 ***********************************************************************/
void put_scanline(int line_num, int buff_num) {
	DX_status status;
	unsigned long i0 = line_num*LLSEP*sizeof(PIXPTR)+buf_offset+my_rtvi_offset;
        while (DX_QUEUED==dx_get_status(xfer_r[buff_num],DX_POLL,&status));
	dx_modify(xfer_r[buff_num],0,i0,0,0,DX_DESTINATION_OFFSET|DX_START);
}


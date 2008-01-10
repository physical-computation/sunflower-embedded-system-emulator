
#ifndef _RTVI_CTRL
#define _RTVI_CTRL

/***********************************************************************
  All of RTVI Setup will be done in this routine
  The default is to currently set to 1024x768, should
  allow for 32-bit color and double-buffering.
 ***********************************************************************/
void setup_buffers(void);

/***********************************************************************
  Here is the routine that will swap buffers for double-buffering
 ***********************************************************************/
void swap_buffers(void);

/***********************************************************************
  Here is the routine that creates memory to DMA xfer to the RTVI
 ***********************************************************************/
PIXPTR *get_scanbuff(int buff_num);

/***********************************************************************
  Here is the routine that will copy a scan line to the backbuffer
 ***********************************************************************/
void put_scanline(int line_num, int buff_num);

#endif


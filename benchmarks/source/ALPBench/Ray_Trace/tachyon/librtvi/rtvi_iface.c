#include <stdio.h>
#include <stdlib.h>
#include "etvi_globals.h"
#include "etvi_hw.h"
#include "rtvi_ctrl.h"
#include "rtvi_iface.h"

typedef struct {
  int flip;
  int xsize;
  int ysize;
  PIXPTR *scanline[2];
} rtvihandle;

void * rt_rtvi_init(int xsize, int ysize) {
  rtvihandle * handle;

  printf("Initializing Synergy RTVI Attached Framebuffer...\n");
  handle = malloc(sizeof(rtvihandle));

  handle->flip = 0;
  setup_buffers();
  handle->scanline[0] = get_scanbuff(0);
  handle->scanline[1] = get_scanbuff(1); 
  handle->xsize = xsize;
  handle->ysize = ysize;
  printf("\n");

  printf("Clearing Framebuffer...\n");
  rt_rtvi_clear(handle);

  return handle;
}

void rt_rtvi_clear(void * voidhandle) {
  rtvihandle * handle = (rtvihandle *) voidhandle; 
  long x, y;

  for(y=0; y<SCREEN_HI; y++) {
    for (x=0; x<SCREEN_WIDE; x++)
      handle->scanline[handle->flip][x]=0x00000000;

    put_scanline(y, handle->flip);
    handle->flip = 1 - handle->flip;
  }
  swap_buffers();

  for(y=0; y<SCREEN_HI; y++) {
    for (x=0; x<SCREEN_WIDE; x++)
      handle->scanline[handle->flip][x]=0x00000000;

    put_scanline(y, handle->flip);
    handle->flip = 1 - handle->flip;
  }
  swap_buffers();
} 

void rt_rtvi_displayimage(unsigned char * img, void * voidhandle) {
  rtvihandle * handle = (rtvihandle *) voidhandle; 
  long x, y, width, height, xoffset, yoffset;
  unsigned char * row;
  PIXPTR * rtvirow;

  if (handle->xsize < SCREEN_WIDE) {
    width = handle->xsize;
    xoffset = (SCREEN_WIDE - handle->xsize) / 2;
  } else {
    width = SCREEN_WIDE;
    xoffset = 0;
  }

  if (handle->ysize < SCREEN_HI) {
    height = handle->ysize;
    yoffset = SCREEN_HI - 1 - ((SCREEN_HI - handle->ysize) / 2);
  } else {
    height = SCREEN_HI;
    yoffset = SCREEN_HI - 1;
  }

  for (x=0; x<SCREEN_WIDE; x++) {
    handle->scanline[0][x] = 0x00000000; 
    handle->scanline[1][x] = 0x00000000; 
  }

  for(y=0; y<height; y++) {
    row = &img[y * handle->xsize * 3];
    rtvirow = handle->scanline[handle->flip];

    for (x=0; x<width; x++) {
     rtvirow[x+xoffset] = row[x*3] | (row[x*3 + 1] << 8) | (row[x*3 + 2] << 16);
    }

    put_scanline(yoffset - y, handle->flip);
    handle->flip = 1 - handle->flip;
  }

  swap_buffers();
}

#ifdef TEST_MAIN
int main(int argv, char argc) {
  int i;
  void * handle = rt_rtvi_init(512, 512);

  for (i=0; i<100; i++) { 
    rt_rtvi_clear(handle);
    swap_buffers();

    printf("# %d \r", i);
    fflush(stdout);
  }
  return 0;
}
#endif


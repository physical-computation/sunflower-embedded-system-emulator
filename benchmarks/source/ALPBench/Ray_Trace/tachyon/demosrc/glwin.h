
void * glwin_create(const char * wintitle, int width, int height);
void glwin_destroy(void * voidhandle);
void glwin_swap_buffers(void * voidhandle);
int glwin_handle_events(void * voidhandle);
void glwin_draw_image(void * voidhandle, int xsize, int ysize, unsigned char * img);


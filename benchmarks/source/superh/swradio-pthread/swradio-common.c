#include "print.h"
#include "swradio.h"

void
fb_compact(FloatBuffer *fb)
{
	fb->rlen -= fb->rpos;
	fb->rpos = 0;

	return;
}


int
fb_ensure_writable(FloatBuffer *fb, int amount)
{
	int available = IN_BUFFER_LEN - fb->rlen;

	if (available >= amount)
	{
    		return 1;
	}
  
	/* Nope, not enough room, move current contents back to the beginning. */
	fb_compact(fb);
  
	available = IN_BUFFER_LEN - fb->rlen;
	if (available >= amount)
	{
		return 1;
	}

	/*		Hmm.  We're probably hosed in this case.	*/
	print("fb_ensure_writable(0x%x): couldn't ensure %d bytes (only %d available)\n",
		fb, amount, available);


	return 0;
}

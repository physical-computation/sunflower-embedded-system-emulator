#include <stdio.h>
#include "print.h"

/*	8 GPRs + PR		*/
unsigned char	REGSAVESTACK[36];

static void		hdlr_install(void);
volatile int		gFlag = 1;

int
main(void)
{
	int	i = 0;

	hdlr_install();

	while (gFlag)
	{
		print("Counting, %d...\n", i++);
	}

	return 0;
}

void
intr_hdlr(void)
{
	gFlag = 0;

	return;
}

void
hdlr_install(void)
{
	extern	unsigned char	vec_stub_begin, vec_stub_end;
	unsigned char *		dstptr = (unsigned char *)0x8000600;
	unsigned char *		srcptr = &vec_stub_begin;


	/*	Copy the vector instructions to vector base	*/
	while (srcptr < &vec_stub_end)
	{
		*dstptr++ = *srcptr++;
	}

	return;
}

#include "e-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "devscc.h"
#include "sh7708.h"
#include "devrtc.h"
#include "misc.h"

#define	CALIBRATE	8	/*	Calibrated to give you ~correct delay @ 60MHz	*/

/*	Needed by print		*/
void
sccputc(int device, uchar ch)
{       
        while (!(*SCC_SCSSR & TX_RDY));

        *SCC_SCTDR = ch;
        *SCC_SCSSR &= TX_CLR;

        return;
}

/*	Needed by lprint	*/
void
devlogprintputc(int device, uchar ch)
{       
        *DEVLOGPRINT = ch;

        return;
}

void
xudelay(ulong usecs)
{
	volatile int	max, i;

	max = CALIBRATE*usecs;
	for (i = 0; i < max; i++)
	{
	}
}

void
xusleep(ulong usecs)
{
	ulong	time = devrtc_getusecs();

	while ((devrtc_getusecs() - time) < usecs)
	{
		sleep();	
	}
}

int
pow10(int y)
{
	int	i, ret = 1;

	for (i = 0; i < y; i++)
	{
		ret *= 10;
	}

	return ret;
}

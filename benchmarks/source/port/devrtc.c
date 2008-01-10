#include "e-types.h"
#include "devsim7708.h"
#include "sh7708.h"

ulong
devrtc_getusecs(void)
{
	ulong	usecs = 0;

	((uchar *)&usecs)[3] = *(SF_SECS+0);
	((uchar *)&usecs)[2] = *(SF_SECS+1);
	((uchar *)&usecs)[1] = *(SF_SECS+2);
	((uchar *)&usecs)[0] = *(SF_SECS+3);

	return	usecs;
}

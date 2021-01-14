#include "e-types.h"
#include "sh7708.h"
#include "devrtc.h"
#include "misc.h"

void
xusleep(ulong usecs)
{
	ulong	time = devrtc_getusecs();

	while ((devrtc_getusecs() - time) < usecs)
	{
		sleep();	
	}
}

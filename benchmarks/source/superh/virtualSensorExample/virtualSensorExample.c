#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "sf-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include "devscc.h"
#include "../../port/devrtc.h"
#include "../../port/devexcp.h"
#include "../../port/devlog.h"
#include "../../port/devloc.h"
#include "../../port/devsensor.h"
#include "../../port/misc.h"
#include "print.h"

volatile float		dam_myPr = 0.0;

int
main(void)
{
	while (1)
	{
		
		dam_myPr = devsignal_read(0);
		printf("Sensor Value = %f Timestamp = %ld",dam_myPr, devrtc_getusecs());
		printf("\n");

	}

	return 0;
}

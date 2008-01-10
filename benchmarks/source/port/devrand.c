#include "e-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"

ulong
devrand_getrand(void)
{
	ulong	rand = 0;

	((uchar *)&rand)[3] = *(RANDDR+0);
	((uchar *)&rand)[2] = *(RANDDR+1);
	((uchar *)&rand)[1] = *(RANDDR+2);
	((uchar *)&rand)[0] = *(RANDDR+3);

	return	rand;
}

void
devrand_seed(ulong seed)
{
	return;
}

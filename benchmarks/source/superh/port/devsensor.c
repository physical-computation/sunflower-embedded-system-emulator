#include "e-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include <stdint.h>
float
devsignal_read(uint32_t which)
{
	/*	Assumes simulator always formats read as same endiannness as modeled CPU	*/
	return *(SENSOR_READ + which);

}

void
devsignal_write(double value)
{
	return;
}

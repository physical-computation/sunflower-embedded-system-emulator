#include "e-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include <stdint.h>

double
devsignal_read(int which)
{
        ulong   sensor_shift_offset = (which & 0xFFF) << 2;
        return *(SENSOR_READ + sensor_shift_offset);
}

void
devsignal_write(double value)
{
	return;
}

#include "e-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include <stdint.h>

double
devsignal_read(int which)
{
        double  reading = 0.0;
        ulong   sensor_shift_offset = (which & 0xFFF) << 4;

        ((uchar *)&reading)[7] = *(SENSOR_READ+sensor_shift_offset+0);
        ((uchar *)&reading)[6] = *(SENSOR_READ+sensor_shift_offset+1);
        ((uchar *)&reading)[5] = *(SENSOR_READ+sensor_shift_offset+2);
        ((uchar *)&reading)[4] = *(SENSOR_READ+sensor_shift_offset+3);
        ((uchar *)&reading)[3] = *(SENSOR_READ+sensor_shift_offset+4);
        ((uchar *)&reading)[2] = *(SENSOR_READ+sensor_shift_offset+5);
        ((uchar *)&reading)[1] = *(SENSOR_READ+sensor_shift_offset+6);
        ((uchar *)&reading)[0] = *(SENSOR_READ+sensor_shift_offset+7);

        return reading;
}

void
devsignal_write(double value)
{
	return;
}

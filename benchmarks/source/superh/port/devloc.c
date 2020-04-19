#include "sf-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"

int
devloc_getorbit(void)
{
	int	orbit = 0;

	((uchar *)&orbit)[3] = *(ORBIT+0);
	((uchar *)&orbit)[2] = *(ORBIT+1);
	((uchar *)&orbit)[1] = *(ORBIT+2);
	((uchar *)&orbit)[0] = *(ORBIT+3);

	return	orbit;
}

int
devloc_getvelocity(void)
{
	int	velocity = 0;

	((uchar *)&velocity)[3] = *(VELOCITY+0);
	((uchar *)&velocity)[2] = *(VELOCITY+1);
	((uchar *)&velocity)[1] = *(VELOCITY+2);
	((uchar *)&velocity)[0] = *(VELOCITY+3);

	return	velocity;
}

double
devloc_getxloc(void)
{
	double	xloc = 0.0;

	((uchar *)&xloc)[7] = *(XLOC+0);
	((uchar *)&xloc)[6] = *(XLOC+1);
	((uchar *)&xloc)[5] = *(XLOC+2);
	((uchar *)&xloc)[4] = *(XLOC+3);
	((uchar *)&xloc)[3] = *(XLOC+4);
	((uchar *)&xloc)[2] = *(XLOC+5);
	((uchar *)&xloc)[1] = *(XLOC+6);
	((uchar *)&xloc)[0] = *(XLOC+7);

	return	xloc;
}

double
devloc_getyloc(void)
{
	double	yloc = 0.0;

	((uchar *)&yloc)[7] = *(YLOC+0);
	((uchar *)&yloc)[6] = *(YLOC+1);
	((uchar *)&yloc)[5] = *(YLOC+2);
	((uchar *)&yloc)[4] = *(YLOC+3);
	((uchar *)&yloc)[3] = *(YLOC+4);
	((uchar *)&yloc)[2] = *(YLOC+5);
	((uchar *)&yloc)[1] = *(YLOC+6);
	((uchar *)&yloc)[0] = *(YLOC+7);

	return	yloc;
}

double
devloc_getzloc(void)
{
	double	zloc = 0.0;

	((uchar *)&zloc)[7] = *(ZLOC+0);
	((uchar *)&zloc)[6] = *(ZLOC+1);
	((uchar *)&zloc)[5] = *(ZLOC+2);
	((uchar *)&zloc)[4] = *(ZLOC+3);
	((uchar *)&zloc)[3] = *(ZLOC+4);
	((uchar *)&zloc)[2] = *(ZLOC+5);
	((uchar *)&zloc)[1] = *(ZLOC+6);
	((uchar *)&zloc)[0] = *(ZLOC+7);

	return	zloc;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "sf-types.h"
#include "tag.h"
#include "devsim7708.h"
#include "sh7708.h"
#include "devscc.h"
#include "devrtc.h"
#include "devexcp.h"
#include "devlog.h"
#include "devloc.h"
#include "devsensor.h"
#include "misc.h"
#include "print.h"

float 
Schlieren(float a, float b) {
	return (a+b)/(a-b);
}

int
main(void)
{
	const int 	samplesPerDistribution = 30; 
	float 		temperatureDistribution[samplesPerDistribution];
	float 		humidityDistribution[samplesPerDistribution];
	for(int i = 0; i < samplesPerDistribution; i++)
	{
		xudelay(30);
		temperatureDistribution[i] = devsignal_read(0);
		humidityDistribution[i] = devsignal_read(1);
	}

	for(int i = 0; i < samplesPerDistribution; i++)
	{
		printf("(%f + %f)/(%f - %f) = %f\n", temperatureDistribution[i], humidityDistribution[i], temperatureDistribution[i], humidityDistribution[i], Schlieren(temperatureDistribution[i], humidityDistribution[i]));
	}	
	return 0;
}

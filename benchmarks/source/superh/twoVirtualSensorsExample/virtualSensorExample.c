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

float findMean(float samples[], int size);
float findStandardDeviation(float samples[], float mean, int size);

int
main(void)
{
	const int samplesPerDistribution = 30; 
	float temperatureDistribution[samplesPerDistribution];
	float humidityDistribution[samplesPerDistribution];
	float temperatureMean;
	float humidityMean; 
	float temperatureStandardDeviation;
	float humidityStandardDeviation;
    for(int n = 0 ; n < 100 ; n++)
    {
	for(uint32_t i = 0; i < samplesPerDistribution; i++)
	{
		xudelay(30);
		temperatureDistribution[i] = devsignal_read(0);
		humidityDistribution[i] = devsignal_read(1);
		//printf("Sensor Value = %f Timestamp = %ld", temperatureDistribution[i], devrtc_getusecs());
		//printf("\n");
		//printf("Sensor Value = %f Timestamp = %ld", humidityDistribution[i], devrtc_getusecs());
		//printf("\n");	
	}
    temperatureMean = findMean(temperatureDistribution, samplesPerDistribution);
    humidityMean = findMean(humidityDistribution, samplesPerDistribution);
    temperatureStandardDeviation = findStandardDeviation(temperatureDistribution, temperatureMean, samplesPerDistribution);
    humidityStandardDeviation = findStandardDeviation(humidityDistribution, humidityMean, samplesPerDistribution); 
    printf(" %u.%05u, ", (int)(temperatureMean), (int)((temperatureMean - (int)(temperatureMean))*100000)); 
    printf("%u.%05u, ", (int)(temperatureStandardDeviation), (int)((temperatureStandardDeviation - (int)(temperatureStandardDeviation))*100000));
    printf("%u.%05u, ", (int)(humidityMean), (int)((humidityMean - (int)(humidityMean))*100000)); 
    printf("%u.%05u, ", (int)(humidityStandardDeviation), (int)((humidityStandardDeviation - (int)(humidityStandardDeviation))*100000));
    printf("\n"); 
    }
    return 0;
}

float findMean(float samples[], int size)
{ 
    float sum = 0;
    for(int i = 0; i < size; i++)
    {
        sum += samples[i];
    }
    sum = sum/size; 
    return sum; 
}


float findStandardDeviation(float samples[], float mean, int size)
{
    float sum = 0;
    for(int i = 0; i < size; i++) 
    {
        sum += (samples[i]-mean)*(samples[i]-mean);
    }
    sum = sqrt(sum/size); 
    return sum; 
}

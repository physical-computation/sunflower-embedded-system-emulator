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
float findCorrelationCoefficient(float samplesX[], float samplesY[], float meanX, float meanY, float standardDeviationX, float standardDeviationY, int size);
float Skewness(float samples[], float mean, float standardDeviation, int size);
float Kurtosis(float samples[], float mean, float standardDeviation, int size);
float findNewUncertainty(float samplesH[], float meanH, float sigmaH, float rhoHT, float sigmaT, int size);
float normalPDF(float h, float meanH, float sigmaH, float sigmaT, float rhoHT);
void sortBubble(float *array, int n);
uint32_t linearCongruential(uint32_t previous);
void boxMueller(float mu, float sigma, float U1, float U2, float *z0, float *z1);
float sin_32(float x);
float cos_32(float x);
float cos_32s(float x);
void bubbleSort(float arr[], int n);
void swap(float *xp, float *yp);

float r0, r1 = 0;

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
	float revisedHumidityStandardDeviation;
	float correlation; 
	float temperatureSkewness;
	float humiditySkewness; 
	float temperatureKurtosis;
	float humidityKurtosis; 
	uint32_t uniformRandomSample=0;
    
    
    //printf("Sensor Value = %f Timestamp = %ld", devsignal_read(0), devrtc_getusecs());
	//printf("\n");
	//printf("Sensor Value = %f Timestamp = %ld", devsignal_read(1), devrtc_getusecs());
	//printf("\n");
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
    temperatureSkewness = Skewness(temperatureDistribution, temperatureMean, temperatureStandardDeviation, samplesPerDistribution); 
    humiditySkewness = Skewness(humidityDistribution, humidityMean, humidityStandardDeviation, samplesPerDistribution); 
    temperatureKurtosis = Kurtosis(temperatureDistribution, temperatureMean, temperatureStandardDeviation, samplesPerDistribution); 
	humidityKurtosis = Kurtosis(humidityDistribution, humidityMean, humidityStandardDeviation, samplesPerDistribution);
    printf(" %u.%05u, ", (int)(temperatureMean), (int)((temperatureMean - (int)(temperatureMean))*100000)); 
    printf("%u.%05u, ", (int)(temperatureStandardDeviation), (int)((temperatureStandardDeviation - (int)(temperatureStandardDeviation))*100000));
    temperatureSkewness = fabsf(temperatureSkewness);
    printf( "%u.%05u, ", (int)(temperatureSkewness), (int)((temperatureSkewness - (int)(temperatureSkewness))*100000));
    printf("%u.%05u, ", (int)(temperatureKurtosis), (int)((temperatureKurtosis - (int)(temperatureKurtosis))*100000));
    printf("%u.%05u, ", (int)(humidityMean), (int)((humidityMean - (int)(humidityMean))*100000)); 
    printf("%u.%05u, ", (int)(humidityStandardDeviation), (int)((humidityStandardDeviation - (int)(humidityStandardDeviation))*100000));
    humiditySkewness = fabsf(humiditySkewness);
    printf("%u.%05u, ", (int)(humiditySkewness), (int)((humiditySkewness - (int)(humiditySkewness))*100000));
    printf("%u.%05u, ", (int)(humidityKurtosis), (int)((humidityKurtosis - (int)(humidityKurtosis))*100000));
    if((temperatureSkewness < 2) && (humiditySkewness < 2) && (temperatureKurtosis < 7) && (humidityKurtosis < 7) && (humidityStandardDeviation > temperatureStandardDeviation*1.1))
    {
    correlation = findCorrelationCoefficient(humidityDistribution, temperatureDistribution, humidityMean, temperatureMean, humidityStandardDeviation, temperatureStandardDeviation, samplesPerDistribution);
    uniformRandomSample = (int)(humidityKurtosis);
    for(int i = 0 ; i < samplesPerDistribution-1 ; i = i + 2)
    {
  			uniformRandomSample = linearCongruential((uint32_t) uniformRandomSample);
  			humidityDistribution[i] = uniformRandomSample/4294967295.0;
   			uniformRandomSample = linearCongruential((uint32_t) uniformRandomSample);
   			humidityDistribution[i+1] = uniformRandomSample/4294967295.0; 
  			boxMueller(humidityMean, humidityStandardDeviation, humidityDistribution[i], humidityDistribution[i+1], &r0, &r1);
  			humidityDistribution[i] = r0;
  			humidityDistribution[i+1] = r1;
 	}
		bubbleSort(humidityDistribution, samplesPerDistribution);
		revisedHumidityStandardDeviation = findNewUncertainty(humidityDistribution, humidityMean, humidityStandardDeviation, correlation, temperatureStandardDeviation, samplesPerDistribution);
		printf("%u.%05u,\n", (int)(revisedHumidityStandardDeviation), (int)((revisedHumidityStandardDeviation - (int)(revisedHumidityStandardDeviation))*100000)); 
	
  	}
  	else 
  	{
		printf("0,\n"); 
    }
    for(int i = 0 ; i < samplesPerDistribution ; i++)
    {
//        printf("Sensor Value = %f Timestamp = %ld", temperatureDistribution[i], devrtc_getusecs());
//		printf("\n");
//	    printf("Sensor Value = %f Timestamp = %ld", humidityDistribution[i], devrtc_getusecs());
//		printf("\n");	
    }
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


float findCorrelationCoefficient(float samplesX[], float samplesY[], float meanX, float meanY, float standardDeviationX, float standardDeviationY, int size)
{ 
    float sumTop = 0;
    float sumBottom = 0;
    for(int i = 0; i < size; i++) 
    {
        sumTop += (samplesX[i]-meanX)*(samplesY[i]-meanY); 
    }
    sumBottom = sqrtf(standardDeviationX*standardDeviationX*size*standardDeviationY*standardDeviationY*size);
    return sumTop/sumBottom; 
}


float Skewness(float samples[], float mean, float standardDeviation, int size)
{
  float Skewness = 0;
  for(int i = 0; i < size; i++)
  {
  Skewness += (samples[i] - mean)*(samples[i] - mean)*(samples[i] - mean);
  }
  Skewness = Skewness/size;
  Skewness = Skewness/(standardDeviation*standardDeviation*standardDeviation);
  return Skewness;
}


float Kurtosis(float samples[], float mean, float standardDeviation, int size)
{
  float Kurtosis = 0;
  for(int i = 0; i < size; i++)
  {
  Kurtosis += (samples[i] - mean)*(samples[i] - mean)*(samples[i] - mean)*(samples[i] - mean);
  }
  Kurtosis = Kurtosis/size;
  Kurtosis = Kurtosis/(standardDeviation*standardDeviation*standardDeviation*standardDeviation);
  return Kurtosis;
}


float findNewUncertainty(float samplesH[], float meanH, float sigmaH, float rhoHT, float sigmaT, int size)
{
    float A = 0;
    float b = 0;
    float h = 0;  
    
    for(int i = 0; i < size-1; i++)
    {
        b = samplesH[i+1] - samplesH[i];
        h = (normalPDF(samplesH[i], meanH, sigmaH, sigmaT, rhoHT) + normalPDF(samplesH[i+1], meanH, sigmaH, sigmaT, rhoHT))/2; 
        A += b*h; 
    }
    return 1.0/(A*sqrtf(2*M_PI));
}


float normalPDF(float h, float meanH, float sigmaH, float sigmaT, float rhoHT) 
{
    float top = ((h-meanH)/sigmaH);
	top = top*top;
    float bottom = 2*(1-(rhoHT*rhoHT));
    float mult = 1/(2*M_PI*sigmaT*sigmaH*sqrtf(1-(rhoHT*rhoHT)));
    return mult*expf(-(top/bottom));
} 


void bubbleSort(float arr[], int n) 
{ 
   int i, j; 
   for (i = 0; i < n-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1]) 
              swap(&arr[j], &arr[j+1]); 
} 

void swap(float *xp, float *yp) 
{ 
    float temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 

uint32_t linearCongruential(uint32_t previous)
{
	return (1664525*previous + 1013904223) % 4294967296;
}

void boxMueller(float mu, float sigma, float U1, float U2, float *z0, float *z1)
{  
  float R2 = -2*logf(U1); 
  float R = sqrtf(R2);
  
  float theta = 2*M_PI*U2;  
  *z0 = R*cos_32(theta)*sigma + mu;
  *z1 = R*sin_32(theta)*sigma + mu; 
}

float cos_32s(float x)
{
	const float c1 = 0.99940307;
	const float c2 = -0.49558072;
	const float c3 = 0.03679168;
	
	float x2;
	x2 = x*x;
	return (c1 + x2*(c2 + c3 * x2));
}

float cos_32(float x)
{
	if(x<0) x = -x;
	int quad = floorf(x/M_PI_2);
	switch(quad) 
	{
		case 0: return cos_32s(x);
		case 1: return -cos_32s(M_PI-x);
		case 2: return -cos_32s(x-M_PI);
		case 3: return cos_32s((2*M_PI)-x);
	}
	return 0;
}

float sin_32(float x)
{
	return cos_32(M_PI_2-x);
}

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define BME680_CONCAT_BYTES(msb, lsb)	(((uint16_t)msb << 8) | (uint16_t)lsb)
#define BME680_CONCAT_BYTESxlsb(msb, xlsb)	(((uint32_t)msb << 4) | (uint32_t)xlsb)
float calcT(uint8_t temp_msb, uint8_t temp_lsb, uint8_t temp_xlsb, uint8_t CalibVals[]);
float calcH(uint8_t hum_msb, uint8_t hum_lsb, uint8_t CalibVals[], float temp_comp); 

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
main()
{
    const int samplesPerDistribution= 30; 
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
	
    for(int i = 0; i < samplesPerDistribution; i++)
	{
	
		temperatureDistribution[i] = 0;
		humidityDistribution[i] = 0;
		
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
		printf("%u.%05u, ", (int)(revisedHumidityStandardDeviation), (int)((revisedHumidityStandardDeviation - (int)(revisedHumidityStandardDeviation))*100000)); 
	
	}
	else 
	{
		printf("0, "); 
	}

}

float calcT(uint8_t temp_msb, uint8_t temp_lsb, uint8_t temp_xlsb, uint8_t CalibVals[]) 
	{ 	 	 	 
	// Put together the  temperature calibration parameters out of values from the calibration register
	uint16_t par_t1 =(uint16_t) (BME680_CONCAT_BYTES(CalibVals[34], CalibVals[33])); 
	int16_t  par_t2 =(uint16_t) (BME680_CONCAT_BYTES(CalibVals[2], CalibVals[1])); 	
	int8_t   par_t3 =(uint8_t)  CalibVals[3];   
	
	// Get the whole temperature value from the three registers it is spread across	 
	uint32_t temp_adc = (temp_msb << 12) | (temp_lsb << 4) | (temp_xlsb >> 4); 
		 	 	 	 
	// Define variables to be used to calculate the compensated temperature
	float t_fine;
	float var1 = 0;
	float var2 = 0;
	float calc_temp = 0;

	/* calculate var1 data */
	var1  = ((((float)temp_adc / 16384.0f) - ((float)par_t1 / 1024.0f))
			* ((float)par_t2));

	/* calculate var2 data */
	var2  = (((((float)temp_adc / 131072.0f) - ((float)par_t1 / 8192.0f)) *
		(((float)temp_adc / 131072.0f) - ((float)par_t1 / 8192.0f))) *
		((float)par_t3 * 16.0f));

	/* t_fine value*/
	t_fine = (var1 + var2); 

	/* compensated temperature data*/
	calc_temp  = ((t_fine) / 5120.0f);
	// Store the calculated temperature in the array that was passed by reference
	return calc_temp; 
	} 

	float calcH(uint8_t hum_msb, uint8_t hum_lsb, uint8_t CalibVals[], float temp_comp)
	{ 

	int16_t hum_adc= BME680_CONCAT_BYTES(hum_msb,hum_lsb);   

	// Combine the componets from the calibration array to get the calibration parameters
	uint16_t par_h1 = (uint16_t)(((uint16_t) CalibVals[27] << 4) | (CalibVals[26] & 0x0F));
	uint16_t par_h2 = (uint16_t)(((uint16_t) CalibVals[25] << 4) | (CalibVals[26] >> 4));
	int8_t   par_h3 = (int8_t) CalibVals[28]; 
	int8_t   par_h4 = (int8_t) CalibVals[29];
	int8_t   par_h5 = (int8_t) CalibVals[30]; 
	uint8_t  par_h6 = (uint8_t) CalibVals[31];
	int8_t   par_h7 = (int8_t) CalibVals[32];

	// Define variables to be used to calculate the compensated humidity
	float calc_hum = 0; 
	float var1 = 0;
	float var2 = 0;
    float var3 = 0;
	float var4 = 0;

	
        // Calculate humidity 
	var1 = (float)((float)hum_adc) - (((float)par_h1 * 16.0f) + (((float)par_h3 / 2.0f)
		* temp_comp));

	var2 = var1 * ((float)(((float) par_h2 / 262144.0f) * (1.0f + (((float)par_h4 / 16384.0f)
		* temp_comp) + (((float)par_h5 / 1048576.0f) * temp_comp * temp_comp))));

	var3 = (float) par_h6 / 16384.0f;

	var4 = (float) par_h7 / 2097152.0f;


	calc_hum = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2);
	// Humidity  can't be more than 100 % or less than 0 %
	if (calc_hum > 100.0f)
		calc_hum = 100.0f;
	else if (calc_hum < 0.0f)
		calc_hum = 0.0f;

	// Store the calculated temperature in the array that was passed by reference
	return calc_hum; 
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
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
#include <math.h>
#include <complex.h>

/*
 * 	Note that the number of samples must be a power of two for the FFT to work.
 * 	Currently the program will crash if numberOfSamples is not a power of 2.
 */
const int	numberOfSamples = 16;

int
main(void)
{
	float	X[numberOfSamples];
	/*
	 *	kK = Number of samples in output vector (number of points for Fourier transform)
	 */
	float	kK = numberOfSamples;
	/*
	 *	kN = Number of samples in inputs
	 */
	float	kN = numberOfSamples;
	float	kX[numberOfSamples];
	for(int j = 0; j < numberOfSamples; j++)
    	{
		/* 
		 *	Insert delay of 2500 uSeconds to simulate 400 Hz sample rate
		 */     
		xudelay(2500);
		/* 
		 *	Read sensor readings from sigsrc 0 which is the x-axis accelerometer readings
		 */
    		kX[j] = devsignal_read(0);
		/*	
		 *	Please note that sigsrc simulates a real signal which changes in time. 
		 *	The value returned by devsignal_read() will be different at different simulation times.
	   	 *	Here we use delay to wait for 1/f seconds where f is the 400 Hz sample frequency specified in the run.m file. 
	   	 *	If we don't wait for this amount of time and instead sample as quickly as we can we will see the same sensor value many times. 
	   	 *	If we set xudelay to a higher value we will start to miss some samples in the file as we are looking at the return value of i
		 *	devsignal_read() at a frequency lower than 400 Hz.
	 	 */
	}

	for(int k = 0; k < kK; k++)
    	{
        	for(int n = 0; n < kN; n++)
        	{
			X[k] = kX[n]*(cos((2*M_PI/kN)*k*n)-I*sin((2*M_PI/kN)*k*n));
        	}
    	}
		
	printf("Input: ");

    	for(int i = 0 ; i < numberOfSamples ; ++i)
    	{
        	printf("%f ", kX[i]);
    	}
        
	printf("\n");
	printf("Output: ");

	for(int i = 0 ; i < numberOfSamples ; ++i)
	{
        	printf("%f ", X[i]);
	}
        
	printf("\n");
	
	return 0;
}


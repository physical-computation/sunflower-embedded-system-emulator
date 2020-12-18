/* 
 *	Aya Helmy wrote this example for her 4B25 coursework. 
 *	James Meech adapted it to run over sunflower and take input from the sigsrc command.
 */
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
#include "../../port/devrtc.h"
#include "../../port/devexcp.h"
#include "../../port/devlog.h"
#include "../../port/devloc.h"
#include "../../port/devsensor.h"
#include "../../port/misc.h"
#include "print.h"

enum samples {numberOfSamples = 400};
struct 
accelAxis
{
  	/*
	 *	Hold data and info about single acceleration axis:
	 */  
	float 	data[numberOfSamples];  /*
				     *	array to store filtered data
				     */
	float	max;    /*
			 *	maximum data entry
			 */
			  
  	float 	min;    /*
			 *	minimum data entry
			 */  
  	float 	peakToPeak;    /*
			 *	peak to peak value
			 */	
  	float 	thresh; /*
			 *	current value of dynamic threshold 
			 */	
};

void 
findAxisProperties(struct accelAxis *axis)
{
  	/*
	 * Find peak-to-peak value and threshold for single-axis data
	 */
        float 	max = axis->data[0];
        float	min = axis->data[0];
        float	peakToPeak;
        float	thresh;

        for (int i=0; i<numberOfSamples; i++)
	{
        	if (axis->data[i] > max)
		{
                	max = axis->data[i];
            	}
            	if (axis->data[i] < min)
	    	{
                	min = axis->data[i];
            	}
        }

        peakToPeak = max-min;
        thresh = (max+min)/2.0;

        /*
	 *	Write values into struct
	 */	
        axis->max = max;
        axis->min = min;
        axis->peakToPeak = peakToPeak;
        axis->thresh = thresh;
}

int 
chooseAxis(struct accelAxis *x, struct accelAxis *y, struct accelAxis *z, float calib)
{
        /*
	 *	Perform maximum activity axis selection
	 */	

        float	peakToPeak[3];
        float	maxPeakToPeak = 0;
        int	maxIndex;

        findAxisProperties(x);
        findAxisProperties(y);
        findAxisProperties(z);

        peakToPeak[0] = x->peakToPeak;
        peakToPeak[1] = y->peakToPeak;
        peakToPeak[2] = z->peakToPeak;

        /*
	 *	Find axis with greatest peak-to-peak (p2p) amplitude
	 */	
        for (int i = 0; i < 3; i++)
	{
        	if (peakToPeak[i] > maxPeakToPeak)
		{
              		maxPeakToPeak = peakToPeak[i];
              		maxIndex = i;
          	}
        }

        /*
	 *	If p2p value of chosen axis is above amplitude calibration value then return chosen axis
	 */	
        if (maxPeakToPeak > calib) 
	{
		return (maxIndex + 1);
        }

        /*
	 *	If p2p value is less than calibration value then we assume invalid vibrations
         *	so return "invalid" 0 index - no steps are counted for this time window
	 */	
        else 
	{
        	return 0;
        }
}

float 
detectSteps(struct accelAxis *chosen)
{
	/*	
	 *	Finds where threshold is crossed in negative slope direction
	 */	

    	float	steps = 0;
    	float	current, next;
    	for(int i=0; i<numberOfSamples-1;i++)
	{
        	current = chosen->data[i];
        	next = chosen->data[i+1];
        if((current > chosen->thresh) && (next < chosen->thresh))
        {
        	steps = steps + 1;
        }
    	}
    	return steps;
}

void 
readRawData(float *tBuffer, float *xBuffer, float *yBuffer, float *zBuffer)
{
	for(int j = 0; j < numberOfSamples; j++)
    	{
		/* 
		 * Insert delay of 2500 uSeconds to simulate 400 Hz sample rate
		 */     
		xudelay(2500);
		/* 
		 * Read sensor readings from sigsrc 0 which is the x-axis accelerometer readings
		 */
    		xBuffer[j] = devsignal_read(0);
		/* 
		 * Read sensor readings from sigsrc 1 which is the y-axis accelerometer readings
		 */
    		yBuffer[j] = devsignal_read(1);
		/* 
		 * Read sensor readings from sigsrc 2 which is the z-axis accelerometer readings
		 */ 
    		zBuffer[j] = devsignal_read(2);
		/*	
		 *	Please note that sigsrc simulates a real signal which changes in time. 
		 *	The value returned by devsignal_read() will be different at different simulation times.
	   	 *	Here we use delay to wait for 1/f seconds where f is the 400 Hz sample frequency specified in the run.m file. 
	   	 *	If we don't wait for this amount of time and instead sample as quickly as we can we will see the same sensor value many times. 
	   	 *	If we set xudelay to a higher value we will start to miss some samples in the file as we are looking at the return value of i
		 *	devsignal_read() at a frequency lower than 400 Hz.
	 	 */
    	}	
}

void 
MovingAvgFilter(float input[], struct accelAxis *output)
{
	/*
	 *	Implement moving average low pass filtering
	 *	by using unweighted average of frames of 4 samples
	 */	

        float	sum = 0, mean;

        /*	
	 *	calculate mean of input array
	 */	
        for (int i=0; i<numberOfSamples; i++) 
	{
        	sum += input[i];
        }
        mean = sum/numberOfSamples;

        for (int i=0; i<numberOfSamples; i++) 
	{
        	/*
		 *	Find mean-subtracted input
		 */	
           	input[i] = input[i] - mean;
           	/*	
		 *	Initialise output to zero
		 */	
           	output->data[i] = 0;
        }

        for (int i=0; i<numberOfSamples; i++) 
	{
        	output->data[i] = input[i];
        }
}


int
main(void)
{
	int	chosenAxis;
	float	steps;
	float	totalSteps = 0;
		
	/*
	 *	used in calibration stage
	 */	
	float	calibrationMax;	
	
	/* 
	 *	Declare buffers to hold raw unfiltered acceleration data
	 */	
    	float	tBuffer[numberOfSamples];
    	float	xBuffer[numberOfSamples];
    	float	yBuffer[numberOfSamples];
    	float	zBuffer[numberOfSamples];

	/*
	 *	Declare struct for each acceleration axis (for use after filtering)
	 */	
    	struct	accelAxis xAcceleration;
    	struct	accelAxis yAcceleration;
    	struct	accelAxis zAcceleration;

	/*
	 *	empirically calculated minimum allowable calibration value
    	 *	in units of 0.25 mg
	 */	
    	calibrationMax = 1;

	/* 
	 *	Stage 2: Step Detection
	 */

    	/*
	 *	Read raw tri-axial accelerometer data and store in buffers
	 */	
    	readRawData(tBuffer, xBuffer, yBuffer, zBuffer);

	MovingAvgFilter(xBuffer, &xAcceleration);
    	MovingAvgFilter(yBuffer, &yAcceleration);
    	MovingAvgFilter(zBuffer, &zAcceleration);

    	/*
	 *	Peform maximal activity axis selection
	 */	
    	chosenAxis = chooseAxis(&xAcceleration, &yAcceleration, &zAcceleration, calibrationMax);
    	if(chosenAxis == 1)
	{
        	steps = detectSteps(&xAcceleration);
        }

     	else if(chosenAxis == 2)
	{
        	steps = detectSteps(&yAcceleration);
        }

     	else if(chosenAxis == 3)
	{
        	steps = detectSteps(&zAcceleration);
        }

     	else 
	{
        	steps = 0;
        }
    	totalSteps += steps;

    	/*
	 *	Reset count to 0 if it reaches 100
	 */	
    	if (totalSteps > 99)
	{
        	totalSteps = 0;
        }  
    	printf("Total steps = %f\n", totalSteps);
	return 0;
}

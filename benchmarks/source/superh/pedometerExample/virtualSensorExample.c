// Aya Helmy wrote this example for her 4B25 coursework. James Meech adapted it to run over sunflower and take input from the sigsrc command
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

#define BUFFER_SIZE 400
struct 
accel_axis
{
  	// Hold data and info about single acceleration axis:
	float 	data[BUFFER_SIZE];  //array to store filtered data
	float	max;    //maximum data entry
  	float 	min;    //minimum data entry
  	float 	p2p;    //peak to peak value
  	float 	thresh; //current value of dynamic threshold 
};

void 
findAxisProperties(struct accel_axis *axis)
{
  	// Find peak-to-peak value and threshold for single-axis data

        float 	max = axis->data[0];
        float	min = axis->data[0];
        float	p2p;
        float	thresh;

        for (int i=0; i<BUFFER_SIZE; i++)
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

        p2p = max-min;
        thresh = (max+min)/2.0;

        // Write values into struct
        axis->max = max;
        axis->min = min;
        axis->p2p = p2p;
        axis->thresh = thresh;
}

int 
chooseAxis(struct accel_axis *x, struct accel_axis *y, struct accel_axis *z, float calib)
{
        // Perform maximum activity axis selection

        float	p2p[3];
        float	max_p2p = 0;
        int	max_index;

        findAxisProperties(x);
        findAxisProperties(y);
        findAxisProperties(z);

        p2p[0] = x->p2p;
        p2p[1] = y->p2p;
        p2p[2] = z->p2p;

        // Find axis with greatest peak-to-peak (p2p) amplitude
        for (int i = 0; i < 3; i++)
	{
        	if (p2p[i] > max_p2p)
		{
              		max_p2p = p2p[i];
              		max_index = i;
          	}
        }

        // If p2p value of chosen axis is above amplitude calibration value then return chosen axis
        if (max_p2p > calib) 
	{
		return (max_index + 1);
        }

        // If p2p value is less than calibration value then we assume invalid vibrations
        // so return "invalid" 0 index - no steps are counted for this time window
        else 
	{
        	return 0;
        }
}

float 
detectSteps(struct accel_axis *chosen)
{
	// Finds where threshold is crossed in negative slope direction

    	float	steps = 0;
    	float	current, next;
    	for(int i=0; i<BUFFER_SIZE-1;i++)
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
readRawData(float *t_buffer, float *x_buffer, float *y_buffer, float *z_buffer)
{
	for(int j = 0; j < BUFFER_SIZE; j++)
    	{
		// Insert delay of 2500 uSeconds to simulate 400 Hz sample rate    
		xudelay(2500);
		// Read sensor readings from sigsrc 0 which is the x-axis accelerometer readings
    		x_buffer[j] = devsignal_read(0);
		// Read sensor readings from sigsrc 1 which is the y-axis accelerometer readings
    		y_buffer[j] = devsignal_read(1);
		// Read sensor readings from sigsrc 2 which is the z-axis accelerometer readings
    		z_buffer[j] = devsignal_read(2);
		/* Please note that sigsrc simulates a real signal which changes in time. The value returned by devsignal_read() will be different at different simulation times.
	   	Here we use delay to wait for 1/f seconds where f is the 400 Hz sample frequency specified in the run.m file. 
	   	If we don't wait for this amount of time and instead sample as quickly as we can we will see the same sensor value many times. 
	   	If we set xudelay to a higher value we will start to miss some samples in the file as we are looking at the return value of i
		devsignal_read() at a frequency lower than 400 Hz.
	 	*/
    	}	
}

void 
MovingAvgFilter(float input[], struct accel_axis *output)
{
	// Implement moving average low pass filtering
      	// by using unweighted average of frames of 4 samples

        float	sum = 0, mean;

        // calculate mean of input array
        for (int i=0; i<BUFFER_SIZE; i++) 
	{
        	sum += input[i];
        }
        mean = sum/BUFFER_SIZE;

        for (int i=0; i<BUFFER_SIZE; i++) 
	{
        	// Find mean-subtracted input
           	input[i] = input[i] - mean;
           	// Initialise output to zero
           	output->data[i] = 0;
        }

        for (int i=0; i<BUFFER_SIZE; i++) 
	{
        	output->data[i] = input[i];
        }
}


int
main(void)
{
	int	chosen_axis;
	float	steps;
	float	steps_total = 0;
	float	calib_max;            // used in calibration stage
	bool	use_butterworth;          // if true use Butterworth filter, else use Moving Average filter    
	
	// Declare buffers to hold raw unfiltered acceleration data
    	float	t_buffer[BUFFER_SIZE];
    	float	x_buffer[BUFFER_SIZE];
    	float	y_buffer[BUFFER_SIZE];
    	float	z_buffer[BUFFER_SIZE];

	// Declare struct for each acceleration axis (for use after filtering)
    	struct	accel_axis x_accel;
    	struct	accel_axis y_accel;
    	struct	accel_axis z_accel;

	// Select type of filtering
    	use_butterworth = false;

	//empirically calculated minimum allowable calibration value
    	// in units of 0.25 mg
    	calib_max = 1;

	/* Stage 2: Step Detection */

    	// Read raw tri-axial accelerometer data and store in buffers
    	readRawData(t_buffer, x_buffer, y_buffer, z_buffer);

	MovingAvgFilter(x_buffer, &x_accel);
    	MovingAvgFilter(y_buffer, &y_accel);
    	MovingAvgFilter(z_buffer, &z_accel);

    	// Peform maximal activity axis selection
    	chosen_axis = chooseAxis(&x_accel, &y_accel, &z_accel, calib_max);
    	if(chosen_axis == 1)
	{
        	steps = detectSteps(&x_accel);
        }

     	else if(chosen_axis == 2)
	{
        	steps = detectSteps(&y_accel);
        }

     	else if(chosen_axis == 3)
	{
        	steps = detectSteps(&z_accel);
        }

     	else 
	{
        	steps = 0;
        }
    	steps_total += steps;

    	// Reset count to 0 if it reaches 100
    	if (steps_total > 99)
	{
        	steps_total = 0;
        }  
    	printf("Total steps = %f\n", steps_total);
	return 0;
}

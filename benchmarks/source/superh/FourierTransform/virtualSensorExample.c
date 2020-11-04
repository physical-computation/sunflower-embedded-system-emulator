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
 * from https://rosettacode.org/wiki/Fast_Fourier_transform 
 */
double PI;
typedef double complex cplx;
/*
 * Note that the number fo samples must be a power of two for the FFT to work.
 * Currently the program will crash if numberOfSamples is not a power of 2.
const int numberOfSamples = 16;
void _fft(cplx buf[], cplx out[], int n, int step)
{
	if (step < n)
	{
		_fft(out, buf, n, step * 2);
	        _fft(out + step, buf + step, n, step * 2);
		
		for (int i = 0; i < n; i += 2 * step) 
		{
			cplx t = cexp(-I * PI * i / n) * out[i + step];
			buf[i / 2] = out[i] + t;
			buf[(i + n)/2] = out[i] - t;
		}
	}
}

void fft(cplx buf[], int n)
{
	cplx out[n];
	for (int i = 0; i < n; i++) out[i] = buf[i];

	_fft(buf, out, n, 1);
}

void show(const char * s, cplx buf[]) 
{
	printf("%s", s);
	for (int i = 0; i < numberOfSamples; i++)
		if (!cimag(buf[i]))
			printf("%g ", creal(buf[i]));
		else
			printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}

int
main(void)
{
	PI = atan2(1,1)*4;
	cplx		xBuffer[numberOfSamples];
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
		 *	Please note that sigsrc simulates a real signal which changes in time. 
		 *	The value returned by devsignal_read() will be different at different simulation times.
	   	 *	Here we use delay to wait for 1/f seconds where f is the 400 Hz sample frequency specified in the run.m file. 
	   	 *	If we don't wait for this amount of time and instead sample as quickly as we can we will see the same sensor value many times. 
	   	 *	If we set xudelay to a higher value we will start to miss some samples in the file as we are looking at the return value of i
		 *	devsignal_read() at a frequency lower than 400 Hz.
	 	 */
	}
	show("Data: ", xBuffer);
	printf("\n");
	fft(xBuffer, numberOfSamples);
	show("Data FFT: ", xBuffer);
	printf("\n");	
	return 0;
}


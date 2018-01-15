/*
	C program to infer the angular rate of the pendulum based on acceleration data.
	
	Authored 2017 by Shao-Tuan Chen.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "logmarkers.h"
#include "measurement.h"

/*
 *	Variable declaration:
 *
 *		PI : mathematical constant.
 *
 *		T : time step, 0.1s since the sampling rate is 10 Hz.
 *
 *		L : length of the pendulum, defined in the measurement header file.
 *
 *		G : acceleration due to gravity, 9.81 m/s^2.
 *
 *		THETA0: initial angle in degrees for the swinging pendulum.
 *   
 */


#define PI 3.14159265359
#define T 0.1
#define G 9.81


/*
 *	Notes:
 *
 *	(0)	"logmarkers.h" is the header file for LOGMARK, a macro to specify 
 * 		performance counting for Sunflower.
 *
 * 	Arrays:  
 *
 *	(1)	double acceleration[] : acceleration data acquired with MPU-9250, which is
 * 		included with the header file.
 *
 *	(2) double angularRate[] : angular rate data acquired with MPU-9250, which is
 * 		included with the header file, the angular rate data will serve as a comparison
 * 		between the inferred and the measured angular rate.
 *
 *	(2)	double radian[] : radian as a function of time, based on the length 
 *		of the pendulum "L" and gravity "G".
 *
 *	(3)	double gcos[] : cosine component of gravity G. According to 
 *		Equation 4 in the paper, we can obtain theta, the angular displacement
 * 		as a function of time, given the length and the initial angular
 * 		displacement of the pendulum. In the paper, the initial angle is 5
 * 		degrees, and the length is 10 cm. We then calculate "g cos (theta(t))"
 * 		based on "radian" array, and use as the input to Sunflower. The calculation
 * 		can be done offline, since it doesn't require any experimental data,
 * 		and it is a sinusoidal signal with constant amplitude. This is the 
 * 		reason why we've put "LOGMARK" after generating the gcos array. Constant 
 * 		amplitude approximation is the cause as in Figure 3, at the start of
 * 		the pendulum swing there's an overshoot for the inferred angular rate,
 * 		compared to the angular rate from the gyroscope. However, the results 
 *		show the inferred angular rate with high coherence to gyroscope data,  
 *		because the initial angle is small, and constant approximation for gcos 
 * 		can hold. For larger angle swings, we can further consider a second-order 
 *		approximation for gcos, where we treat theta as a damped response to 
 *		improve the accuracy for the inferred angular rate with acceleration data.
 *
 *	(4)	double sign[]: sign direction array, based on the period of the 
 *		pendulum. This array creates a square wave, which is applied to give the 
 *		correct direction of the inferred angular rate. The sign array gives a value 
 *		of "+1", when the value of gcos array is positive, and "-1" when the value 
 *		of gcos array is negative.
 *
 *	(5)	double inferred[] : output array for inferred angular rate based on 
 *		Equation 10 in the paper, where L (meter) is the length of the pendulum.
 *		Measurement noise causes erroneous computation of "NaN", in the inferred 
 *		angular rate when |acceleration[i]| < |gcos[i]|. 
 *		To ensure no erroneous value is present, the value of inferred[i] is assigned 
 *		to be assigned[i-1] if |acceleration[i]| < |gcos[i]|.  
 *
 */


double *inferGyroRobust(double *acceleration, double *angularRate, int numberOfSamples, 
						int startIndex, int inferLength);
int
startup(int argc, char *argv[]) 
{			
	int 			numberOfSamples = sizeof(acceleration)/sizeof(double);
	int 			startIndex = 0;	
	int 			i;
	
	double 			startValue = acceleration[startIndex];
	
	
   /*
	*		We set the start condition for aligning acceleration with gcos to be when
	*		the change of measured acceleration exceeds certain threshold value and 
	*		start to infer the angular rate of the pendulum. Here we choose the starting 
	*		threshold to be 0.03 m/s^2.	This is possible since we are measuring the
	* 		dynamics of the pendulum from standstill, therefore we can be sure the pendulum
	*		has started swinging when the measured acceleration changes significantly.
	*/

	for (i = 0; i < numberOfSamples; i++)
	{
			if (fabs((acceleration[i] - acceleration[i+1])) > THRESHOLD) 
			{
				startIndex = i;
				startValue = acceleration[startIndex];

				break;
			}
	}

	printf ("Start value: %f \n", startValue);	
	printf ("Start index: %i, numberOfSamples: %d\n", startIndex, numberOfSamples);
	
   /*
	*	Notes:
	*
	*	(1)	Here we print out the value and index of the measured acceleration which is aligned 
	*		with gcos for us to manually check whether the start condition is satisfied.
	*
	*	(2) We have implemented 2 functions to calculate the angular rate of the pendulum based
	*		on acceleration, "inferGyroBasic" and "inferGyroRobust". The basic implementaion 
	*		calculates the "gcos" and "sign" array based on the first alignment when the start 
	* 		condition for the measured acceleration is met. Even though this impelmentation is 
	*		very energy efficiecnt, the "sign" arrayr in this implementation is prone to misalignment 
	*		with the actual measured angular rate due to the misalignment between the peak in measured 
	*		acceleration and the calculated "gcos" array. The robust impelmentation therefore scans
	*		through the entire acceleration data and finds each peak, then the "gcos" and "sign" arrays
	*		will be recalibrated to the start condition at each peak of the acceleration data. This 
	*		approach largely improve the accuracy in the sign and the inferred value of the angular rate. 
	*
	*/	
		
	int 			inferLength = numberOfSamples - startIndex;
	
	double 			*inferredResult;
	
	inferredResult = inferGyroRobust(acceleration, angularRate, numberOfSamples, startIndex, inferLength);
	 		
	return 0;
}


double
*inferGyroBasic(double *acceleration, double *angularRate, int numberOfSamples, 
				int startIndex, int inferLength)
{
		
   /*
	*	Notes:
	*
	*	(1)	The acceleration and gcos need to be in sync with a 180 degrees phase angle
	*		to calculate the inferred angular rate from acceleration and gcos. 
	*
	*	(2)	When performing experiment, we always record the angular rate to start from 
	*		negative value to positive value. If the sign order is reversed, we just need
	*		to change the condition in sign[i] array from "<= PI ? -1 : 1" to "<= PI ? 1 : -1".
	*		However, this won't affect the start condition we have set in (2), since either the
	*		angular rate changes from positive to negative, or negative to positive, the 
	*		measured acceleration in both cases increase in value.
	*
	*/	
	
	int 			i;
	int				sign[inferLength];
		
	double			radian[numberOfSamples];
	double			gcos[numberOfSamples];
	double			inferred[numberOfSamples];
	
	for (i = 0; i < inferLength; i++) 
	{
			radian[i] = sqrt(G / L)* T * i;
			sign[i] = fmod(radian[i], 2 * PI) < PI GCOSSIGN;
			gcos[i + startIndex] = G * cos(THETA0 * PI / 180 * cos(radian[i]));
			
	}
	
	LOGMARK(0);
	
	for (i = 0; i < inferLength; i++) 
	{
		
			if (fabs(acceleration[i + startIndex]) > fabs(gcos[i + startIndex]))
			{
					inferred[i + startIndex] = 
						sign[i] * sqrt ((-acceleration[i + startIndex] - gcos[i + startIndex]) / L); 
					//printf("%f \n", gcos[i]);
			}
			else 
			{
					inferred[i + startIndex] = inferred[i + startIndex - 1] ; 
					//printf("%f \n", gcos[i]);
			}		
	}
			
	LOGMARK(1);

	for ( i = 0; i < numberOfSamples; i++ ) 
	{
		printf ("%f, %f, %f, %f \n", acceleration[i], angularRate[i], inferred[i], gcos[i]);
	}
	
	LOGMARK(2);
					
	return 0;
		
}

double
*inferGyroRobust(double *acceleration, double *angularRate, int numberOfSamples, 
				int startIndex, int inferLength)
{
	
   /*
	*	Notes:
	*
	*	(1)	The acceleration and gcos need to be in sync with a 180 degrees phase angle
	*		to calculate the inferred angular rate from acceleration and gcos. 
	*
	*	(2)	For the robust implementation, we scan through the acceleration data and find 
	*		peaks in each cycle, the corresponding "gcos" array will be aligned to each peak
	*		and the sign changes will also take place at each peak of the acceleration. The
	*		calculated "gcos" and "sign" are therefore used to derive the inferred angular rate
	*		of the swinging pendulum. 
	*/	
	

	int 			i;
	int 			j;
	int				sign[inferLength];

	double			radian[numberOfSamples];
	double			gcos[numberOfSamples];
	double			inferred[numberOfSamples];
	
	LOGMARK(0);

	for (j = 0; j < inferLength; ++j) 
		{ 
			sign[j] = 1; 
		}

	do
	{			
			if ((acceleration[i + startIndex] > acceleration[i + startIndex -1]) && 
				(acceleration[i + startIndex] > acceleration[i + startIndex +1])) 
			{
					radian[i] = 0;
					gcos[i + startIndex] = G * cos(THETA0 * PI / 180 * cos(radian[i]));
				
					if (i == 0) { sign[i] = STARTSIGN; }
					else
					{
						sign[i] = -1 * sign[i - 1];
					}
				
					//printf("%d \n", sign[i]);
					i = i + 1;
			}
			else
			{
					radian[i] = sqrt(G / L)* T * i;
					gcos[i + startIndex] = G * cos(THETA0 * PI / 180 * cos(radian[i]));
				
					if (i == 0) { sign[i] = STARTSIGN; }
					else
					{
						sign[i] = sign[i - 1];
					}
				
					//printf("%d \n", sign[i]);
					i = i + 1;
			}
			
	} while (i < inferLength);

	for (i = 0; i < inferLength; i++) 
	{
			if (fabs(acceleration[i + startIndex]) > fabs(gcos[i + startIndex]))
			{
					inferred[i + startIndex] = 
						sign[i] * sqrt ((-acceleration[i + startIndex] - gcos[i + startIndex]) / L); 
			}
			else 
			{
					inferred[i + startIndex] = inferred[i + startIndex - 1] ; 
			}		
	}

	LOGMARK(1);

	for ( i = 0; i < numberOfSamples; i++ ) 
	{
		printf ("%f, %f, %f, %f \n", acceleration[i], angularRate[i], inferred[i], gcos[i]);
	}
	

	LOGMARK(2);
					
	return 0;

}

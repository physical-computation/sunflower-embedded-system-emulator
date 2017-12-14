/*
	C program to infer the angular rate of the pendulum based on acceleration data.
	
	Authored 2017 by Shao-Tuan Chen.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "logmarkers.h"

/*
 *	Variable declaration:
 *
 *		PI : mathematical constant.
 *
 *  		t : time step, 0.1s since the sampling rate is 10 Hz.
 *
 *		l : length of the pendulum, 0.1 meter.
 *
 *		g : acceleration due to gravity, 9.81 m/s^2.
 *   
 */

#define PI 3.14159265359
#define t 0.1
#define l 0.1
#define g 9.81

/*
 *	Notes:
 *
 *	(0)	"logmarkers.h" is the header file for LOGMARK, a macro to specify 
 * 		performance counting for Sunflower.
 *
 *  	Arrays:  
 *
 *	(1)	long double acceleration[61] : acceleration data acquired with MPU-9250. 
 *
 *	(2)	long double radian[infer_length] : radian as a function of time, based on the length 
 *		of the pendulum "l" and gravity "g".
 *
 *	(3)	long double gcos[infer_length] : cosine component of gravity g. According to 
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
 *	(4)	long double sign[infer_length]: sign direction array, based on the period of the 
 *		pendulum. This array creates a square wave, which is applied to give the 
 *		correct direction of the inferred angular rate. The sign array gives a value 
 *		of "+1", when the value of gcos array is positive, and "-1" when the value 
 *		of gcos array is negative.
 *
 *	(5)	long double inferred[infer_length] : output array for inferred angular rate based on 
 *		Equation 10 in the paper, where 0.1 (meter) is the length of the pendulum.
 *		Measurement noise causes erroneous computation of "NaN", in the inferred 
 *		angular rate when |acceleration[i]| < |gcos[i]|. 
 *		To ensure no erroneous value is present, the value of inferred[i] is assigned 
 *		to be assigned[i-1] if |acceleration[i]| < |gcos[i]|.  
 *
 */

#define PI 3.14159265359
#define t 0.1
#define l 0.1
#define g 9.81

/*
 *	Variable declaration:
 *
 *		PI : mathematical constant.
 *
 *  		t : time step, 0.1 second since the sampling rate is 10 Hz.
 *
 *		l : length of the pendulum, 0.1 meter.
 *
 *		g : acceleration due to gravity, 9.81 m/s^2.
 *   
 */

int
startup(int argc, char *argv[]) 
{
					
		long double acceleration[61] = 
			{ 
				-9.770898,
				-9.766672,
				-9.770842,
				-9.773954,
				-9.769031,
				-9.766070,
				-9.770710,
				-9.772675,
				-9.816317,
				-9.852140,
				-9.774437,
				-9.831465,
				-9.818206,
				-9.777526,
				-9.797795,
				-9.838834,
				-9.781586,
				-9.802683,
				-9.815384,
				-9.787777,
				-9.801776,
				-9.824968,
				-9.794776,
				-9.783875,
				-9.809954,
				-9.802066,
				-9.772747,
				-9.799950,
				-9.812172,
				-9.785635,
				-9.794366,
				-9.816803,
				-9.780145,
				-9.785876,
				-9.813944,
				-9.789288,
				-9.779901,
				-9.813481,
				-9.793507,
				-9.776321,
				-9.814372,
				-9.795508,
				-9.775270,
				-9.806280,
				-9.801828,
				-9.772852,
				-9.799093,
				-9.806290,
				-9.774644,
				-9.792571,
				-9.810519,
				-9.778481,
				-9.784913,
				-9.811959,
				-9.786848,
				-9.778827,
				-9.809833,
				-9.791220,
				-9.774934,
				-9.806862,
				-9.796848,
			};
			
/*
*	Notes:
*
*	(1)	The acceleration and gcos need to be in sync with a 180 degrees phase angle
*		to calculate the inferred angular rate from acceleration and gcos. 
*
*	(2)	We set the start condition for aligning acceleration with gcos to be when
*		the change of measured acceleration exceeds certain value, here we choose 
*		the threshold to be 0.03 m/s^2.	This is possible since we are measuring the
* 		dynamics of the pendulum from standstill, therefore we can be sure the pendulum
*		has started swinging when the measured acceleration changes significantly.
*
*	(3)	When performing experiment, we always record the angular rate to start from 
*		negative value to positive value. If the sign order is reversed, we just need
*		to change the condition in sign[i] array from ">= PI ? 1 : -1" to ">= PI ? -1 : 1".
*		However, this won't affect the start condition we have set in (2), since either the
*		angular rate changes from positive to negative, or negative to positive, the 
*		measured acceleration in both cases increase in value.
*
*/	
		
		int acc_length = 61;
			
		int start_index = 0;
			
		int i = 0;
				
		long double start_value = acceleration[start_index];

		for (i = 0; i < acc_length; i++)
		{
				if ((acceleration[i] - acceleration[i+1]) > 0.03) 
			    {
						start_index = i;
	        
						start_value = acceleration[start_index];
			
						break;
			    }
		}
		
		printf ("Start value: %Lf \n",start_value);
			
		printf ("Start index: %i \n",start_index);
		
/*
*	Notes:
*
*	(1)	Here we print out the value and index of the measured acceleration which is aligned 
*		with gcos for us to manually check whether the start condition is satisfied.
*
*/	
		
		int infer_length = acc_length - start_index;
		
		long double radian[infer_length]; 
	
		long double gcos[infer_length];
		
		long double sign[infer_length];
		
		for (i = 0; i < infer_length; i++ ) 
		{
				radian[i] = sqrt(g / l) * t * i;
		
				gcos[i] = g * cos(5 * PI / 180 * cos(radian[i]));
		
				sign[i] = fmod(radian[i], 2 * PI) >= PI ? 1 : -1;
		}
	   
		long double inferred[infer_length];
		
		LOGMARK(0);

		for ( i = 0; i < infer_length; i++ ) 
		{
				if (fabs(acceleration[i + start_index]) > fabs(gcos[i]))
				{
						inferred[i] = sign[i] * sqrt ((-acceleration[i + start_index] - gcos[i]) / l) ; 
				} 
				else 
				{
						inferred[i] = inferred[i-1] ; 
				}		
		}

		LOGMARK(1);
	
		for ( i = 0; i < infer_length; i++ ) 
		{
		    	printf ("%Lf \n",inferred[i]);
		}
	
		LOGMARK(2);
 
		return 0;
	}

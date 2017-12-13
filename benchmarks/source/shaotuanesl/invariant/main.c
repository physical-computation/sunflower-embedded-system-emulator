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
 *	Notes:
 *
 *	(0)	"logmarkers.h" is the header file for LOGMARK, a macro to specify 
 * 		performance counting for Sunflower.
 *
 *  	Arrays:  
 *
 *	(1)	long double acceleration [63] : acceleration data acquired with MPU-9250. 
 *
 *	(2)	long double radian[63] : radian as a function of time, based on the length 
 *		of the pendulum "l" and gravity "g".
 *
 *	(3)	long double gcos [63] : cosine component of gravity g. According to 
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
 *	(4)	long double sign[63]: sign direction array, based on the period of the 
 *		pendulum. This array creates a square wave, which is applied to give the 
 *		correct direction of the inferred angular rate. The sign array gives a value 
 *		of "+1", when the value of gcos array is positive, and "-1" when the value 
 *		of gcos array is negative.
 *
 *	(5)	long double inferred [63] : output array for inferred angular rate based on 
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
 *  	t : time step, 0.1s since the sampling rate is 10 Hz.
 *
 *		l : length of the pendulum, 0.1 meter.
 *
 *		g : acceleration due to gravity, 9.81 m/s^2.
 *   
 */

int
startup(int argc, char *argv[]) 
{
		int i;
					
		long double acceleration[63] = 
			{ 
				-9.772714,
				-9.798804,
				-9.804063,
				-9.773774,
				-9.810173,
				-9.843967,
				-9.777526,
				-9.797795,
				-9.838834,
				-9.781586,
				-9.794288,
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
				-9.772907,
				-9.801904,
				-9.801979,
				-9.772991,
				-9.795726,
				-9.807166,
				-9.775240,
				-9.789308,
				-9.809207,
			};
	
		long double radian[63]; 
	
		long double gcos[63];
		
		long double sign[63];
		
		for (i = 0; i < 63; i++ ) 
		{
				radian[i] = sqrt(g / l) * t * i;
		
				gcos[i] = g * cos(5 * PI / 180 * cos(radian[i]));
		
				sign[i] = fmod(radian[i], 2 * PI) >= PI ? 1 : -1;
		}
 	   
		long double inferred[63];
		
		LOGMARK(0);

		for ( i = 0; i < 63; i++ ) 
		{
				if (fabs(acceleration[i]) > fabs(gcos[i]))
				{
						inferred[i] = sign[i] * sqrt ((-acceleration[i] - gcos[i]) / l) ; 
				} 
				else 
				{
						inferred[i] = inferred[i-1] ; 
				}		
		}

		LOGMARK(1);
	
		for ( i = 0; i < 63; i++ ) 
		{
		    	printf ("%Lf \n",inferred[i]);
		}
	
		LOGMARK(2);
 
		return 0;
}

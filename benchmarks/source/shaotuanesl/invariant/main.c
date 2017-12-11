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
/*

/*
 *	Arrays:
 *
 *	(1)	double acceleration [147] : acceleration data acquired with MPU-9250. 
 *
 *	(2)	double gcos [147] : cosine component of gravity g. According to 
 *		Equation 4 in the paper, we can obtain theta, the angular displacement
 * 		as a function of time, given the length and the initial angular
 * 		displacement of the pendulum. In the paper, the initial angle is 5
 * 		degrees, and the length is 10 cm. We then calculate "g cos (theta)"
 * 		based on theta, and use as the input to Sunflower. This calculation
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
 *	(3)	double inferred [147] : output array for inferred angular rate based on 
 *		Equation 10 in the paper, where 0.1 (meter) is the length of the pendulum.
 *
 */


int
startup(int argc, char *argv[]) 
{
	int i;
					
	double acceleration[147] = 
		{ 
			-9.784208984,
			-9.786875000,
			-9.790976563,
			-9.789609375,
			-9.780517578,
			-9.787011719,
			-9.779355469,
			-9.783046875,
			-9.782739258,
			-9.786054688,
			-9.777065430,
			-9.778569336,
			-9.789609375,
			-9.782192383,
			-9.785815430,
			-9.790258789,
			-9.789267578,
			-9.790156250,
			-9.784448242,
			-9.787456055,
			-9.787797852,
			-9.787866211,
			-9.803212891,
			-9.884970703,
			-9.935146484,
			-9.904316406,
			-9.866035156,
			-9.719541016,
			-9.781611328,
			-9.864121094,
			-9.910058594,
			-9.926464844,
			-9.839375000,
			-9.781337891,
			-9.796787109,
			-9.852636719,
			-9.900009766,
			-9.900488281,
			-9.818593750,
			-9.796855469,
			-9.777441406,
			-9.840400391,
			-9.894267578,
			-9.873144531,
			-9.833085938,
			-9.788652344,
			-9.777031250,
			-9.858447266,
			-9.885996094,
			-9.865761719,
			-9.812509766,
			-9.797333984,
			-9.795830078,
			-9.840537109,
			-9.881552734,
			-9.852636719,
			-9.838623047,
			-9.806357422,
			-9.809638672,
			-9.841972656,
			-9.890029297,
			-9.835136719,
			-9.815859375,
			-9.792412109,
			-9.815859375,
			-9.832744141,
			-9.852500000,
			-9.863164063,
			-9.811757813,
			-9.784619141,
			-9.800341797,
			-9.835751953,
			-9.847578125,
			-9.844775391,
			-9.807177734,
			-9.793232422,
			-9.814765625,
			-9.847783203,
			-9.859746094,
			-9.837802734,
			-9.831992188,
			-9.824609375,
			-9.821806641,
			-9.832470703,
			-9.839443359,
			-9.806357422,
			-9.808339844,
			-9.820576172,
			-9.806152344,
			-9.831445313,
			-9.858310547,
			-9.814082031,
			-9.815107422,
			-9.825839844,
			-9.812646484,
			-9.846416016,
			-9.827138672,
			-9.814355469,
			-9.816337891,
			-9.822421875,
			-9.822626953,
			-9.841083984,
			-9.833906250,
			-9.822968750,
			-9.814492188,
			-9.810664063,
			-9.829257813,
			-9.841835938,
			-9.824335938,
			-9.806972656,
			-9.816064453,
			-9.803007813,
			-9.819345703,
			-9.842109375,
			-9.840263672,
			-9.817431641,
			-9.825224609,
			-9.817294922,
			-9.802187500,
			-9.819550781,
			-9.818935547,
			-9.826318359,
			-9.819550781,
			-9.827958984,
			-9.816337891,
			-9.805058594,
			-9.826523438,
			-9.816884766,
			-9.820371094,
			-9.819482422,
			-9.828642578,
			-9.828300781,
			-9.808339844,
			-9.838417969,
			-9.814492188,
			-9.802939453,
			-9.819140625,
			-9.822216797,
			-9.818183594,
			-9.802255859,
			-9.815380859,
			-9.806494141,
			-9.830078125,
			-9.821123047,
			-9.813671875,
			-9.806904297,
			-9.830693359,
		};

	/* time step is 0.01 for gcos */
	
	double gcos[147];
	
	for (i = 0; i < 147; i++ ) 
	{
		gcos[i] = 9.8 * cos(5 * 3.14 / 180 * cos(sqrt (9.8 / 0.1) * 0.01 * i));	
	}
 
	double inferred[147];
		
	LOGMARK(0);

	for ( i = 0; i < 147; i++ ) 
	{
		inferred[i] = sqrt ((acceleration[i] + gcos[i]) / 0.1) ; 
	}

	LOGMARK(1);
 
	return 0;
}

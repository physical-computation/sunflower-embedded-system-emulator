/*										*/
/*		Copyright (C) 1999-2006 Phillip Stanley-Marbell.		*/
/*										*/
/*	You may redistribute and/or copy the source and binary			*/
/*	versions of this software provided:					*/
/*										*/
/*	1.	The source retains the copyright notice above, 			*/
/*		this list of conditions, and the following 			*/
/*		disclaimer.							*/
/*										*/
/*	2.	Redistributed binaries must reproduce the above			*/
/*		copyright notice, this list of conditions, and			*/
/*		the following disclaimer.					*/
/*										*/
/*	3.	The source and binaries are used for non-commercial 		*/
/*		purposes only.							*/
/*										*/
/*	4.	Permission is granted to all others, to redistribute		*/
/*		or make copies of the source or binaries, either		*/
/*		as it was received or with modifications, provided		*/
/*		the above three conditions are enforced and 			*/
/*		preserved, and that permission is granted for 			*/
/*		further redistribution, under the conditions of this		*/
/*		document.							*/
/*										*/
/*	The source is provided "as is", and no warranty is either		*/
/*	implied or explicitly granted. The authors will not be liable 		*/
/*	for any damages arising in any way out of the use or misuse 		*/
/*	of this software, even if advised of the possibility of such 		*/
/*	damage.									*/
/*										*/
/*	Contact: phillip Stanley-Marbell <pstanley@ece.cmu.edu>			*/
/*										*/	
typedef struct Signalsrc Signalsrc;
typedef struct Sensor Sensor;
typedef struct Actuator Actuator;

enum
{
	/*		Physical phenomena			*/
	MAX_SIGNAL_SRCS	= 8,
	MAX_SIGNALSRC_SUBSCRIBERS = 512,
	MAX_NODE_SENSORS = 8,
};

struct Signalsrc
{
	int	valid;
	int	ID;

	/*							*/
	/*	During each sampling period, signals from	*/
	/*	all sources of the same 'type' are summed	*/
	/*	at all subscribed nodes. Thus, you can		*/
	/*	define, e.g., 2 phenomyna of the same type,	*/
	/*	that interfere w/ each other.			*/
	/*	You can implement non-monotonic degradation	*/
	/*	effects like shadowing, multipath etc.,		*/
	/*	by defining additional signals of the same	*/
	/*	type with negative amplitudes, located at	*/
	/*	the positions of pronounced attenuation, and	*/
	/*	furthermore, recall all signals can have	*/
	/*	a flexible spatial attenuation relation.	*/
	/*							*/
	/*	It would be nice to implement a graphical tool	*/
	/*	that defines a terrain (e.g. as contour maps)	*/
	/*	and signal sources, and outputs configs to	*/
	/*	be provided to the simulator as input		*/
	/*							*/
	int		type;


	/*	A textual description of the phenomenon		*/
	char		*description;


	/*	Nodes that are subscribed to this phenomenon	*/
	Sensor*		subscribed_sensors[MAX_SIGNALSRC_SUBSCRIBERS];
	int		numsubscribed;

	/*							*/
	/*	The quantum of updates to subscribers of Phn.	*/
	/*	Physical phenomena such as sound propoagation	*/
	/*	will have larger constants than, say, EM rad.	*/
	/*							*/
	double		updates_tau;


	/*							*/
	/*	The signal propagation speed in environment	*/
	/*							*/
	double		propagation_speed;


	/*							*/
	/*	  Constants defining spatial attenuation. 	*/
	/*	The general form is:				*/
	/*							*/
	/* signal = S*(A*x^m + B*x^n + C*x^o + D*x^p + E*K^(F*x^q + G*x^r + H*x^s + I*x^t))*/
	/*							*/
	/*	where a..d and m..p are constants defined	*/
	/*	for each phenomenon, defaulting to 0. Thus,	*/
	/*	e.g., for wireless comms.:			*/
	/*		a = 1, m = -2, i.e.:			*/
	/*		signal = signal * 1/r^2			*/
	/*	Ideally, you should use a tool like		*/
	/*	Mathematica to fit data representative	of	*/
	/*	the physical phenomenon to the equation		*/
	/*	above, to obtain the coeffs.			*/
	/*							*/
	double		const_coeff_A;
	double		const_coeff_B;
	double		const_coeff_C;
	double		const_coeff_D;
	double		const_coeff_E;
	double		const_coeff_F;
	double		const_coeff_G;
	double		const_coeff_H;
	double		const_coeff_I;

	double		const_base_K;

	double		const_pow_m;
	double		const_pow_n;
	double		const_pow_o;
	double		const_pow_p;
	double		const_pow_q;
	double		const_pow_r;
	double		const_pow_s;
	double		const_pow_t;

	/*							*/
	/*	Positioning of signal source in 3-space		*/
	/*	and trajectory / speed of motion.		*/
	/*							*/
	double		xloc;
	double		yloc;
	double		zloc;
	double		*xlocs;
	double		*ylocs;
	double		*zlocs;

	char		*trajectory_file;
	int		nlocations;
	int		trajectory_rate;
	int		looptrajectory;


	/*							*/
	/*	Variation of unattenuated signal in time.	*/
	/*	The array of values is may be read from a file.	*/
	/*	E.g., for sound waves, this is basically	*/
	/*	the audio signal. The sampling rate defines	*/
	/*	how often subscribers to phenomenon have their	*/
	/*	sensor values updated. Sampling rates smaller	*/
	/*	than the minimum simulation step are 		*/
	/*	thresholded. Sampling rate is in samples/sec	*/
	/*	A sample rate of 0 is used for constant signals	*/
	/*							*/
	double		sample;
	double		*samples;

	char		*samples_file;
	int		nsamples;
	int		sample_rate;
	int		loopsamples;
};

struct Sensor
{
	/*		Owner is..				*/
	void		*parent;

	/*		Which sensor are we:			*/
	int		which;		
	
	double		reading;
};

struct Actuator
{
	Signalsrc	signalsrc;
};

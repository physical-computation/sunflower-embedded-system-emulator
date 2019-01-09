/*									*/
/*	Adapted from							*/
/*		"fmref.c: C reference implementation of FM Radio	*/
/*		David Maze <dmaze@cag.lcs.mit.edu>			*/
/*		$Id: fmref.c,v 1.8 2002/07/29 14:05:48 aalamb Exp $"	*/
/*	by								*/
/*		pstanley@ece.cmu.edu					*/
/*									*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "misc.h"
#include "print.h"
#include "devrtc.h"
#include "devlog.h"
#include "swradio.h"


#define RELEASE_0(c)		pthread_mutex_lock(&(c)->mutex0);\
				pthread_cond_broadcast(&(c)->cond0);\
				pthread_mutex_unlock(&(c)->mutex0);

#define	PRE_WAIT_0(c)		pthread_mutex_lock(&(c)->mutex0);

#define	POST_WAIT_0(c)		pthread_cond_wait(&(c)->cond0, &(c)->mutex0);\
				pthread_mutex_unlock(&(c)->mutex0);

#define RELEASE_1(c)		pthread_mutex_lock(&(c)->mutex1);\
				pthread_cond_broadcast(&(c)->cond1);\
				pthread_mutex_unlock(&(c)->mutex1);

#define	PRE_WAIT_1(c)		pthread_mutex_lock(&(c)->mutex1);

#define	POST_WAIT_1(c)		pthread_cond_wait(&(c)->cond1, &(c)->mutex1);\
				pthread_mutex_unlock(&(c)->mutex1);

#define CHAN_INIT(c)		pthread_cond_init(&(c)->cond0, NULL);\
				pthread_cond_init(&(c)->cond1, NULL);\
				pthread_mutex_init(&(c)->mutex0, NULL);\
				pthread_mutex_init(&(c)->mutex1, NULL);

#define CHAN_DESTROY(c)		pthread_mutex_destroy(&(c)->mutex0);\
				pthread_cond_destroy(&(c)->cond0);\
				pthread_mutex_destroy(&(c)->mutex1);\
				pthread_cond_destroy(&(c)->cond1);

#define	POST_NATAL(in)		PRE_WAIT_1(in)\
				RELEASE_0(in)

#define	SEND(out)		PRE_WAIT_0(out)\
				RELEASE_1(out)\
				POST_WAIT_0(out)

#define	RECEIVE(in, use)	POST_WAIT_1(in)\
				use;\
				PRE_WAIT_1(in)\
				RELEASE_0(in)

typedef struct
{
	pthread_mutex_t		mutex0;
	pthread_mutex_t		mutex1;
	pthread_cond_t		cond0;
	pthread_cond_t		cond1;

	int			data;
} Chan;

float eq_cutoffs[EQUALIZER_BANDS + 1] =
{
	55.00,
	77.78,
	110.00,
	155.56,
	220.00,
	311.12,
    	440.00,
	622.25,
	880.00,
	1244.50,
	1760.00
};

enum
{
	SLOWDOWN	= 10,
	PERIOD		= 125*SLOWDOWN,

	MAX_SAMPLES	= 8192,
};

volatile FloatBuffer	fb1, fb2, fb3[NUM_SUPERPIPE_EQ], fb4[NUM_SUPERPIPE_EQ];
LPFData			lpf_data;
EqualizerData		eq_data;
float			lpf_out[EQUALIZER_BANDS + 1];
Chan			*sinkout;

static void		lpf(Chan *);
static void		demod(Chan *);
static void		eq(Chan *);
static void		sink(Chan *);
static void		fatal(char *);


//TODO: The way we are splitting up the data around the EQ stage is
//	decimating it by NUM_SUPERPIPE_EQ times. Technically, we
//	need some more code in sink to reconstruct it ?
void
main(int argc, char *argv[])
{
	int		i, status;
	ulong		lastsend = 0;
	Chan		*out;
	pthread_t	tid;

	
	/*									*/
	/*	Argument is the integer value of which stage (prev/next)	*/
	/*	gets to have the buffer as local. All the code in this file	*/
	/*	assumes source thread is mapped to node 0, lpf to 1, eqs to 	*/
	/*	nodes 3-10 and sink on node 11.					*/
	/*									*/ 
	if (argc == 1)
	{
		char	tmpchar, *ep = &tmpchar;
		int	tmp = strtol(argv[0], &ep, 0);

		if (*ep != '\0')
		{
			printf("argc = %d, argv[0] = \"%s\"\n", argc, argv[0]);
			fatal("Invalid argument to swradio.");
		}

		switch (tmp)
		{
		case 0:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 1);
			pthread_register((Maddr)fb3,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb3[]", 2);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb4[i],
					sizeof(FloatBuffer), "fb4[i]", 3+i);
			}
			break;

		case 1:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 1);
			pthread_register((Maddr)fb3,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb3[]", 2);
			pthread_register((Maddr)fb4,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 11);
			break;

		case 2:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 1);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb3[i],
					sizeof(FloatBuffer), "fb3[i]", 3+i);
			}
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb4[i],
					sizeof(FloatBuffer), "fb4[i]", 3+i);
			}
			break;

		case 3:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 1);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb3[i],
					sizeof(FloatBuffer), "fb3[i]", 3+i);
			}
			pthread_register((Maddr)fb4,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 11);
			break;

		case 4:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 2);
			pthread_register((Maddr)fb3,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb3[]", 2);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb4[i],
					sizeof(FloatBuffer), "fb4[i]", 3+i);
			}
			break;

		case 5:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 2);
			pthread_register((Maddr)fb3,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb3[]", 2);
			pthread_register((Maddr)fb4,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 11);
			break;

		case 6:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 2);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb3[i],
					sizeof(FloatBuffer), "fb3[i]", 3+i);
			}
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb4[i],
					sizeof(FloatBuffer), "fb4[i]", 3+i);
			}
			break;

		case 7:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 2);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb3[i],
					sizeof(FloatBuffer), "fb3[i]", 3+i);
			}
			pthread_register((Maddr)fb4,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 11);
			break;

		case 8:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 1);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 1);
			pthread_register((Maddr)fb3,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb3[]", 2);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb4[i],
					sizeof(FloatBuffer), "fb4[i]", 3+i);
			}
			break;

		case 9:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 1);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 1);
			pthread_register((Maddr)fb3,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb3[]", 2);
			pthread_register((Maddr)fb4,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 11);
			break;

		case 10:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 1);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 1);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb3[i],
					sizeof(FloatBuffer), "fb3[i]", 3+i);
			}
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb4[i],
					sizeof(FloatBuffer), "fb4[i]", 3+i);
			}
			break;

		case 11:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 1);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 1);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb3[i],
					sizeof(FloatBuffer), "fb3[i]", 3+i);
			}
			pthread_register((Maddr)fb4,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 11);
			break;

		case 12:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 1);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 2);
			pthread_register((Maddr)fb3,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb3[]", 2);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb4[i],
					sizeof(FloatBuffer), "fb4[i]", 3+i);
			}
			break;

		case 13:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 1);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 2);
			pthread_register((Maddr)fb3,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb3[]", 2);
			pthread_register((Maddr)fb4,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 11);
			break;

		case 14:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 1);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 2);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb3[i],
					sizeof(FloatBuffer), "fb3[i]", 3+i);
			}
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb4[i],
					sizeof(FloatBuffer), "fb4[i]", 3+i);
			}
			break;

		case 15:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 1);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 2);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb3[i],
					sizeof(FloatBuffer), "fb3[i]", 3+i);
			}
			pthread_register((Maddr)fb4,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 11);
			break;

		/* Special case: map all fb's onto node 0 */
		case 16:
			pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
			pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 0);
			for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
			{
				pthread_register((Maddr)&fb3[i],
					sizeof(FloatBuffer), "fb3[i]", 0);
			}
			pthread_register((Maddr)fb4,
				sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 01);
			break;


		default:
			printf("argc = %d, argv[0] = \"%s\"\n", argc, argv[0]);
			fatal("Invalid argument to swradio.");
		}
	}
	else
	{
		pthread_register((Maddr)&fb1, sizeof(fb1), "fb1", 0);
		pthread_register((Maddr)&fb2, sizeof(fb2), "fb2", 0);
		pthread_register((Maddr)fb3,
			sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb3[]", 0);
		pthread_register((Maddr)fb4,
			sizeof(FloatBuffer)*NUM_SUPERPIPE_EQ, "fb4[]", 0);
	}

	out = (Chan *) calloc(1, sizeof(Chan));
	if (out == NULL)
	{
		fatal("Could not allocate memory for (Chan *) out\n");
	}
	CHAN_INIT(out)


	sinkout = (Chan *) calloc(1, sizeof(Chan));
	if (sinkout == NULL)
	{
		fatal("Could not allocate memory for (Chan *) sinkout\n");
	}
	CHAN_INIT(sinkout)


	/*							*/
	/*	Create first thread in the pipeline (lpf) as	*/
	/*	well as last thread (sink). The sink channel	*/
	/*	is global.					*/
	/*							*/
	PRE_WAIT_0(out)
	status = pthread_create(&tid, NULL, (void *)lpf, (void *)out);
	if (status < 0)
	{
		fatal("Could not create new thread\n");
	}
	POST_WAIT_0(out)

	PRE_WAIT_0(sinkout)
	status = pthread_create(&tid, NULL, (void *)sink, (void *)sinkout);
	if (status < 0)
	{
		fatal("Could not create new thread\n");
	}
	POST_WAIT_0(sinkout)


	fb1.rpos = fb1.rlen = 0;
	get_floats((FloatBuffer *)&fb1);

	/*									*/
	/*	LPF needs at least NUM_TAPS+1 inputs; get_floats is fine.	*/
	/*									*/
	printf("swradio-source sending [%d] bytes (init0) to LPF\n", sizeof(fb1));


	SEND(out)


	/*									*/
	/*	Since we are doing Call-By-Value, we must make this change 	*/
	/*	here. The corresponding calculation in run_lpf is a No-op	*/
	/*									*/
	fb1.rpos += DECIMATION + 1;


	/*	First bundle takes a while for LPF to process, and we also	*/
	/*	account for equalizer_init taking a long time.			*/
	usleep(0.5E6);

	/*									*/
	/*	run_demod needs 1 input, OK here. run_equalizer needs 51 	*/
	/*	inputs (same reason as for LPF).  This means running the 	*/
	/*	pipeline up to demod 50 times in advance: 			*/
	/*									*/
	for (i = 0; i < 64; i++)
	{
		if (fb1.rlen - fb1.rpos < NUM_TAPS + 1)
		{
			get_floats((FloatBuffer *)&fb1);
		}

		SEND(out)

		/*									*/
		/*	Since we are doing Call-By-Value, we must make this change 	*/
		/*	here. The corresponding calculation in run_lpf is a No-op	*/
		/*									*/
		fb1.rpos += DECIMATION + 1;
	
		/*	Wait until start of next sampling period	*/
		printf("(init) time left = [%ld]\n", PERIOD - (devrtc_getusecs() - lastsend));
		while ((devrtc_getusecs() - lastsend) < PERIOD)
		{
			sleep();
		}
		lastsend = devrtc_getusecs();
	}

	while (1)
	{
    		/*								*/
		/*	The low-pass filter will need NUM_TAPS+1 items; read 	*/
		/*	them if we need to. 					*/
		/*								*/
    		if (fb1.rlen - fb1.rpos < NUM_TAPS + 1)
		{
      			get_floats((FloatBuffer *)&fb1); 
		}

		/*	Now send fb1 to be LPF'd	*/
		printf("swradio-source sending [%d] bytes (steady) to LPF\n", sizeof(fb1));
		printf("fb1.rpos = [%d], fb1.rlen = [%d]\n", fb1.rpos, fb1.rlen);

		SEND(out)

		lprint("sentsample @ %dus", devrtc_getusecs());

		/*									*/
		/*	Since we are doing Call-By-Value, we must make this change 	*/
		/*	here. The corresponding calculation in run_lpf is a No-op	*/
		/*									*/
		fb1.rpos += DECIMATION + 1;


		/*	Wait until start of next sampling period	*/
		printf("(steady) time left = [%ld]\n", PERIOD - (devrtc_getusecs() - lastsend));
		while ((devrtc_getusecs() - lastsend) < PERIOD)
		{
			sleep();
		}
		lastsend = devrtc_getusecs();
	}

	return;		
}

void
get_floats(FloatBuffer *fb)
{
	fb_compact(fb);
  
	/*	Fill the remaining space in fb with 1.0.	*/
	while (fb->rlen < IN_BUFFER_LEN)
	{
		fb->buff[fb->rlen++] = 1.0;
	}

	return;
}



void
lpf(Chan *in)
{
	Chan		*out;
	pthread_t	tid;
	int		frames_rcvd = 0, status;


print("lpf stage created\n");
	out = (Chan *) calloc(1, sizeof(Chan));
	if (out == NULL)
	{
		fatal("Could not allocate memory for (Chan *) out\n");
	}
	CHAN_INIT(out)


	/*	Create first thread in the pipeline & wait	*/
	PRE_WAIT_0(out)
	status = pthread_create(&tid, NULL, (void *)demod, (void *)out);
	if (status < 0)
	{
		fatal("Could not create new thread\n");
	}
	POST_WAIT_0(out)



	/*	Release previous stage waiting on our birth:	*/
	POST_NATAL(in)
print("lpf stage done postnatal\n");



	fb2.rpos = fb2.rlen = 0;
	init_lpf_data(&lpf_data, CUTOFF_FREQUENCY, NUM_TAPS, DECIMATION);
	RECEIVE(in, run_lpf((void *)&fb1, (void *)&fb2, &lpf_data))

	/*									*/
	/*	run_demod needs 1 input, OK here. run_equalizer needs 51 	*/
	/*	inputs (same reason as for LPF).  This means running the 	*/
	/*	pipeline up to demod 50 times in advance: 			*/
	/*									*/
	while (frames_rcvd < NUM_TAPS)
	{
		RECEIVE(in, run_lpf((void *)&fb1, (void *)&fb2, &lpf_data))
		frames_rcvd++;
		SEND(out)
	}

	while (1)
	{
		RECEIVE(in, run_lpf((void *)&fb1, (void *)&fb2, &lpf_data))
		SEND(out)
	}

	return;		
}

void
run_lpf(FloatBuffer *fbin, FloatBuffer *fbout, LPFData *data)
{
	float sum = 0.0;
	int i = 0;


	if (fbin->rpos + data->taps - 1 >= fbin->rlen)
	{
		printf("WARNING: upcoming underflow in run_lpf()\n");
	}

	for (i = 0; i < data->taps; i++)
	{
		sum += fbin->buff[fbin->rpos + i] * data->coeff[i];
	}
	fbin->rpos += data->decimation + 1;
  
	/*								*/
	/*	Check that there's room in the output buffer; 		*/
	/*	move data if necessary.					*/
	/*								*/
	fb_ensure_writable(fbout, 1);
	fbout->buff[fbout->rlen++] = sum;


	return;
}

void
init_lpf_data(LPFData *data, float freq, int taps, int decimation)
{
	/*								*/
	/*	Assume that CUTOFF_FREQUENCY is non-zero. See		*/
	/*	comments in StreamIt LowPassFilter.java for origin.	*/
	/*								*/
	float w = 2 * M_PI * freq / SAMPLING_RATE;
	int i;
	float m = taps - 1.0;

	data->freq = freq;
	data->taps = taps;
	data->decimation = decimation;

	for (i = 0; i < taps; i++)
	{
		if (i - m/2 == 0.0)
		{
			data->coeff[i] = w / M_PI;
		}
    		else
      		{
			data->coeff[i] = sin(w * (i - m/2)) / M_PI / (i - m/2) *
        		(0.54 - 0.46 * cos(2 * M_PI * i / m));
		}
	}
	
	return;
}



void
demod(Chan *in)
{
	Chan		*out[NUM_SUPERPIPE_EQ];
	pthread_t	tid;
	int		frames_rcvd = 0, cur_eq = 0, i, status;


print("demod stage created\n");
	for (i = 0; i < NUM_SUPERPIPE_EQ; i++)
	{
		out[i] = (Chan *) calloc(1, sizeof(Chan));
		if (out[i] == NULL)
		{
			fatal("Could not allocate memory for (Chan *) out\n");
		}
		CHAN_INIT(out[i])

		/*	      EQ's index	*/
		out[i]->data = i;

		/*	Create EQ stage		*/
		PRE_WAIT_0(out[i])
		status = pthread_create(&tid, NULL, (void *)eq, (void *)out[i]);
		if (status < 0)
		{
			fatal("Could not create new thread\n");
		}
		POST_WAIT_0(out[i])
	}


	/*	Release previous stage waiting on our birth:	*/
	POST_NATAL(in)
print("demod stage done post-natal\n");


	/*									*/
	/*	run_demod needs 1 input, OK here. run_equalizer needs 51 	*/
	/*	inputs (same reason as for LPF).  This means running the 	*/
	/*	pipeline up to demod 50 times in advance: 			*/
	/*									*/
	while (frames_rcvd < NUM_TAPS)
	{
		RECEIVE(in, run_demod((void *)&fb2, (void *)&fb3))
		frames_rcvd++;
	}

	while (1)
	{
		RECEIVE(in, run_demod((void *)&fb2, (void *)&fb3[cur_eq]))
		SEND(out[cur_eq])
		cur_eq++;
		cur_eq %= NUM_SUPERPIPE_EQ;
	}

	return;		
}

void
run_demod(FloatBuffer *fbin, FloatBuffer *fbout)
{
	float temp, gain;


	gain = MAX_AMPLITUDE * SAMPLING_RATE / (BANDWIDTH * M_PI);
	temp = fbin->buff[fbin->rpos] * fbin->buff[fbin->rpos + 1];
	temp = gain * atan(temp);
	fbin->rpos++;
	fb_ensure_writable(fbout, 1);
	fbout->buff[fbout->rlen++] = temp;


	return;
}



void
eq(Chan *in)
{
	int	myindex;

print("EQ stage created\n");

	/*	Release previous stage waiting on our birth:	*/
	POST_NATAL(in)
	myindex = in->data;

print("EQ stage done postnatal, myindex = %d\n", myindex);

	init_equalizer(&eq_data);

	while (1)
	{
		RECEIVE(in, run_equalizer((void *)&fb3[myindex], (void *)&fb4[myindex], &eq_data))
		SEND(sinkout)
	}

	return;		
}

void
init_equalizer(EqualizerData *data)
{
	int i;
  
	/*								*/
	/*	Equalizer structure: there are ten band-pass filters,	*/
	/*	with cutoffs as shown below.  The outputs of these 	*/
	/*	filters get added together.  Each band-pass filter 	*/
	/*	is LPF(high)-LPF(low).					*/
	/*								*/
	for (i = 0; i < EQUALIZER_BANDS + 1; i++)
	{
		init_lpf_data(&data->lpf[i], eq_cutoffs[i], 64, 0);
	}

	/*		Also initialize member buffers.			*/
	for (i = 0; i < EQUALIZER_BANDS + 1; i++)
	{
		data->fb[i].rpos = data->fb[i].rlen = 0;
	}

	for (i = 0; i < EQUALIZER_BANDS; i++)
	{
		data->gain[i] = 1.0;
	}

	return;
}

void
run_equalizer(FloatBuffer *fbin, FloatBuffer *fbout, EqualizerData *data)
{
	int i, rpos;
	float sum = 0.0;

	/*								*/
	/*	Save the input read location; we can reuse the same 	*/
	/*	input data on all of the LPFs.				*/
	/*								*/
	rpos = fbin->rpos;
  
	/*	Run the child filters.		*/
	for (i = 0; i < EQUALIZER_BANDS + 1; i++)
	{
		fbin->rpos = rpos;
		run_lpf(fbin, &data->fb[i], &data->lpf[i]);
		lpf_out[i] = data->fb[i].buff[data->fb[i].rpos++];
	}

	/*								*/
	/*	Now process the results of the filters. Remember 	*/
	/*	that each band is output(hi)-output(lo).		*/
	/*								*/
	for (i = 0; i < EQUALIZER_BANDS; i++)
	{
		sum += (lpf_out[i+1] - lpf_out[i]) * data->gain[i];
	}

	/* Write that result.  */
	fb_ensure_writable(fbout, 1);
	fbout->buff[fbout->rlen++] = sum;

	return;
}



void
sink(Chan *in)
{
	int	nrcvd = 0;

print("sink stage created\n");

	/*	Release previous stage waiting on our birth:	*/
	POST_NATAL(in)

print("sink stage done postnatal\n");

	while (1)
	{
		RECEIVE(in, write_floats((void *)&fb4[nrcvd % NUM_SUPERPIPE_EQ]))
		nrcvd++;

		if (++nrcvd >= MAX_SAMPLES)
		{
			devlog_ctl("quit\n");
		}
	}

	return;		
}

void write_floats(FloatBuffer *fb)
{
	/*	printf() any data that's available:	*/
	while (fb->rpos < fb->rlen)
	{
		printf("%f\n", fb->buff[fb->rpos++]);
	}
}



void
fatal(char *error)
{
	printf("Swradio Error: \"%s\"\n", error);

	exit(-1);
}

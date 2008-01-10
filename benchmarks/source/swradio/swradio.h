/*									*/
/*	Adapted from							*/
/*		"fmref.c: C reference implementation of FM Radio	*/
/*		David Maze <dmaze@cag.lcs.mit.edu>			*/
/*		$Id: fmref.c,v 1.8 2002/07/29 14:05:48 aalamb Exp $"	*/
/*	by								*/
/*		pstanley@ece.cmu.edu					*/
/*									*/

#define SAMPLING_RATE		200000
#define CUTOFF_FREQUENCY	108000000
#define NUM_TAPS		64
#define MAX_AMPLITUDE		27000.0
#define BANDWIDTH		10000
#define DECIMATION		4

/* Must be at least NUM_TAPS+1: */
#define IN_BUFFER_LEN 128
#define EQUALIZER_BANDS 10

enum
{
	NUM_SUPERPIPE_EQ = 8
};

typedef struct FloatBuffer
{
	float 	buff[IN_BUFFER_LEN];
	int 	rpos;
	int	rlen;
} FloatBuffer;

typedef struct LPFData
{
	float	coeff[NUM_TAPS];
	float	freq;
	int	taps;
	int	decimation;
} LPFData;

typedef struct EqualizerData
{
	LPFData		lpf[EQUALIZER_BANDS + 1];
	FloatBuffer	fb[EQUALIZER_BANDS + 1];
	float		gain[EQUALIZER_BANDS];
} EqualizerData;

int	fb_ensure_writable(FloatBuffer *fb, int amount);
void	fb_compact(FloatBuffer *fb);
void	get_floats(FloatBuffer *fb);
void	init_equalizer(EqualizerData *data);
void	init_lpf_data(LPFData *data, float freq, int taps, int decimation);
void	run_demod(FloatBuffer *fbin, FloatBuffer *fbout);
void	run_equalizer(FloatBuffer *fbin, FloatBuffer *fbout, EqualizerData *data);
void	run_lpf(FloatBuffer *fbin, FloatBuffer *fbout, LPFData *data);
void	write_floats(FloatBuffer *fb);

/*
 * cmn.c -- Various forms of cepstral mean normalization
 *
 * **********************************************
 * CMU ARPA Speech Project
 *
 * Copyright (c) 1996 Carnegie Mellon University.
 * ALL RIGHTS RESERVED.
 * **********************************************
 * 
 * HISTORY
 * 
 * 19-Jun-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Changed to compute CMN over ALL dimensions of cep instead of 1..12.
 * 
 * 04-Nov-95	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Created.
 */


#include <assert.h>

#include <libutil/libutil.h>
#include "cmn.h"
#include <math.h>
#include <string.h>


/*
 * Normalize all cepstral vectors (except C0) by subtracting the mean for the
 * entire utterance.
 */
void norm_mean(float32 ** mfc, int32 n_frame, int32 veclen)
{
	static float32 *mean = 0, *var = 0;
	float *mfcp;
	int32 i, f, normvar;
	char *varnorm;

	varnorm = (char *) cmd_ln_access("-varnorm");
	if (strcmp(varnorm, "yes") == 0)
		normvar = 1;
	else
		normvar = 0;

	if (normvar)
		E_INFO("Normalizing utterance variance\n");

	assert((n_frame > 0) && (veclen > 0));

	if (mean == 0)
		mean = (float32 *) ckd_calloc(veclen, sizeof(float32));
	if (var == 0)
		var = (float32 *) ckd_calloc(veclen, sizeof(float32));

	for (i = 0; i < veclen; i++)
		mean[i] = var[i] = 0.0;

	for (f = 0; f < n_frame; f++) {
		mfcp = mfc[f];
		for (i = 0; i < veclen; i++)
			mean[i] += mfcp[i];
	}

	for (i = 0; i < veclen; i++)
		mean[i] /= n_frame;

	if (normvar) {
		for (f = 0; f < n_frame; f++) {
			mfcp = mfc[f];
			for (i = 0; i < veclen; i++)
				var[i] +=
				    (mfcp[i] - mean[i]) * (mfcp[i] -
							   mean[i]);
		}

		for (i = 0; i < veclen; i++)
			var[i] = (float) sqrt((double) var[i] / (double) n_frame);	/* std dev */
	}

	for (f = 0; f < n_frame; f++) {
		mfcp = mfc[f];
		for (i = 0; i < veclen; i++)
			mfcp[i] -= mean[i];
	}

	if (normvar) {
		for (f = 0; f < n_frame; f++) {
			mfcp = mfc[f];
			for (i = 0; i < veclen; i++)
				mfcp[i] /= var[i];
		}
	}
}

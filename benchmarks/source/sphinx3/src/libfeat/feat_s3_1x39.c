/*
 * feat_s3_1x39.c -- Cepstral feature stream; Sphinx3 version: single vector of
 * 		12 cep, 12 dcep, 3 pow, 12 ddcep.
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
 * 10-Jan-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Created.
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <assert.h>

#include "feat_s3_1x39.h"
#include <libutil/libutil.h>
#include <libio/libio.h>


#define N_FEAT			1

#define FEAT_CEPLEN		12
#define FEAT_DCEPLEN		FEAT_CEPLEN
#define FEAT_POWLEN		3
#define FEAT_D2CEPLEN		FEAT_CEPLEN

#define FEAT_DCEP_WIN		2


static int32 feat_size[1] = {
    FEAT_CEPLEN + FEAT_DCEPLEN + FEAT_POWLEN + FEAT_D2CEPLEN,
};


int32 feat_s3_1x39_cepsize ( int32 veclen )
{
    return (FEAT_CEPLEN+1);
}


int32 feat_s3_1x39_featsize (int32 **size)
{
    *size = feat_size;
    return N_FEAT;
}


/*
 * Feature vectors computed from input mfc vectors using this window (+/- window).
 */
int32 feat_s3_1x39_window_size ( void )
{
    return (FEAT_DCEP_WIN+1);
}


void feat_s3_1x39_cep2feat (float32 **mfc, float32 **feat)
{
    float32 *f;
    float32 *w, *_w;
    float32 *w1, *w_1, *_w1, *_w_1;
    float32 d1, d2;
    int32 i, j;
    
    /* CEP; skip C0 */
    memcpy (feat[0], mfc[0]+1, FEAT_CEPLEN * sizeof(float32));
    
    /*
     * DCEP: mfc[w] - mfc[-w], where w = FEAT_DCEP_WIN;
     */
    f = feat[0] + FEAT_CEPLEN;
    w  = mfc[ FEAT_DCEP_WIN] + 1;	/* +1 to skip C0 */
    _w = mfc[-FEAT_DCEP_WIN] + 1;

    for (i = 0; i < FEAT_CEPLEN; i++)
	f[i] = w[i] - _w[i];
    
    /* POW: C0, DC0, D2C0 */
    f += FEAT_CEPLEN;

    f[0] = mfc[0][0];
    f[1] = mfc[FEAT_DCEP_WIN][0] - mfc[-FEAT_DCEP_WIN][0];

    d1 = mfc[FEAT_DCEP_WIN+1][0] - mfc[-FEAT_DCEP_WIN+1][0];
    d2 = mfc[FEAT_DCEP_WIN-1][0] - mfc[-FEAT_DCEP_WIN-1][0];
    f[2] = d1 - d2;

    /* D2CEP: (mfc[w+1] - mfc[-w+1]) - (mfc[w-1] - mfc[-w-1]), where w = FEAT_DCEP_WIN */
    f += FEAT_POWLEN;
    
    w1   = mfc[ FEAT_DCEP_WIN+1] + 1;	/* Final +1 to skip C0 */
    _w1  = mfc[-FEAT_DCEP_WIN+1] + 1;
    w_1  = mfc[ FEAT_DCEP_WIN-1] + 1;
    _w_1 = mfc[-FEAT_DCEP_WIN-1] + 1;

    for (i = 0; i < FEAT_CEPLEN; i++) {
	d1 =  w1[i] -  _w1[i];
	d2 = w_1[i] - _w_1[i];

	f[i] = d1 - d2;
    }
}

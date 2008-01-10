/*
 * feat_1s_c.c -- Cepstral feature stream; Sphinx3 version: single vector of
 * 		13 cep
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

#include "feat_1s_c.h"
#include <libutil/libutil.h>
#include <libio/libio.h>


#define N_FEAT			1

static int32 feat_size[1];


int32 feat_1s_c_cepsize ( int32 veclen )
{
    feat_size[0] = veclen;
    return (veclen);
}


int32 feat_1s_c_featsize (int32 **size)
{
    *size = feat_size;
    return N_FEAT;
}


/*
 * Feature vectors computed from input mfc vectors using this window (+/- window).
 */
int32 feat_1s_c_window_size ( void )
{
    return (1);
}


void feat_1s_c_cep2feat (float32 **mfc, float32 **feat)
{
    memcpy (feat[0], mfc[0], feat_size[0] * sizeof(float32));

}

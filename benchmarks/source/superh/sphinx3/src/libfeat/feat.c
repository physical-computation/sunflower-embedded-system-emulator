/*
 * feat.c -- Cepstral features computation.
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
 * 03-Oct-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added unistd.h include.
 * 
 * 02-Oct-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added check for sf argument to s2mfc_read being within file size.
 * 
 * 18-Sep-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added sf, ef parameters to s2mfc_read().
 * 
 * 10-Jan-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Added feat_cepsize().
 * 		Added different feature-handling:
 *		    (s2_4x, s3_1x39, 1s_12c_12d_3p_12dd, 1s_c, 1s_c_d, 1s_c_d_dd).
 * 		Moved feature-dependent functions to feature-dependent files.
 * 
 * 09-Jan-96	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Moved constant declarations from feat.h into here.
 * 
 * 04-Nov-95	M K Ravishankar (rkm@cs.cmu.edu) at Carnegie Mellon University
 * 		Created.
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#if (_SUN4)
#include <unistd.h>
#endif
#include <string.h>
#include <assert.h>

#if (! WIN32)
#include <sys/file.h>
#include <sys/errno.h>
#include <sys/param.h>
#else
#include <fcntl.h>
#endif

#include <s3.h>

#include "feat.h"
#include "feat_s2_4x.h"
#include "feat_s3_1x39.h"
#include "feat_1s_c.h"
#include "feat_1s_c_d.h"
#include "feat_1s_c_d_dd.h"
#include "feat_1s_c_dd.h"
#include "feat_1s_c_wd_dd.h"
#include "feat_1s_c_d_cd.h"
#include "feat_1s_c_d_ld_dd.h"

#include <libutil/libutil.h>
#include <libio/libio.h>


volatile static struct {
    int32 (*cepsize)( int32 veclen );
    int32 (*featsize)(int32 **size);
    int32 (*window_size)( void );
    void  (*cep2feat)(float32 **mfc, float32 **feat);
} fcb = {NULL, NULL, NULL, NULL};	/* Feature control block */


int32 feat_cepsize ( int32 veclen )
{
    assert (fcb.cepsize != NULL);
    return (( *(fcb.cepsize) ) (veclen));
}


int32 feat_featsize (int32 **size)
{
    assert (fcb.featsize != NULL);
    return (( *(fcb.featsize) ) (size));
}


/*
 * Feature vectors computed from input mfc vectors using this window (+/- window).
 */
int32 feat_window_size ( void )
{
    assert (fcb.window_size != NULL);
    return (( *(fcb.window_size) ) ());
}


void feat_cep2feat (float32 **mfc, float32 **feat)
{
    assert (fcb.cep2feat != NULL);
    ( *(fcb.cep2feat) ) (mfc, feat);
}


void feat_init (char *type)
{
    E_INFO("Initializing feature stream to type: %s\n", type);
    
    if (strcmp (type, "s2_4x") == 0) {
	fcb.cepsize	= feat_s2_4x_cepsize;
	fcb.featsize	= feat_s2_4x_featsize;
	fcb.window_size = feat_s2_4x_window_size;
	fcb.cep2feat	= feat_s2_4x_cep2feat;
    } else if (strcmp (type, "s3_1x39") == 0 || 
			strcmp (type, "1s_12c_12d_3p_12dd") == 0) {
	fcb.cepsize	= feat_s3_1x39_cepsize;
	fcb.featsize	= feat_s3_1x39_featsize;
	fcb.window_size = feat_s3_1x39_window_size;
	fcb.cep2feat	= feat_s3_1x39_cep2feat;
    } else if (strcmp (type, "1s_c") == 0) {
	fcb.cepsize	= feat_1s_c_cepsize;
	fcb.featsize	= feat_1s_c_featsize;
	fcb.window_size = feat_1s_c_window_size;
	fcb.cep2feat	= feat_1s_c_cep2feat;
    } else if (strcmp (type, "1s_c_d") == 0) {
	fcb.cepsize	= feat_1s_c_d_cepsize;
	fcb.featsize	= feat_1s_c_d_featsize;
	fcb.window_size = feat_1s_c_d_window_size;
	fcb.cep2feat	= feat_1s_c_d_cep2feat;
    } else if (strcmp (type, "1s_c_d_dd") == 0) {
	fcb.cepsize	= feat_1s_c_d_dd_cepsize;
	fcb.featsize	= feat_1s_c_d_dd_featsize;
	fcb.window_size = feat_1s_c_d_dd_window_size;
	fcb.cep2feat	= feat_1s_c_d_dd_cep2feat;
    } else if (strcmp (type, "1s_c_d_cd") == 0) {
	fcb.cepsize	= feat_1s_c_d_cd_cepsize;
	fcb.featsize	= feat_1s_c_d_cd_featsize;
	fcb.window_size = feat_1s_c_d_cd_window_size;
	fcb.cep2feat	= feat_1s_c_d_cd_cep2feat;
    } else if (strcmp (type, "1s_c_wd_dd") == 0) {
	fcb.cepsize	= feat_1s_c_wd_dd_cepsize;
	fcb.featsize	= feat_1s_c_wd_dd_featsize;
	fcb.window_size = feat_1s_c_wd_dd_window_size;
	fcb.cep2feat	= feat_1s_c_wd_dd_cep2feat;
    } else if (strcmp (type, "1s_c_dd") == 0) {
	fcb.cepsize	= feat_1s_c_dd_cepsize;
	fcb.featsize	= feat_1s_c_dd_featsize;
	fcb.window_size = feat_1s_c_dd_window_size;
	fcb.cep2feat	= feat_1s_c_dd_cep2feat;
    } else if (strcmp (type, "1s_c_d_ld_dd") == 0) {
	fcb.cepsize	= feat_1s_c_d_ld_dd_cepsize;
	fcb.featsize	= feat_1s_c_d_ld_dd_featsize;
	fcb.window_size = feat_1s_c_d_ld_dd_window_size;
	fcb.cep2feat	= feat_1s_c_d_ld_dd_cep2feat;
    } else
	E_FATAL("Unknown feature type: %s\n", type);
}


#define MAX_MFCBUFSIZE	S3_MAX_FRAMES
static int32 mfcbufsize = 0;	/* #Frames of mfcbuf currently allocated */
static float32 **mfcbuf;


/*
 * Read specified segment [sf..ef] of Sphinx-II format mfc file read and return
 * #frames read.  Return -1 if error.
 */
int32 s2mfc_read (char *file, int32 sf, int32 ef, float32 ***mfc, int32 veclen)
{
    FILE *fp;
    int32 n_float32;
    struct stat statbuf;
    int32 i, n, byterev, cepsize;
    char tmp;
    
    E_INFO("Reading mfc file: %s\n", file);
    
    assert (fcb.cepsize != NULL);
    cepsize = ( *(fcb.cepsize) ) (veclen);
    
    /* Find filesize; HACK!! To get around intermittent NFS failures, use stat_retry */
    if (stat_retry (file, &statbuf) < 0) {
	E_ERROR("stat_retry(%s) failed\n", file);
	return -1;
    }

    if ((fp = fopen(file, "rb")) == NULL) {
	E_ERROR("fopen(%s,rb) failed\n", file);
	return -1;
    }
    
    /* Read #floats in header */
    if (fread_retry (&n_float32, sizeof(int32), 1, fp) != 1) {
	fclose (fp);
	return -1;
    }
    

    /* Check if n_float32 matches file size */
/* pip: this is a hack to get it to run on simulator:
	we always know the endianess of the sim (big-e),
	so always swap_int32

    byterev = FALSE;
    if ((n_float32*sizeof(float32) + 4) != statbuf.st_size) {
	n = n_float32;
	SWAP_INT32(&n);

	if ((n*sizeof(float32) + 4) != statbuf.st_size) {
	    E_ERROR("Header size field: %d(%08x); filesize: %d(%08x)\n",
		    n_float32, n_float32, statbuf.st_size, statbuf.st_size);
	    fclose (fp);
	    return -1;
	}

	n_float32 = n;
	byterev = TRUE;
    }
pip */
/* pip : */ byterev = FALSE;


    if (n_float32 <= 0) {
	E_ERROR("Header size field: %d\n",  n_float32);
	fclose (fp);
	return -1;
    }
    
	E_INFO("pip: n = %d, n_float32 = %d, cepsize = %d, veclen = %d\n", n, n_float32, cepsize, veclen);
	E_INFO("pip: forcing cepsize to feat_s3_1x39_cepsize()\n");
	cepsize = ((*feat_s3_1x39_cepsize))(veclen);
	E_INFO("pip again: n = %d, n_float32 = %d, cepsize = %d\n", n, n_float32, cepsize);
	E_INFO("pip: forcing cepsize to 13\n");
	cepsize = 13; 
	E_INFO("pip again: n = %d, n_float32 = %d, cepsize = %d\n", n, n_float32, cepsize);


    /* n = #frames of input */
    n = n_float32/cepsize;
    if (n * cepsize != n_float32) {
	E_ERROR("Header size field: %d; not multiple of %d\n", n_float32, cepsize);
	fclose (fp);
	return -1;
    }
    if (sf > 0) {
	if (sf >= n) {
	    E_ERROR("Start frame (%d) beyond MFC file size (%d)\n", sf, n);
	    fclose (fp);
	    return -1;
	}
	n -= sf;
    }

    /* Limit n if indicated by [sf..ef] */
    if ((ef-sf+1) < n)
	n = (ef-sf+1);

    if (n > mfcbufsize) {
	if (mfcbufsize > 0)
	    E_FATAL("Increase MAX_MFCBUFSIZE\n");

	mfcbufsize = MAX_MFCBUFSIZE;
	mfcbuf = (float32 **) ckd_calloc_2d (mfcbufsize, cepsize, sizeof(float32));
    }
    
    if (sf > 0)
	fseek (fp, sf*cepsize*sizeof(float32), SEEK_CUR);
    
    /* Read mfc data and byteswap if necessary */
    n_float32 = n * cepsize;
    if (fread_retry (mfcbuf[0], sizeof(float32), n_float32, fp) != n_float32) {
	E_ERROR("Error reading mfc data\n");
	fclose (fp);
	return -1;
    }
    if (byterev) {
	for (i = 0; i < n_float32; i++)
	    SWAP_FLOAT32(&(mfcbuf[0][i]));
    }

    fclose (fp);
    
    *mfc = mfcbuf;
    
    return n;
}

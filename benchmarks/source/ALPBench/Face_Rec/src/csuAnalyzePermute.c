/*
 * 
 * This file is part of the ALPBench Benchmark Suite Version 1.0
 * 
 * Copyright (c) 2005 The Board of Trustees of the University of Illinois
 * 
 * All rights reserved.
 * 
 * ALPBench is a derivative of several codes, and restricted by licenses
 * for those codes, as indicated in the source files and the ALPBench
 * license at http://www.cs.uiuc.edu/alp/alpbench/alpbench-license.html
 * 
 * The multithreading and SSE2 modifications for SpeechRec, FaceRec,
 * MPEGenc, and MPEGdec were done by Man-Lap (Alex) Li and Ruchira
 * Sasanka as part of the ALP research project at the University of
 * Illinois at Urbana-Champaign (http://www.cs.uiuc.edu/alp/), directed
 * by Prof. Sarita V. Adve, Dr. Yen-Kuang Chen, and Dr. Eric Debes.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal with the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimers.
 * 
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimers in the documentation and/or other materials provided
 *       with the distribution.
 * 
 *     * Neither the names of Professor Sarita Adve's research group, the
 *       University of Illinois at Urbana-Champaign, nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this Software without specific prior written permission.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE CONTRIBUTORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH THE
 * SOFTWARE.
 * 
 */


//pip: copied in a "good" basename from OpenBSD, modified to fit 
//simulation env, with the ff defines

#define	MAXPATHLEN	1024
#define ENAMETOOLONG	1024
#define	NULL		((void *)0)


/*	$OpenBSD: strlcpy.c,v 1.4 1999/05/01 18:56:41 millert Exp $	*/

/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */



#include <sys/types.h>
#include <string.h>

/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t strlcpy(dst, src, siz)
	char *dst;
	const char *src;
	size_t siz;
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0 && --n != 0) {
		do {
			if ((*d++ = *s++) == 0)
				break;
		} while (--n != 0);
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}



/* OPENBSD ORIGINAL: lib/libc/gen/basename.c */

/*	$OpenBSD: basename.c,v 1.11 2003/06/17 21:56:23 millert Exp $	*/

/*
 * Copyright (c) 1997 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

char *
basename(const char *path)
{
	static char bname[MAXPATHLEN];
	register const char *endp, *startp;

	/* Empty or NULL string gets treated as "." */
	if (path == NULL || *path == '\0') {
		(void)strlcpy(bname, ".", sizeof bname);
		return(bname);
	}

	/* Strip trailing slashes */
	endp = path + strlen(path) - 1;
	while (endp > path && *endp == '/')
		endp--;

	/* All slashes become "/" */
	if (endp == path && *endp == '/') {
		(void)strlcpy(bname, "/", sizeof bname);
		return(bname);
	}

	/* Find the start of the base */
	startp = endp;
	while (startp > path && *(startp - 1) != '/')
		startp--;

	if (endp - startp + 2 > sizeof(bname)) {
		//pip: errno has no meaning in sim environ 
		//	errno = ENAMETOOLONG;
		return(NULL);
	}
	strlcpy(bname, startp, endp - startp + 2);
	return(bname);
}



/*
Copyright (c) 2003 Colorado State University

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
//pip#include <libgen.h>

#include <csuCommon.h>

#define OPENNING "Sample Recognition Rate Distributions from swapped Gallery and Probe Sets."

/*
   ============================================================================
 Copyright 2002, Colorado State University. All Rights Reserved.           
 --------------------------------------------------------------------------- 
 Permission to use, copy or modify this software and its documentation for   
 educational and research purposes only, provided that this copyright notice  
 appear on all copies and supporting documentation.                          
 ---------------------------------------------------------------------------        
 File:    csuPermute.c                                                     
 Authors: J. Ross Beveridge, Kai She, David Bolme                                        
 Date:    May 24, 2002                                                  
   ============================================================================
*/

/* ===========================================================================
 Compile time configuration optiosn     
*/

#define PRINT_LEVEL 2        /* Zero for a quiet program - nothing to stdout */
#define MAX_FSTRLEN 100      /* Upper bound on file name string lengths */
#define ITERATIONS 10000     /* How many times to sample gallery and probe sets, normally 10000 */


/* ===========================================================================
 Structures user here are:
  FERETID
  FIDPR
  PGSET
*/

/*
 The FERETID structure is used to facilitate most of the operations 
 performed in this file.  It contains critical information about the FERET 
 subject derived from the name. It is also used to carry back pointer information 
 for each test image. Specifically, the local subject index, running from 0 
 to the numSubjects-1, and the local replicate index, running from 0 to 
 numReplicates-1.  
*/

typedef struct feretid {
    char* text;     /* The text name designating FERET image distance file */
    /* char* type; */ /* The type of the image, fa, fb, etc. */
    int subject; /* The unique subject identifier */
    /* int day; */  /* The day of the month the image was collected */
    /* int month; */ /* The month the image was collected */
    /* int year; */ /* The year the image was collected */
    int index;      /* The local index into distance matrix */
    int si;   /* The local subject index */
    int ri;   /* The local replicate index */
}
FERETID;

/*
 The FIDPR, or FERERTID Pair, is a simple struct to maintain points to two
 instances of FERETIDs as well as a locally cached copy of the distance 
 between them. This simplifies sorting lists of paired images.
*/
typedef struct fidpr {
    FERETID** ids;   /* Will become a two element array, the ith and jth FERETID */
    float dist;   /* The distance between them drawn out of the distance array */
}
FIDPR;

/* The PGSET struct provides a single clean place to store information about
 a particular randomized selection of probe and gallery images. More is said
 below about its use. */

typedef struct pgset {
    int* gallery; /* Gallery replicate ri stored for each subject index si */
    int* probes;    /* Probe set replicate ri stored for each subject index si */
    int* gMarker;   /* One if image index is in gallery, zero otherwise */
    int* pMarker; /* One if image index is in gallery, zero otherwise */
    int* recCount;  /* An integer array to record recognition counts */
}
PGSET;

typedef struct {
    char* basename;
    FIDPR*** fidprs;
    int** recCounts;
    int** hist;
} AlgorithmData;



/*
 The next set of functions parse a FERET name and create an instance of a
 FERETID struct to represent that image. This section also includes code
 for the FIDPR struct.
*/


/*
 Create a new instance of the FERETID structure.  Parses the name to derive 
 the subject ID, the image type and the date.   The index passsed in is 
 used to record the local index of this particular FERETID. This index is 
 the index into the distance matrix for this particular image. When the 
 original set of image names are read from the test image names file, each 
 is given a unique local index such that the indices run from 0 to 
 numImages-1.
*/
FERETID* parseFERETID (char *name, int index, int subject) {
    FERETID* fid;
    char *tt;

    fid = (FERETID*) malloc(sizeof(FERETID));
    assert(fid);
    tt = (char*) malloc(sizeof(char) * MAX_FSTRLEN);
    assert(tt);

    strcpy(tt, name);
    fid->text = tt;
    fid->index = index;
    fid->subject = subject;
    return fid;
}


FIDPR* makeFIDPR(FERETID* fid0, FERETID* fid1, float** dist) {
    FIDPR* pr;
    FERETID** ids;

    pr = (FIDPR*) malloc(sizeof(FIDPR));
    assert(pr);
    ids = (FERETID**) malloc(sizeof(FERETID*) * 2);
    assert(ids);
    pr->ids = ids;
    pr->ids[0] = fid0;
    pr->ids[1] = fid1;
    pr->dist = dist[fid0->index][fid1->index];
    return pr;
}

/* This is the function used by quick sort to sort feret image pairs */
static int compareFIDPRs(const void *f1, const void *f2) {
    FIDPR **fidpr1 = (FIDPR**) f1;
    FIDPR **fidpr2 = (FIDPR**) f2;
    float d1 = (*fidpr1)->dist;
    float d2 = (*fidpr2)->dist;
    if (d1 > d2)
        return 1;
    if (d2 > d1)
        return -1;
    return 0;
}


/*
 Process test image names by building the master list of FERETIDs and 
 the subject replicates table.
*/

/* Given the array of FERETIDs, return the largest FERET subject index.
    This is extremely useful because we will want to built integer arrays
    that count how many times a subject appears in the test image set. */
int maxSubjectIndex( FERETID** fids, int numImages) {
    int i, max;

    max = 0;
    for (i = 0; i < numImages; i++) {
        if (fids[i]->subject > max)
            max = fids[i]->subject;
    }
    return max;
}

/* For each FERET subject ID, count up how many images are in the test
 image set.  These are the replicates from which probe and galleries 
 will be constructed. While doing this, it is convenient to record
 what will become the local subject index and the local replicate index, 
 i.e. FERETID fields si and ri.  This is a good place to do this 
 operation, because by going through subject ID in ascending order, 
 and assigning each a unique local index, the resulting table (si, ri) 
 will be sorted from least to greatest FERET subject id. 
 
 This function returns a count array indicating how many replicates
 appear for each subject. A zero indicates that there is a gap in the
 FERET subject ids, and zeroes can be expected. */
int* subjectCounts(FERETID** fids, int numImages, int *subjectIndexBound) {
    int i, j, max, siLocal;
    int *subCounts, *subIndex;
    siLocal = 0;
    max = maxSubjectIndex(fids, numImages);
    subCounts = (int*) malloc(sizeof(int) * (max + 1));
    assert(subCounts);
    subIndex = (int*) malloc(sizeof(int) * (max + 1));
    assert(subCounts);
    for (i = 0; i < (max + 1); i++)
        subCounts[i] = 0;
    for (i = 0; i < (max + 1); i++) {
        for (j = 0; j < numImages; j++) {
            if (i == fids[j]->subject) {
                if (subCounts[i] == 0) {
                    subIndex[i] = siLocal++;
                }
                fids[j]->si = subIndex[i];
                fids[j]->ri = subCounts[i];
                subCounts[i]++;
            }
        }
    }
    *subjectIndexBound = max + 1;
    return subCounts;
}

/* Find the largest number of replicates that appear for any subject */
int maxReplicateCount(int* subCounts, int n) {
    int i, max;
    max = 0;
    for (i = 0; i < n; i++) {
        if (max < subCounts[i]) {
            max = subCounts[i];
        }
    }
    return max;
}


FERETID*** subjectReplicatesTable(ImageList **iml, int *numImages, int *numSubjects, int *numReplicates, FERETID ***fids) {
    int i, s, r, expectedReplicates;
    FERETID*** srt;
    ImageList *subject, *replicate;

    *numImages = 0;
    *numSubjects = 0;
    expectedReplicates = 0;
    /*  First pass through subject replicates list to determine how many subject, how many replicates,
        and that there are the same number of replicates for each subject */
    for (subject = *iml; subject; subject = subject->next_subject) {
        (*numSubjects)++;
        (*numReplicates) = 0;
        for (replicate = subject; replicate; replicate = replicate->next_replicate) {
            (*numImages)++;
            (*numReplicates)++;
        }
        if ((*numSubjects != 1) && ((*numReplicates) != expectedReplicates) ) {
            fprintf(stderr, "Error, encountered different number of replicates for subject %s\n", subject->filename);
            /* printout debug info and exit */
            exit(1);
        }
        expectedReplicates = (*numReplicates);
    }
    /* Now build the subject replicates table as a 2D array and populate it with FERET IDs */
    srt = (FERETID***) malloc(sizeof(FERETID**) * (*numSubjects));
    assert(srt);
    for (i = 0; i < *numSubjects; i++) {
        srt[i] = (FERETID**) malloc(sizeof(FERETID*) * (*numReplicates));
        assert(srt[i]);
    }

    /* Now build the feret id array */
    (*fids) = (FERETID**) malloc(sizeof(FERETID*) * (*numImages));
    assert(fids);

    i = 0;
    s = 0;
    for (subject = *iml; subject; subject = subject->next_subject) {
        r = 0;
        for (replicate = subject; replicate; replicate = replicate->next_replicate) {
            srt[s][r] = parseFERETID(replicate->filename, i, s);
            (*fids)[i++] = srt[s][r];
            r++;
        }
        s++;
    }
    return srt;
}


/*
 The function readDistances uses the function getLocalIndex to determine
 the local index of the image for which a distance value is currently being
 read from file.
 
 getLocalIndex is given an image name, it then performs an exhaustive search
 of the subject replicate table to find a match.  If the match is found the
 local index of the image is returned.  Otherwise -1 is returned to indicate no
 match was found.
 
 */

int getLocalIndex(char *name, FERETID*** srt, int numReplicates, int maxSub) {
    int i, j;
    for (i = 0; i < maxSub; i++) {
        for (j = 0; j < numReplicates; j++) {
            if (!strcmp(name, srt[i][j]->text)) {
                return srt[i][j]->index;
            }
        }
    }

    /* name not found so return -1 */
    return -1;
}

/* This procedure iterates through the list of subject names opening the
 corresponding file of distances to other images in the data directory.
 Note that the subject names ARE the file names for the sake of this code.
 When reading a given data file, each record is assumed to contain two
 entries: the name of the other image and the distance to this other 
 image. The code is not robust with respect to changes in this format.
 
 This code is robust with respect to placing the distance into the distance
 matrix based upon the image name. So, entries corresponding to images not
 in the test image file are ignored. Also, if the order of the images in
 the distance file is permuted, this code will still behave properly.
*/


float** readDistances(FERETID** fids, FERETID*** srt, int numImages, int numReplicates, int numSubjects, char *dir) {
    int i, li;
    char buffer[100];
    char *bn, *bv, *name;
    float **dist;
    FILE *f;

    name = (char*) malloc(sizeof(char) * MAX_FSTRLEN);
    bn = (char*) malloc(sizeof(char) * MAX_FSTRLEN);
    bv = (char*) malloc(sizeof(char) * MAX_FSTRLEN);

    dist = (float **) malloc(numImages * sizeof(float*));
    assert(dist);
    for (i = 0;i < numImages; i++) {
        dist[i] = (float*) malloc(numImages * sizeof(float));
        assert(dist[i]);
    }
    fflush(stdout);
    for (i = 0; i < numImages; i++) {
        sprintf(name, "%s/%s", dir, fids[i]->text);
        f = fopen(name, "r");
        if (f == NULL) {
            printf("\n Cannot open %s to read!\n", name);
            exit(1);
        }
        while (fgets(buffer, 100, f)) {
            sscanf(buffer, "%s %s", bn, bv);
            li = getLocalIndex(bn, srt, numReplicates, numSubjects);
            if (li != -1) {
                dist[i][li] = atof(bv);
                /** printf("\n Distance entry %d %d is %f.", i, li, dist[i][li]);*/
            }
        }
        fclose(f);
    }
    return dist;
}


/*
 For each test image, build a sorted array of paired FERET ids, one for 
 test images, and the other for all other test images. 
*/
FIDPR*** buildSortedPairedMatrix(FERETID** fids, int numImages, float** dist) {
    int i, j;
    FIDPR*** fidprs;

    fidprs = (FIDPR***) malloc(sizeof(FIDPR**) * numImages);
    assert(fidprs);
    for (i = 0; i < numImages; i++) {
        fidprs[i] = (FIDPR**) malloc(sizeof(FIDPR*) * numImages);
        assert(fidprs);
    }

    for (i = 0; i < numImages; i++) {
        for (j = 0; j < numImages; j++) {
            fidprs[i][j] = makeFIDPR(fids[i], fids[j], dist);
        }
    }

    for (i = 0; i < numImages; i++) {
        qsort(fidprs[i], numImages, sizeof(FIDPR*), compareFIDPRs);
    }

    return fidprs;
}

void printPairedMatrix( FIDPR*** fidprs, int numImages) {
    int i, j;
    FIDPR* fidpr;
    FERETID* fid0;
    FERETID* fid1;
    for (i = 0; i < numImages; i++) {
        printf("\n");
        for (j = 0; j < numImages; j++) {
            fidpr = fidprs[i][j];
            fid0 = fidpr->ids[0];
            fid1 = fidpr->ids[1];
            printf(" <%d %d %d %d>", fid0->si, fid0->ri, fid1->si, fid1->ri);
        }
    }
    for (i = 0; i < numImages; i++) {
        printf("\n");
        for (j = 0; j < numImages; j++) {
            printf(" %5.0f", fidprs[i][j]->dist);
        }
    }
}

/*
 Generate subsets of test images to serve as gallery and probe sets
 
 These choices are stored in a PGSET struct that keeps track of the 
 gallery images and the probe images using two different representations.
 The first is an array indexed by local subject id, the same as the srt
 table, and it indicates which replicate is in the set for each subject.
 The second is a marker array indexed by image index, running from 0 to 
 numImages, and a one in the marker array indicates inclusion of the 
 image in the probe/gallery set. This second form is convenient for
 controlling selection and counting while iterating through the complete
 set of test images.
 
 The PGDSET structure is intended to be reused: creating a new one for 
 each of thousands of random choices of probe and gallery sets would be 
 slow at best and proably exhaust memory.
 */

PGSET* makePGSET(int numImages, int numSubjects) {
    PGSET* pgs = (PGSET*) malloc(sizeof(PGSET));
    assert(pgs);

    pgs->gallery = (int*) malloc(sizeof(int) * numSubjects);
    assert(pgs->gallery);
    pgs->probes = (int*) malloc(sizeof(int) * numSubjects);
    assert(pgs->probes);
    pgs->gMarker = (int*) malloc(sizeof(int) * numImages);
    assert(pgs->gMarker);
    pgs->pMarker = (int*) malloc(sizeof(int) * numImages);
    assert(pgs->pMarker);

    return pgs;
}

/* The next two functions are very convenient for debugging, but not
 used in the mature released code. Of course, they are kept around 
 because, well debugging may become relevant again :-)
*/
void listPGSETimages1(PGSET* pgs, FERETID** fids, int numImages) {
    int i, c;
    c = 0;
    for (i = 0; i < numImages; i++) {
        if (pgs->gMarker[i]) {
            printf("\n Gallery image %d is %s.", c++, fids[i]->text);
        }
    }
    c = 0;
    for (i = 0; i < numImages; i++) {
        if (pgs->pMarker[i]) {
            printf("\n Probe image %d is %s.", c++, fids[i]->text);
        }
    }
}

void listPGSETimages2(PGSET* pgs, FERETID*** srt, int numSubjects) {
    int si, ri;
    FERETID* gfid, *pfid;

    for (si = 0; si < numSubjects; si++) {
        ri = pgs->gallery[si];
        gfid = srt[si][ri];
        ri = pgs->probes[si];
        pfid = srt[si][ri];
        printf("\n  Gallery %s \t Probe %s", gfid->text, pfid->text);
    }
}



/**
 The randomized selection of probe and gallery images is accomplished by
 shuffling the local subject indices in the srt table and then selecting
 from the columns of this matrix using a fixed pattern. The pattern grows
 in length as the number of replicates increase. So, for example, if
 numReplicates = 2
       r1  r2
   1   p   g
   2   g   p
 For 3 replicates
       r1  r2  r3
   1   p   g   -
   2   p   -   g
   3   g   p   -
   4   -   p   g
   5   g   -   p
   6   -   g   p
 For 4 replicates
       r1  r2  r3  r4
   1   p   g   -   -
   2   p   -   g   -
   3   p   -   -   g
   4   g   p   -   -
   5   -   p   g   -
   6   -   p   -   g
   7   g   -   p   -
   8   -   g   p   -
   9   -   -   p   g
  10   g   -   -   p
  11   -   g   -   p
  12   -   -   g   p
*/

void fullBalancePattern(int* p, int* g, int numReplicates) {
    int i, pc, gc;

    i = 0;
    for (pc = 0; pc < numReplicates; pc++) {
        for (gc = 0; gc < numReplicates; gc++) {
            if (pc != gc) {
                p[i] = pc;
                g[i] = gc;
                i++;
            }
        }
    }
}

/*
    The repetive pattern of selection from the columns of the srt matrix are
 captured in the probe pattern, pp, and gallery pattern, gp arrays.  These
 are then used in conjunction with a shuffled set of indices into the rows
 of the srt matrix to gather the probe and gallery sets.
*/
void balancedSelectionProbeGallery(PGSET* pgs, FERETID*** srt, int numImages,
                                   int numSubjects, int numReplicates) {
    int i, si, ri;
    int *shuff;
    int *pp, *gp;
    int runLen;

    runLen = numReplicates * (numReplicates - 1);
    pp = (int*) malloc(sizeof(int) * (runLen));
    assert(pp);
    gp = (int*) malloc(sizeof(int) * (runLen));
    assert(gp);
    fullBalancePattern(pp, gp, numReplicates);

    shuff = shuffle(numSubjects);
    for (i = 0; i < numSubjects; i++) {
        pgs->gallery[shuff[i]] = gp[i % runLen];
        pgs->probes[shuff[i]] = pp[i % runLen];
    }

    for (i = 0; i < numImages; i++) {
        pgs->gMarker[i] = 0;
        pgs->pMarker[i] = 0;
    }

    for (si = 0; si < numSubjects; si++) {
        ri = pgs->gallery[si];
        i = srt[si][ri]->index;
        pgs->gMarker[i] = 1;
        ri = pgs->probes[si];
        i = srt[si][ri]->index;
        pgs->pMarker[i] = 1;
    }
}

/*
 Given a probe and gallery set selection and the test images sorted
 by ascending distance, compute the recognition counts at ranks 1 through
 the upper bound specified.  Recognition count is the raw number correctly
 recognized at a given rank. It may latter be turned into a percentage, 
 i.e. a rank.
 
 As an intermediate step, the recognition rank is determined for each of the
 probe images.  As a nod to efficiency in this particular code, the arrays
 used to store recognition rank and recognition rate are passed as
 arguments.  
 
 Recognition rank is indexed from zero to numSubjects minus one.  Each entry
 representing the recognition rank for a distinct probe image.  Recognition
 Count is indexed from one to numSubjects inclusive.  The zero position
 represents the number of images correctly recognized at rank one, and the
 first position rank two.  etc.  While the logic computes it, note that the
 numSubjects position must have a constant value: the number of subjects.
*/

int* makeRecRanks(int numSubjects) {
    int* r;
    r = (int*) malloc(sizeof(int) * (numSubjects));
    assert(r);
    return r;
}

int* makeRecCount(int numSubjects) {
    int* r;
    r = (int*) malloc(sizeof(int) * (numSubjects + 1));
    assert(r);
    return r;
}

/* This two dimensional array of recognition counts is indexed by the
 iteration number t and the rank.  It is large, but it also represents
 the complete summary of the performance over the iterations. Finally,
 it will be used to build histograms of the recognition count at rank 1,
 rank 2, etc.
*/

int** makeRecCounts(int iterations, int numSubjects) {
    int **r, t;
    r = (int**) malloc(sizeof(int*) * iterations);
    assert(r);
    for (t = 0; t < iterations; t++) {
        r[t] = (int*) malloc(sizeof(int) * (numSubjects + 1));
        assert(r[t]);
    }
    return r;
}

void printRecCount(int* recCount, int max) {
    int i;
    for (i = 1; i <= max; i++) {
        printf("\n  Rec Count at rank %2d is %d.", i + 1, recCount[i]);
    }
}

/*  The recognition rank is found by searching down the sorted list of
 FERETID Pairs until a match is found between the probe and the gallery
 subject.  Here the first index of ids in the pair represents the probe
 and the second represents the gallery. Thus, the probe subject ID can
 be taken from any of the entries: it is taken from the first for 
 convenience. 
*/

int recognitionRank(FIDPR** gal, int* marker, int numImages) {
    int i, count, subject, match;
    subject = gal[0]->ids[0]->subject;
    count = 0;
    i = 0;
    match = 0;
    while (!match) {
        if (marker[gal[i]->ids[1]->index]) {
            if (gal[i]->ids[1]->subject == subject) {
                match = 1;
            }
            count++;
        }
        i++;
        if (i > numImages) {
            printf("Error: somehow failed to match subjects in function recognitionRank.");
            exit(1);
        }
    }
    return count;
}

void loadRecognitionCounts(PGSET* pgs, FIDPR*** fidprs, int* recRanks, int* recCount,
                           int numImages, int numSubjects) {
    int i, pi, rank, count;

    pi = 0;
    for (i = 0; i < numImages; i++) {
        if (pgs->pMarker[i]) {
            recRanks[pi] = recognitionRank(fidprs[i], pgs->gMarker, numImages);
            /* printf("\n Recognition rank for probe %d is %d.", pi, recRanks[pi]); */
            pi++;
        }
    }
    i = 0;
    for (rank = 1; rank <= numSubjects; rank++) {
        count = 0;
        for (i = 0; i < numSubjects; i++) {
            if (recRanks[i] <= rank)
                count++;
        }
        recCount[rank] = count;
    }
}

/* ====================================================================
 Histograms represent the sample distributions for recognition Count, 
 or after normalization, rate.  There is one histogram built for each
 recognition rank from one to the max rank passed in.
*/

int** histogramRecognitionCounts(int** recCounts, int iterations, int numSubjects) {
    int i, rank, count, rc;
    int** hist;
    int* h;
    hist = (int**) malloc(sizeof(int*) * (numSubjects + 1));
    assert(hist);
    for (i = 0; i <= numSubjects; i++) {
        hist[i] = (int*) malloc(sizeof(int) * (numSubjects + 1));
        assert(hist[i]);
    }
    for (rank = 1; rank <= numSubjects; rank++) {
        for (count = 0; count <= numSubjects; count++) {
            hist[rank][count] = 0;
        }
    }
    for (rank = 1; rank <= numSubjects; rank++) {
        h = hist[rank];
        for (i = 0; i < iterations; i++) {
            rc = recCounts[i][rank];
            h[rc]++;
        }
    }
    return hist;
}

int** histogramCompareCounts(int** recCounts1, int** recCounts2, int iterations, int numSubjects) {
    int i, rank, count, rc;
    int** hist;
    int* h;
    int zero = numSubjects + 1;
    hist = (int**) malloc(sizeof(int*) * (numSubjects + 1));
    assert(hist);
    for (i = 0; i <= numSubjects; i++) {
        hist[i] = (int*) malloc(sizeof(int) * (2*numSubjects + 1));
        assert(hist[i]);
    }
    for (rank = 1; rank <= numSubjects; rank++) {
        for (count = -numSubjects; count <= numSubjects; count++) {
            hist[rank][zero+count] = 0;
        }
    }
    for (rank = 1; rank <= numSubjects; rank++) {
        h = hist[rank];
        for (i = 0; i < iterations; i++) {
            rc = recCounts1[i][rank]-recCounts2[i][rank];
            h[zero+rc]++;
        }
    }
    return hist;
}

void printHistogram(int** hist, int rank, int numSubjects) {
    int rc;
    int sum;
    sum = 0;
    printf("\nHistgram of recognition counts at rank %d", rank);
    for (rc = 0; rc <= numSubjects; rc++) {
        printf("\n  %3d \t %d", rc, hist[rank][rc]);
        sum = sum + hist[rank][rc];
    }
    printf("\n  Sums to %d", sum);
}

/* Determine the mode, i.e. the high point, on the histograms for all rank
 values and return these in an integer array indexed from 1 to numSubjects
 inclusive. Also determine the lower bound of the confidence interval, 
 the upper bound, and the mean recognition rate. */
int* recRateMode(int** hist, int numSubjects) {
    int rank, rr, maxrr, mode = 0;
    int* m = (int*) malloc(sizeof(int) * (numSubjects + 1));
    assert(m);

    for (rank = 1; rank <= numSubjects; rank++) {
        maxrr = -1;
        for (rr = 0; rr <= numSubjects; rr++) {
            if (hist[rank][rr] > maxrr) {
                maxrr = hist[rank][rr];
                mode = rr;
            }
        }
        m[rank] = mode;
    }
    return m;
}

float* recRateMean(int** hist, int numSubjects) {
    int rank, rr, wsum, sum;
    float* m = (float*) malloc(sizeof(float) * (numSubjects + 1));
    assert(m);

    for (rank = 1; rank <= numSubjects; rank++) {
        wsum = 0;
        sum = 0;
        for (rr = 0; rr <= numSubjects; rr++) {
            wsum = wsum + hist[rank][rr] * rr;
            sum = sum + hist[rank][rr];
        }
        m[rank] = ((float) wsum) / ((float) sum);
    }
    return m;
}

int* recRateLowerBound(int** hist, int numSubjects, int iterations, float boundingProb) {
    int rank, rr, stopCount, cum;
    int* lb = (int*) malloc(sizeof(int) * (numSubjects + 1));
    assert(lb);

    stopCount = (int) (boundingProb * (float) iterations);
    for (rank = 1; rank <= numSubjects; rank++) {
        cum = 0;
        rr = 0;
        while ((cum < stopCount) && (rr <= numSubjects)) {
            cum = cum + hist[rank][rr];
            rr++;
        }
        lb[rank] = rr - 1;
    }
    return lb;
}

int* recRateUpperBound(int** hist, int numSubjects, int iterations, float boundingProb) {
    int rank, rr, stopCount, cum;
    int* ub = (int*) malloc(sizeof(int) * (numSubjects + 1));
    assert(ub);

    stopCount = (int) (boundingProb * (float) iterations);
    for (rank = 1; rank <= numSubjects; rank++) {
        cum = 0;
        rr = numSubjects;
        while ((cum < stopCount) && (rr >= 0)) {
            cum = cum + hist[rank][rr];
            rr--;
        }
        ub[rank] = rr + 1;
    }
    return ub;
}


void generateHistogramDistributionReports(char *outFilePrefix, char *algBasename, int **hist, int numSubjects, int iterations) {
    FILE *f;
    int rank, rc;
    float rr;
    char name[MAX_FSTRLEN];

    /* The histogram of raw counts */
    sprintf(name, "%s%s_%s", outFilePrefix, algBasename, "HistCounts.txt");
    f = fopen(name, "w");
    if (f == NULL) {
        printf("can not open %s to wirte\n", name);
        exit(1);
    }
    fprintf(f, "rc\trr ");
    for (rank = 1; rank <= numSubjects; rank++) {
        fprintf(f, "\tr%d", rank);
    }
    for (rc = 0; rc <= numSubjects; rc++) {
        rr = ((float) rc) / ((float) numSubjects);
        fprintf(f, "\n%d\t%5.2f", rc, rr);
        for (rank = 1; rank <= numSubjects; rank++) {
            fprintf(f, "\t%d", hist[rank][rc]);
        }
    }
    fclose(f);

    /* The histogram of normalized counts - sample probabilities */
    sprintf(name, "%s%s_%s", outFilePrefix,algBasename, "HistProbs.txt");
    f = fopen(name, "w");
    if (f == NULL) {
        printf("can not open %s to wirte\n", name);
        exit(1);
    }
    fprintf(f, "rc\trr ");
    for (rank = 1; rank <= numSubjects; rank++) {
        fprintf(f, "\tr%d", rank);
    }
    for (rc = 0; rc <= numSubjects; rc++) {
        rr = ((float) rc) / ((float) numSubjects);
        fprintf(f, "\n%d\t%5.2f", rc, rr);
        for (rank = 1; rank <= numSubjects; rank++) {
            fprintf(f, "\t%5.4f", ((float) hist[rank][rc]) / ((float) iterations));
        }
    }
    fclose(f);
}

void generateCumulativeMatchCurveReport(char *outFilePrefix, char *algBasename, int **hist, int numSubjects, int iterations) {
    int rank;
    FILE *f;
    char name[MAX_FSTRLEN];
    int *mode, *lb, *ub;
    float *mean;

    mode = recRateMode(hist, numSubjects);
    lb = recRateLowerBound(hist, numSubjects, iterations, 0.025);
    ub = recRateUpperBound(hist, numSubjects, iterations, 0.025);
    mean = recRateMean(hist, numSubjects);

    /* The histogram of raw counts */
    sprintf(name, "%s%s_%s", outFilePrefix,algBasename, "CMCurve.txt");
    f = fopen(name, "w");
    if (f == NULL) {
        printf("can not open %s to wirte\n", name);
        exit(1);
    }
    fprintf(f, "\nrank\tlower\tmode\tupper\tmean");
    for (rank = 1; rank <= numSubjects; rank++) {
        fprintf(f, "\n%d\t%d\t%d\t%d\t%5.1f", rank, lb[rank], mode[rank], ub[rank], mean[rank]);
    }
    fclose(f);
}


#define TR_START(file) fprintf((file),"<TR>\n")
#define TR_END(file) fprintf((file),"</TR>\n")
#define TD_STRING(file,var) fprintf((file),"<TD>%s</TD>\n",(var))
#define TD_RSTRING(file,var) fprintf((file),"<TD><FONT COLOR=RED>%s</FONT></TD>\n",(var))
#define TD_FLOAT(file,var) fprintf((file),"<TD>%0.2f</TD>\n",(var))
#define TD_PERC(file,var) fprintf((file),"<TD>%0.1f%%</TD>\n",(var))
#define TD_PROB(file,var) fprintf((file),"<TD>%0.4f</TD>\n",(var))
#define TD_RPROB(file,var) fprintf((file),"<TD><FONT COLOR=RED>%0.4f</FONT></TD>\n",(var))
#define TD_INT(file,var) fprintf((file),"<TD>%d</TD>\n",(var))


void generateRankNComparisonReport(int n, char *outFilePrefix, AlgorithmData** algData, int numDistDir, int numSubjects, int iterations) {
    int zero = numSubjects + 1;
    int alg, alg1, alg2;
    FILE *f;
    char name[MAX_FSTRLEN];
    int *mode, *lb, *ub;
    float *mean;
    double ns = numSubjects;
    /* The histogram of raw counts */
    sprintf(name, "%s_Rank%d.html", outFilePrefix, n);
    f = fopen(name, "w");
    if (f == NULL) {
        printf("can not open %s to wirte\n", name);
        exit(1);
    }

    fprintf(f, "<HTML><HEAD><TITLE>Rank %d: Algorithm Comparision Report</TITLE></HEAD><BODY>\n",n);
    fprintf(f, "<H1>Rank %d: Algorithm Comparision Report</H1><HR>\n",n);
    fprintf(f, "<P><H2>Algorithms Tested:</H2>\n<P><UL>\n");
    for(alg = 0; alg < numDistDir; alg++){
        fprintf(f, "<LI>%s</LI>\n",algData[alg]->basename);
    }
    fprintf(f, "</UL><HR>\n");

    fprintf(f, "<H2>Permutation Results:</H2>\n");


    fprintf(f, "<TABLE BORDER=2><TR><TH>Algorithm</TH><TH>Mean</TH><TH>Mode</TH><TH>Lower</TH><TH>Upper</TH>"
               "<TH>Mean%%</TH><TH>Mode%%</TH><TH>Lower%%</TH><TH>Upper%%</TH></TR>\n");
    for(alg = 0; alg < numDistDir; alg++){
        mode = recRateMode(algData[alg]->hist, numSubjects);
        lb = recRateLowerBound(algData[alg]->hist, numSubjects, iterations, 0.025);
        ub = recRateUpperBound(algData[alg]->hist, numSubjects, iterations, 0.025);
        mean = recRateMean(algData[alg]->hist, numSubjects);
        fprintf(f, "<TR><TD>%s</TD><TD>%0.1f</TD><TD>%d</TD><TD>%d</TD><TD>%d</TD>"
                   "<TD>%0.1f%%</TD><TD>%0.1f%%</TD><TD>%0.1f%%</TD><TD>%0.1f%%</TD></TR>\n",
                   algData[alg]->basename, mean[n], mode[n], lb[n], ub[n], 100.0*mean[n]/ns, 100.0*mode[n]/ns, 100.0*lb[n]/ns, 100.0*ub[n]/ns);
    }
    fprintf(f, "</TABLE><HR>\n");

    fprintf(f, "<H2>Comparing Algorithms (Alg1 - Alg2): </H2>\n");
    fprintf(f, "<TABLE BORDER=2><TR><TH>Algorithm 1</TH><TH>Algorithm 2</TH>"
               "<TH>Mean</TH><TH>Mode</TH><TH>Lower</TH><TH>Upper</TH><TH>P(Alg1 > Alg2)</TH><TH>P(Alg1 < Alg2)</TH></TR>\n");
    for(alg1 = 0; alg1 < numDistDir; alg1++){
        for(alg2 = alg1 + 1; alg2 < numDistDir; alg2++){
            int** diffhist;
            float diff_mean;
            int diff_mode = 0;
            int diff_upper = 0;
            int total_count = 0;
            int diff_lower = 0;
            int PAlg1 = 0, PAlg2 = 0;
            int count = 0;
            int maxrr = -1;
            int sum = 0;

            /* Compute the difference histogram */
            diffhist = histogramCompareCounts(algData[alg1]->recCounts, algData[alg2]->recCounts, iterations, numSubjects);

            /* Calculate the Histogram Statistics */
            for (count = -numSubjects; count <= numSubjects; count++) {
                if( maxrr < diffhist[n][zero+count] ){
                    maxrr = diffhist[n][zero+count];
                    diff_mode = count;
                }

                if(count > 0) PAlg1 += diffhist[n][zero+count];
                if(count < 0) PAlg2 += diffhist[n][zero+count];
                sum += count*diffhist[n][zero+count];

                if(total_count < 0.975*iterations){
                    diff_upper = count;
                }
                total_count += diffhist[n][zero+count];
                if(total_count < 0.025*iterations){
                    diff_lower = count+1;
                }
            }
            diff_mean = ((double)sum)/iterations;

            TR_START(f);
            if(((double)PAlg1)/iterations > ((double)PAlg2)/iterations){
                TD_RSTRING(f,algData[alg1]->basename);
            }
            else{
                TD_STRING(f,algData[alg1]->basename);
            }

            if(((double)PAlg1)/iterations < ((double)PAlg2)/iterations){
                TD_RSTRING(f,algData[alg2]->basename);
            }
            else{
                TD_STRING(f,algData[alg2]->basename);
            }

            TD_FLOAT(f,diff_mean);
            TD_INT(f,diff_mode);
            TD_INT(f,diff_lower);
            TD_INT(f,diff_upper);
            if(((double)PAlg1)/iterations >= 0.95){
                TD_RPROB(f,((double)PAlg1)/iterations);
            }
            else{
                TD_PROB(f,((double)PAlg1)/iterations);
            }

            if(((double)PAlg2)/iterations >= 0.95){
                TD_RPROB(f,((double)PAlg2)/iterations);
            }
            else{
                TD_PROB(f,((double)PAlg2)/iterations);
            }
            TR_END(f);
        }
    }
    fprintf(f, "</TABLE>\n");

    fprintf(f, "done.</BODY></HTML>\n");
    fclose(f);
}


/* Process the command line arguments */

void usage(const char* progname){
    printf("\nThree command line arguments required:\n");
    printf("1: .srt File of test image names.\n");
    printf("2: Prefix for output file names.\n");
    printf("3: List of directorys to read distances files from.\n");
    exit(1);
}

void readArgs(int argc, char *argv[], int* numDistDir, char ***distDir, char **distFileNames, char **outFilePrefix) {
    int i;

    if ((argc < 4) || (strcmp(argv[1], "-help") == 0)) {
        usage(argv[0]);
    }
    *distFileNames = argv[1];
    *outFilePrefix = argv[2];
    (*distDir)=malloc(sizeof(char*)*(argc-3));
    *numDistDir = (argc-3);
    for(i = 0; i < *numDistDir; i++){
        (*distDir)[i] = argv[i+3];
    }
    if (PRINT_LEVEL > 0) {
        printf("\nCommand Line arguments are:\n");
        printf("  Directory from which to read distance files ... %s\n", (*distDir)[0]);
        printf("  .srt File of test image names ...................... %s\n", *distFileNames);
        printf("  Prefix for output file names .................. %s\n", *outFilePrefix);
    }
}

/*

This function will create and configure experiment datastructures for one
distance matrix. It starts by reading in the distance matrix for the set of
images. It then, for each test image, builds a sorted list of the other test
images. This is done once, and expedites computing recognition rate for each
of the many, typicaly 10,000, iterations of randomly swapping probe and
gallery images.

*/

AlgorithmData* loadAlgoirthmData(char *distDir, FERETID*** srt, int numSubjects, int numReplicates, int numImages, FERETID** fids){
    int i;
    float** dist;
    int lastchar;
    AlgorithmData* data = malloc(sizeof(AlgorithmData));



    if (PRINT_LEVEL > 0)
        printf("\nReading in distance matrix.");
    dist = readDistances(fids, srt, numImages, numReplicates, numSubjects, distDir);

    if (PRINT_LEVEL > 0)
        printf("\nSorting image pairs by ascending distance.");
    data->fidprs = buildSortedPairedMatrix(fids, numImages, dist);

    data->recCounts = makeRecCounts(ITERATIONS, numSubjects);

    for( i = 0; i < numImages; i++){
        free(dist[i]);
    }
    free(dist);

    lastchar = strlen(distDir)-1;
    if(distDir[lastchar] == '/'){
        distDir[lastchar] = 0;
    }
//pip    data->basename = strdup(basename(distDir));

    return data;
}

/*
                                        MAIN
   
   This code will read the file of test images names and then build the 
   distance matrix by reading each of the distance files in the distances
   directory. It then, for each test image, builds a sorted list of the
   other test images. This is done once, and expedites computing recognition
   rate for each of the many, typicaly 10,000, iterations of randomly swapping
   probe and gallery images. At the end, three files are generated that contain
   summaries of the results. Specifically, there are two versions of the
   recognition rate histograms: one using raw integer counts and the other 
   normalized values. The later may be interpreted as probabilities and the 
   histogram as a whole as a sample probability density function. The last
   report gives the cumulative match curve along defined by the mode of these 
   distributions, along with error bars defined by the two-sides ninety five 
   percent confidence interval. 

*/

int main(int argc, char *argv[]) {
    int t, d, numSubjects, numReplicates, numImages;
    char **distDir;     /* Name of the directory where distance files are stored */
    char *distFileNames;  /* Name imagefile used for the experiment */
    char *outFilePrefix;    /* Name of the file to which results are written */
    FERETID** fids;
    FERETID*** srt;
    PGSET* pgs;
    int* recRanks;
    ImageList* imNames;
    AlgorithmData** algData;
    int numDistDir;

    if (PRINT_LEVEL > 0)
        printf("\n%s\n%s", OPENNING, VERSION);
    readArgs(argc, argv, &numDistDir, &distDir, &distFileNames, &outFilePrefix);

    imNames = getImageNames(distFileNames, &numImages);

    srt = subjectReplicatesTable(&imNames, &numImages, &numSubjects, &numReplicates, &fids);
    if (PRINT_LEVEL > 1)
        printf("\n  There are %d subjects and %d replicates.", numSubjects, numReplicates);

    if (numReplicates < 3) {
        printf("\nError:  there must be at least 3 replicates per subject.");
        printf("\n        The image list indicates %d replicate per subject.", numReplicates);
        printf("\n        Please check your file list to make sure it is in the proper format.\n");
        exit(1);
    }



    algData = malloc(sizeof(AlgorithmData*) * numDistDir);
    assert(algData);
    for( d = 0; d < numDistDir; d++){
        algData[d] = loadAlgoirthmData(distDir[d], srt, numSubjects, numReplicates, numImages, fids);
    }

    if (PRINT_LEVEL > 0)
        printf("\nPreparing data structures to record recognition counts over iterations.");
    pgs = makePGSET(numImages, numSubjects);
    recRanks = makeRecRanks(numSubjects);

    if (PRINT_LEVEL > 0)
        printf("\nBegining iterations.");
    for (t = 0; t < ITERATIONS; t++) {
        balancedSelectionProbeGallery(pgs, srt, numImages, numSubjects, numReplicates);
        for( d = 0; d < numDistDir; d++){
            loadRecognitionCounts(pgs, algData[d]->fidprs, recRanks, algData[d]->recCounts[t], numImages, numSubjects);
        }
        if ((t + 1) % (ITERATIONS / 10) == 0)
            printf("\n  Iteration %d.", t + 1);
    }

    if (PRINT_LEVEL > 0)
        printf("\nBuilding Sample Distributions - Histograms.");
    for( d = 0; d < numDistDir; d++){
        algData[d]->hist = histogramRecognitionCounts(algData[d]->recCounts, ITERATIONS, numSubjects);
        generateHistogramDistributionReports(outFilePrefix, algData[d]->basename, algData[d]->hist, numSubjects, ITERATIONS);
        generateCumulativeMatchCurveReport(outFilePrefix, algData[d]->basename, algData[d]->hist, numSubjects, ITERATIONS);
    }

    generateRankNComparisonReport(1, outFilePrefix, algData, numDistDir, numSubjects, ITERATIONS);

    if (PRINT_LEVEL > 0)
        printf("\nFinished\n");
    return 0;

}


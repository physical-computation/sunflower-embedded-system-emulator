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
#include <assert.h>
#include <csuCommon.h>

#define OPENNING "Parse list of FERET IDs into new file, one line per subject."

/*
 *  ============================================================================
 *  Copyright 2002, Colorado State University. All Rights Reserved.           
 *  --------------------------------------------------------------------------- 
 *  Permission to use, copy or modify this software and its documentation for   
 *  educational and research purposes only, provided that this copyright notice  
 *  appear on all copies and supporting documentation.                          
 *  ---------------------------------------------------------------------------        
 *  File:    csuReplicatesTable.c                                                     
 *  Authors: J. Ross Beveridge                                   
 *  Date:    May 24, 2002                                                  
 *  ============================================================================
 */

/* ===========================================================================
 * Compile time configuration options 
 */
#define PRINT_LEVEL 1    /* Controls amount of text printed to stdout */
#define MAX_FSTRLEN 100      /* Upper bound on file name string lengths */

/*  ===========================================================================
 *   Local structures to build linked lists of subjects and images
 */
typedef struct feretimage {
    char* name;        			/* The text name designating the FERET image */
    int subject;       		  	/* The unique subject identification number */
    struct feretimage *next;  	/* Next image for this subject */
}
FIM;

typedef struct feretsubject {
    int id;   					/* The unique subject identification number */
    struct feretimage *images;  /* The list of images for this subject */
    struct feretsubject *next;
}
FSU;

/* ============================================================================
 * Function Prototypes
 */

FIM* makeFIM(char* name);
FSU* makeFSU(int id);
int  positionOfFirstNonDigit(char *name);
int  idFromName(char* name);
FSU* insertSubjectsFromFile(char *inFile);
FSU* insertSubjectName(FSU *head, char* name);
void insertSubjectNameRecurse(FSU *prev, FSU* curr, int si, char* name);
void appendImage(FIM* curr, FIM* im);
void printSubjectReplicatesFile(FSU* head, char* outFile);
void readArgs(int argc, char *argv[], char **fileNames, char **outFile);

/* ===========================================================================
 * Structure Constructors
 */

FIM* makeFIM(char* name) {
    char* tt;
    FIM* im;

    im = (FIM*) malloc(sizeof(FIM));
    assert(im);
    tt = (char*) malloc(sizeof(char) * MAX_FSTRLEN);
    assert(tt);
    strcpy(tt, name);
    im->name = tt;
    im->subject = idFromName(name);
    im->next = NULL;

    return im;
}

FSU* makeFSU(int id) {
    FSU* su = (FSU*) malloc(sizeof(FSU));
    assert(su);
    su->id = id;
    su->next = NULL;

    return su;
}

/* ===========================================================================
 * Routines to assist interpreting strings representing FERET ids.
 */

/* The integer corresponding to the FERET subject ID extracted from name */
int idFromName(char* name) {
    char str[MAX_FSTRLEN];
    int i = positionOfFirstNonDigit(name);
    strncpy(str, name, i);

    return atoi(str);
}

/*  The integer index of the first character that is not a digit. */
int positionOfFirstNonDigit(char *name) {
    int i, j, len, flag;
    const char digits[] = "0123456789";

    len = strlen(name);
    flag = 1;
    i = 0;
    while ((i < len) && flag) {
        flag = 0;
        for (j = 0; j < 10; j++) {
            if (name[i] == digits[j])
                flag = 1;
        }
        i++;
    }
    return i;
}

/* ===========================================================================
 Build up a linked list of subjects, each point pointing to a linked list
 of the images for that subject. The linked list of subjects is maintained
 in sorted order from least to greatest subject id. The images for each 
 subject are in the order encountered in the input file of image names.
*/

FSU* insertSubjectsFromFile(char *inFile) {
    FILE *f;
    FSU *head = NULL;
    char buffer[MAX_FSTRLEN], name[MAX_FSTRLEN];

    f = fopen(inFile, "r");
    if (f == NULL) {
        printf("\nCannot open %s to read!\n", inFile);
        exit(1);
    }
    while (fgets(buffer, 100, f)) {
        sscanf(buffer, "%s", name);
        head = insertSubjectName(head, name);
    }
    fclose(f);
    return head;
}

FSU* insertSubjectName(FSU *head, char* name) {
    FIM* im;
    FSU* su;
    int si;  /* Subject index of name to insert */

    si = idFromName(name);
    /* First initial case, linked list of subjects is empty */
    if (head == NULL) {
        /* printf("\n Adding %s, id %d,  as first subject in linked list \n", name, si); */
        im = makeFIM(name);
        su = makeFSU(si);
        su->images = im;
        return su;
    }
    /* Special case that current subject id is less than head of list */
    if (head->id > si) {
        /* printf("Special Case: Current Subject id is less than head of list, adding %s, id %d to head. \n", name, si ); */
        im = makeFIM(name);
        su = makeFSU(si);
        su->images = im;
        su->next = head;
        return su;
    }
    /* Special case that current subject id is equal to the head of the list */
    if (head->id == si) {
        /* printf("Another image %s for subject id %d. \n", name, si); */
        appendImage(head->images, makeFIM(name));
        return head;
    }
    /* List is not empty, and current is not going to become the head */
    insertSubjectNameRecurse(head, head->next, si, name);
    return head;
}

void insertSubjectNameRecurse(FSU *prev, FSU* curr, int si, char* name) {
    FIM* im;
    FSU* su;

    /* We've run off the end of the list, tag new subject onto the end */
    if (curr == NULL) {
        /* printf("In recursion, ran off end with subject id %d, add %s to end \n", prev->id, name); */
        im = makeFIM(name);
        su = makeFSU(si);
        su->images = im;
        prev->next = su;
    }
    /* If current id equals new, then append image to current */
    else if (curr->id == si) {
        /* printf("Current id %d equals %d, adding image %s to end of images. \n", curr->id, si, name); */
        appendImage(curr->images, makeFIM(name));
    }
    /* If current id greater than new, then insert new subject here */
    else if (curr->id > si) {
        /* printf("Current id %d greater than new id %d, inserting new subject with first image %s. \n", curr->id, si, name); */
        im = makeFIM(name);
        su = makeFSU(si);
        su->images = im;
        prev->next = su;
        su->next = curr;
    }
    /* Keep looking down the list */
    else {
        insertSubjectNameRecurse(curr, curr->next, si, name);
    }
}

void appendImage(FIM* curr, FIM* im) {
    if (curr->next == NULL) {
        curr->next = im;
    } else {
        appendImage(curr->next, im);
    }
}

/* Print out a usage message and exit */
void usage(const char* progname) {
    if (progname){
        printf("\nusage: %s flat_list_file.list subject_replicate_file.srt\n", progname);
    }
    printf("\n    Two command line arguments required:\n");
    printf("    1: File of test image names, one name per line in the file.\n");
    printf("    2: Name of file to create with names grouped by subject.\n");
    exit(1);
}
/* ===========================================================================
 Read in the command line arguments, rather simple for this program 
*/

void readArgs(int argc, char *argv[], char **fileNames, char **outFile) {
    if (argc <= 1) {
        usage(argv[0]);
    }
    if ((strcmp(argv[1], "-help") == 0) || (argc < 3)) {
        usage(argv[0]);
    }
    *fileNames = argv[1];
    *outFile = argv[2];
    if (PRINT_LEVEL > 0) {
        printf("\nCommand Line arguments are:\n");
        printf("  File of test image names, one name per line in the file ... %s\n", *fileNames);
        printf("  Name of file to create with names grouped by subject ...... %s\n", *outFile);
    }
}

/*  ---------------------------------------------------------------------------
    Generate the output file from the subject replicates linked lists.
    ---------------------------------------------------------------------------*/

void printSubjectReplicatesFile(FSU* head, char* outFile) {
    FILE *f;
    FIM* im;
    int i;

    f = fopen(outFile, "w");
    if (f == NULL) {
        printf("cannot open %s to write\n", outFile);
        exit(1);
    }
    while (head != NULL) {
        i = 0;
        im = head->images;
        while (im != NULL) {
            if (i++ > 0)
                fprintf(f, "\t");  /* print tab between names */
            fprintf(f, "%s ", im->name);
            im = im->next;
        }
        fprintf(f, "\n");
        head = head->next;
    }

    fclose(f);
}

/* ===========================================================================
                                        MAIN
   
   This code reads a simple file with one FERET image name per row. It groups 
   the images by subject id and then writes a new file. This new file lists 
   all images for a single subject on one line and these lines are sorted by
   subject ID.  This is the format read by csuSubspaceTrain.
                                    
*/

int main(int argc, char *argv[]) {
    char *inFile;     /* Name of the file with image names one per line */
    char *outFile;    /* Name of the file to which results are written */
    FSU* head = NULL; /* Head pointer to the linked list of subjects */

    if (PRINT_LEVEL > 0)
        printf("\n%s\n%s", OPENNING, VERSION);
    readArgs(argc, argv, &inFile, &outFile);

    if (PRINT_LEVEL > 0)
        printf("\nReading from %s and building subject/replicates linked lists.", inFile);
    head = insertSubjectsFromFile(inFile);

    printSubjectReplicatesFile(head, outFile);
    if (PRINT_LEVEL > 0)
        printf("\nFinished. \n");
    return 0;
}


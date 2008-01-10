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
$RCSfile: csuEBGMLocalization.c,v $
$Author: teixeira $
$Date: 2003/04/23 14:02:31 $
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

#include <csuCommon.h>
#include <csuEBGMUtil.h>
#include <csuEBGMSimilarity.h>

#define NUM_MEASURES 7
#define NUM_DISPLACE 4

struct {
    char* expSpec;
    char* imageDir;
    char* graphDir;
    char* outputDir;
    char* maskFile;

    JetSimilarityMeasure* measures;
    JetDisplacementEstimator* estimators;
    char** simLabels;
    char** disLabels;

    JetMasks masks;
} globals;



void usage(const char* progname){
    printf("usage: %s Experimentspecification gaborimagedir graph_directory output_directory maskfile\n", progname);
    exit(1);
}



void process_command(int argc, char** argv) {
    int i;
    int param_num;

    param_num = 0;

    if (argc < 3)
        usage(argv[0]);
    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-help") == 0) { usage(argv[0]); exit(1); }
    }


    quiet = 0;
    debuglevel = 0;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-debuglevel") == 0) {
            DEBUG_CHECK(++i < argc, "Bad parameter list");
            debuglevel = atoi(argv[i]);
        } else if (strcmp(argv[i], "-quiet") == 0) {
            quiet = 1;
        } else if (argv[i][0] == '-') {
            printf("Unknown option: %s\n", argv[i]);
            usage(argv[0]);
        } else if (param_num == 0) {
            globals.expSpec = argv[i];
            param_num++;
        } else if (param_num == 1) {
            globals.imageDir = argv[i];
            param_num++;
        } else if (param_num == 2) {
            globals.graphDir = argv[i];
            param_num++;
        } else if (param_num == 3) {
            globals.outputDir = argv[i];
            param_num++;
        } else if (param_num == 4) {
            globals.maskFile = argv[i];
            param_num++;
        } else {
            printf("Unknown parameter: %s\n", argv[i]);
            usage(argv[0]);
        }
    }

    if (param_num != 5)
        usage(argv[0]);
}


void printHtmlHeader(FILE* rFile){
    time_t t = time(0);
    fprintf(rFile,"<HTML>\n<HEAD>\n<TITLE>");
    fprintf(rFile,"Gabor Localization Study");
    fprintf(rFile,"</TITLE>\n</HEAD>\n<BODY>\n");
    fprintf(rFile,"<H1><CENTER>Gabor Localization Study</CENTER></H1>\n");
    fprintf(rFile,"<CENTER>Date: %s</CENTER><br>\n",ctime(&t));
    fprintf(rFile,"<CENTER>Masks: %s</CENTER><br>\n",globals.maskFile);
    fprintf(rFile,"<p>These are the results of the gabor localization study. \n"
                  "The purpose of this file is to study the ability of gabor \n"
                  "jets to localized points on a face.  The images shown here \n"
                  "are created by measuring the similarity of jets from each \n"
                  "point in the image to the a selected jet.\n"
                  "\n"
                  "<p>Idealy the images should show a good basin of attraction\n"
                  "around the correctly matched points.  This study is investigates\n"
                  "different facal features and similarity measures.\n<p>\n"
    );
    fflush(rFile);
}

void printExp1Header(FILE* rFile){
    fprintf(rFile,"<HR><H2>Experiment 1: Show the similarity for jets taken from the same image.</H2>\n");
    fprintf(rFile,"<p>This experiment shows the results when a jet is taken from \n"
                  "the same image that is used for testing.  For this reason the\n"
                  "correct match should always be found.\n<p>\n"
    );
    fflush(rFile);
}

void printExp2Header(FILE* rFile){
    fprintf(rFile,"<HR><H2>Experiment 2: Show the similarity for jets taken from the same subject but different images.</H2>\n");
    fprintf(rFile,"<p>This experiment shows the results when a jet is taken from \n"
                  "the same subject that is used for testing, however the jets are\n"
                  "taken from different images.  This experiment will hopfully show\n"
                  "that jets taken from the same subject are similar enough to find\n"
                  "the same point for an unseen image of the same person.<p>\n"
    );
    fflush(rFile);
}


void printExp3Header(FILE* rFile){
    fprintf(rFile,"<HR><H2>Experiment 3: Show the similarity jets across subjects.</H2>\n");
    fprintf(rFile,"<p>This experiment shows the results when a jet is taken from one \n"
                  "is compared to a jet taken from another subject.  This experiment \n"
                  "will show how well the Jets can localize points for subjects it has\n"
                  "never seen before.<p>\n"
    );
    fflush(rFile);
}

void printHtmlFooter(FILE* rFile){
    fprintf(rFile,"</BODY>\n</HTML>\n");
    fflush(rFile);
}


/* Save a color image into a specific format of file */
void saveColorImage(char *filename, Image im){
    char* command = (char*)malloc(sizeof(char)*4096);
    char* ppmname = (char*)malloc(sizeof(char)*4096);

    assert(im->channels == 3);

    /* Save image to a ppm file */
    sprintf(ppmname,"%s.ppm",filename);
    writePPMImage(im,ppmname);

    /* Let convert figure out how to save the file in the requested format */
    sprintf(command,"convert %s %s", ppmname, filename);
    system(command);

    /* remove the ppm file leaving only the requested image */
    sprintf(command,"rm %s", ppmname);
    system(command);

    free(command);
    free(ppmname);
}


/* Save a gray image into a specific format of file */
void saveBWImage(char *filename, Image im){
    char* command = (char*)malloc(sizeof(char)*4096);
    char* pgmname = (char*)malloc(sizeof(char)*4096);

    assert(im->channels == 1);

    /* Save image to a pgm file */
    sprintf(pgmname,"%s.pgm",filename);
    writePGMImage(im,pgmname,0);

    /* Let convert figure out how to save the file in the requested format */
    sprintf(command,"convert %s %s", pgmname, filename);
    system(command);

    /* remove the ppm file leaving only the requested image */
    sprintf(command,"rm %s", pgmname);
    system(command);

    free(command);
    free(pgmname);
}


/* Save a gray image into a specific format of file */
void saveThermalImage(char *filename, Image im){
    char* command = (char*)malloc(sizeof(char)*4096);
    char* ppmname = (char*)malloc(sizeof(char)*4096);
    Image therm = makeThermalColorImage(im);

    assert(im->channels == 1);

    /* Save image to a pgm file */
    sprintf(ppmname,"%s.ppm",filename);
    writePPMImage(therm,ppmname);

    /* Let convert figure out how to save the file in the requested format */
    sprintf(command,"convert %s %s", ppmname, filename);
    system(command);

    /* remove the ppm file leaving only the requested image */
    sprintf(command,"rm %s", ppmname);
    system(command);

    free(command);
    free(ppmname);
    freeImage(therm);
}

/* Save a gray image into a specific format of file */
void saveThermalImageMark(char *filename, Image im, FTYPE x, FTYPE y){
    char* command = (char*)malloc(sizeof(char)*4096);
    char* ppmname = (char*)malloc(sizeof(char)*4096);
    Image therm = makeThermalColorImage(im);

    assert(im->channels == 1);
    
    markPoint(therm,x,y);

    /* Save image to a pgm file */
    sprintf(ppmname,"%s.ppm",filename);
    writePPMImage(therm,ppmname);

    /* Let convert figure out how to save the file in the requested format */
    sprintf(command,"convert %s %s", ppmname, filename);
    system(command);

    /* remove the ppm file leaving only the requested image */
    sprintf(command,"rm %s", ppmname);
    system(command);

    free(command);
    free(ppmname);
    freeImage(therm);
}





void plotHist(char* filename, char* distMes, int* counts, int maxIndex){
    int i;
    FILE* histfile = fopen(makePath(globals.outputDir,"tmp.dat"),"w");
    FILE* gnuplot  = popen("gnuplot","w");

    printf("Outputing histogram: %s\n",filename);
    assert(gnuplot);
    assert(histfile);
    for(i = 0; i < 25; i++){
        fprintf(histfile,"%d %d\n",i, i < maxIndex ? counts[i] : 0);
    }
    fclose(histfile);

    fprintf(gnuplot,"set xlabel \'Distance\'\n");
    fprintf(gnuplot,"set ylabel \'Count\'\n");
    fprintf(gnuplot,"set xtics 5\n");
    fprintf(gnuplot,"set term png color\n");
    fprintf(gnuplot,"set output \'%s\'\n",makePath(globals.outputDir,filename));
    fprintf(gnuplot,"set title \'%s Distances\'\n",distMes);
    fprintf(gnuplot,"show title\n");
    fprintf(gnuplot,"plot [0:25] \'%s\' with steps\n",makePath(globals.outputDir,"tmp.dat"));
    fclose(gnuplot);
}



void plotVectorFieldPNG(char* filename, char* title, Image displacementImage, int expNum, FTYPE x, FTYPE y){
    int i,j;
    FILE* gnuplot  = popen("gnuplot","w");

    printf("Outputing Vector Field: %s\n",filename);
    assert(gnuplot);

    fprintf(gnuplot,"set xlabel \'X Displacement\'\n");
    fprintf(gnuplot,"set ylabel \'Y Displacement\'\n");
    fprintf(gnuplot,"set xtics 5\n");
    fprintf(gnuplot,"set term png color\n");
/* fprintf(gnuplot,"set size square\n"); */
    fprintf(gnuplot,"set size .75,1.05\n");
    fprintf(gnuplot,"set size ratio -1\n");
    fprintf(gnuplot,"set output \'%s\'\n",filename);
/*     fprintf(gnuplot,"set title \'%s\'\n",title); */
/*     fprintf(gnuplot,"set notitle \n"); */
/*     fprintf(gnuplot,"show title\n"); */
    fprintf(gnuplot,"plot [-8:8] [8:-8] \'-\' notitle with vector\n");
    for(j = 0; j < displacementImage->height; j++){
        for(i = 0; i < displacementImage->width; i++){
            fprintf(gnuplot,"%f %f %f %f\n",(double)i - x,(double)j-y,-ie(displacementImage,i,j,0),-ie(displacementImage,i,j,1));
        }
    }
    fprintf(gnuplot,"e\n");
    fprintf(gnuplot,"quit\n");
    fclose(gnuplot);
}


void plotVectorFieldEPS(char* filename, char* title, Image displacementImage, int expNum, FTYPE x, FTYPE y){
    int i,j;
    FILE* gnuplot  = popen("gnuplot","w");

    printf("Outputing Vector Field: %s\n",filename);
    assert(gnuplot);

    fprintf(gnuplot,"set xlabel \'X Displacement\'\n");
    fprintf(gnuplot,"set ylabel \'Y Displacement\'\n");
    fprintf(gnuplot,"set xtics 5\n");
    fprintf(gnuplot,"set term postscript eps monochrome\n");
/*     //fprintf(gnuplot,"set size .5..5\n"); */
    fprintf(gnuplot,"set size 0.5,0.7\n");
    fprintf(gnuplot,"set size ratio -1\n");
    fprintf(gnuplot,"set output \'%s\'\n",filename);
/*     //fprintf(gnuplot,"set title \'%s\'\n",title); */
/*     //fprintf(gnuplot,"set notitle \n"); */
/*     //fprintf(gnuplot,"show title\n"); */
    fprintf(gnuplot,"plot [-8:8] [8:-8] \'-\' notitle with vector\n");
    for(j = 0; j < displacementImage->height; j++){
        for(i = 0; i < displacementImage->width; i++){
            fprintf(gnuplot,"%f %f %f %f\n",(double)i - x,(double)j-y,-ie(displacementImage,i,j,0),-ie(displacementImage,i,j,1));
        }
    }
    fprintf(gnuplot,"e\n");
    fprintf(gnuplot,"quit\n");
    fclose(gnuplot);
}



void plotDisplaceErrorPNG(char* filename, char* title, Image displacementImage, int expNum, FTYPE x, FTYPE y){
    int i,j;
    FILE* gnuplot  = popen("gnuplot","w");
    FTYPE aError[13] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0 };
    FTYPE cError[13] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0 };

    printf("Outputing histogram: %s\n",filename);
    assert(gnuplot);

    fprintf(gnuplot,"set xlabel \'Jet Distance\'\n");
    fprintf(gnuplot,"set ylabel \'Displacement Error\'\n");
    fprintf(gnuplot,"set xtics 2\n");
    fprintf(gnuplot,"set ytics 2\n");
/*     //fprintf(gnuplot,"set term postscript eps color\n"); */
    fprintf(gnuplot,"set term png color\n");
    fprintf(gnuplot,"set size .75,1.05\n");
    fprintf(gnuplot,"set size ratio -1\n");
    fprintf(gnuplot,"set output \'%s\'\n",filename);
/*     //fprintf(gnuplot,"set title \'%s\'\n",title); */
/*     //fprintf(gnuplot,"set notitle \n"); */
/*     //fprintf(gnuplot,"show title\n"); */
    fprintf(gnuplot,"plot [0:12] [0:12] \'-\' notitle with points lt -1 pt 7 ps 0.2, \'-\' notitle with lines lt 1 lw 3\n");
/*     //for(point = 0; point < novelGraph->numVert; point++){ */
/*         //double x = novelGraph->verts[point].x; */
/*         //double y = novelGraph->verts[point].y; */
        for(j = 0; j < displacementImage->height; j++){
            for(i = 0; i < displacementImage->width; i++){
                double ptdist = sqrt(SQR(x-i) + SQR(y-j));
                double estx = i-ie(displacementImage,i,j,0);
                double esty = j-ie(displacementImage,i,j,1);
                double errordist = sqrt(SQR(estx - x) + SQR(esty - y));
                int index = ROUND(ptdist);

                fprintf(gnuplot,"%f %f\n",ptdist,errordist);

                if( index < 13){
                    aError[index] += errordist;
                    cError[index] += 1.0;
                }
            }
        }
/*     //} */

    fprintf(gnuplot,"e\n");
    for(i = 0; i < 13; i++){
        if(cError[i] != 0.0){
            fprintf(gnuplot,"%f %f\n", (FTYPE)i, aError[i]/cError[i]);
        } else {
/*             //fprintf(gnuplot,"%f 0.0\n", (FTYPE)i); */
        }
    }
    fprintf(gnuplot,"e\n");
    fprintf(gnuplot,"quit\n");
    fclose(gnuplot);
}



void plotDisplaceErrorEPS(char* filename, char* title, Image displacementImage,int expNum, FTYPE x, FTYPE y){
    int i,j;
    FILE* gnuplot  = popen("gnuplot","w");
    FTYPE aError[13] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0 };
    FTYPE cError[13] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0 };

    printf("Outputing histogram: %s\n",filename);
    assert(gnuplot);

    fprintf(gnuplot,"set xlabel \'Jet Distance\'\n");
    fprintf(gnuplot,"set ylabel \'Displacement Error\'\n");
    fprintf(gnuplot,"set xtics 2\n");
    fprintf(gnuplot,"set ytics 2\n");
    fprintf(gnuplot,"set term postscript eps color \"Helvetica\" 10\n");
/*     //fprintf(gnuplot,"set term png color\n"); */
    fprintf(gnuplot,"set size 0.5,0.7\n");
    fprintf(gnuplot,"set size ratio -1\n");
    fprintf(gnuplot,"set output \'%s\'\n",filename);
/*     //fprintf(gnuplot,"set title \'%s\'\n",title); */
/*     //fprintf(gnuplot,"set notitle \n"); */
/*     //fprintf(gnuplot,"show title\n"); */
    fprintf(gnuplot,"plot [0:12] [0:12] \'-\' notitle with points lt 9 pt 7 ps 0.2, \'-\' notitle with lines lt 1 lw 3\n");
/*     //for(point = 0; point < novelGraph->numVert; point++){ */
/*         //double x = novelGraph->verts[point].x; */
/*         //double y = novelGraph->verts[point].y; */
        for(j = 0; j < displacementImage->height; j++){
            for(i = 0; i < displacementImage->width; i++){
                double ptdist = sqrt(SQR(x-i) + SQR(y-j));
                double estx = i-ie(displacementImage,i,j,0);
                double esty = j-ie(displacementImage,i,j,1);
                double errordist = sqrt(SQR(estx - x) + SQR(esty - y));
                int index = ROUND(ptdist);

                fprintf(gnuplot,"%f %f\n",ptdist,errordist);

                if( index < 13){
                    aError[index] += errordist;
                    cError[index] += 1.0;
                }
            }
        }
/*     //} */

    fprintf(gnuplot,"e\n");
    for(i = 0; i < 13; i++){
        if(cError[i] != 0.0){
            fprintf(gnuplot,"%f %f\n", (FTYPE)i, aError[i]/cError[i]);
        } else {
/*             //fprintf(gnuplot,"%f 0.0\n", (FTYPE)i); */
        }
    }
    fprintf(gnuplot,"e\n");
    fprintf(gnuplot,"quit\n");
    fclose(gnuplot);
}


void plotTotalDisplaceErrorPNG(char* filename, char* title, Image*  displacementImage, GraphDiscription novelGraph,int expNum){
    int i,j, point;
    FILE* gnuplot  = popen("gnuplot","w");
    FTYPE aError[13] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0 };
    FTYPE cError[13] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0 };

    printf("Outputing histogram: %s\n",filename);
    assert(gnuplot);

    fprintf(gnuplot,"set xlabel \'Jet Distance\'\n");
    fprintf(gnuplot,"set ylabel \'Displacement Error\'\n");
    fprintf(gnuplot,"set xtics 2\n");
    fprintf(gnuplot,"set ytics 2\n");
/*     //fprintf(gnuplot,"set term postscript eps color\n"); */
    fprintf(gnuplot,"set term png color\n");
    fprintf(gnuplot,"set size .75,1.05\n");
    fprintf(gnuplot,"set size ratio -1\n");
    fprintf(gnuplot,"set output \'%s\'\n",filename);
/*     //fprintf(gnuplot,"set title \'%s\'\n",title); */
/*     //fprintf(gnuplot,"set notitle \n"); */
/*     //fprintf(gnuplot,"show title\n"); */
    fprintf(gnuplot,"plot [0:12] [0:12] \'-\' notitle with points lt -1 pt 7 ps 0.2, \'-\' notitle with lines lt 1 lw 3\n");
    for(point = 0; point < novelGraph->numVert; point++){
        double x = novelGraph->verts[point].x;
        double y = novelGraph->verts[point].y;
        for(j = 0; j < displacementImage[point]->height; j++){
            for(i = 0; i < displacementImage[point]->width; i++){
                double ptdist = sqrt(SQR(x-i) + SQR(y-j));
                double estx = i-ie(displacementImage[point],i,j,0);
                double esty = j-ie(displacementImage[point],i,j,1);
                double errordist = sqrt(SQR(estx - x) + SQR(esty - y));
                int index = ROUND(ptdist);

                fprintf(gnuplot,"%f %f\n",ptdist,errordist);

                if( index < 13){
                    aError[index] += errordist;
                    cError[index] += 1.0;
                }
            }
        }
    }

    fprintf(gnuplot,"e\n");
/*     //fprintf(gnuplot,"replot \'-\' notitle with lines\n"); */
    for(i = 0; i < 13; i++){
        if(cError[i] != 0.0){
            fprintf(gnuplot,"%f %f\n", (FTYPE)i, aError[i]/cError[i]);
        } else {
/*             //fprintf(gnuplot,"%f 0.0\n", (FTYPE)i); */
        }
     }
    fprintf(gnuplot,"e\n");
    fprintf(gnuplot,"quit\n");
    fclose(gnuplot);
}



void plotTotalDisplaceErrorEPS(char* filename, char* title, Image*  displacementImage, GraphDiscription novelGraph,int expNum){
    int i,j, point;
    FILE* gnuplot  = popen("gnuplot","w");
    FTYPE aError[13] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0 };
    FTYPE cError[13] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 , 0.0 };

    printf("Outputing histogram: %s\n",filename);
    assert(gnuplot);

    fprintf(gnuplot,"set xlabel \'Jet Distance\'\n");
    fprintf(gnuplot,"set ylabel \'Displacement Error\'\n");
    fprintf(gnuplot,"set xtics 2\n");
    fprintf(gnuplot,"set ytics 2\n");
    fprintf(gnuplot,"set term postscript eps color \"Helvetica\" 10\n");
/*     //fprintf(gnuplot,"set term png color\n"); */
    fprintf(gnuplot,"set size 0.5,0.5\n");
    fprintf(gnuplot,"set output \'%s\'\n",filename);
    fprintf(gnuplot,"set size 0.5,0.7\n");
    fprintf(gnuplot,"set size ratio -1\n");
/*     //fprintf(gnuplot,"set title \'%s\'\n",title); */
/*     //fprintf(gnuplot,"set notitle \n"); */
/*     //fprintf(gnuplot,"show title\n"); */
    fprintf(gnuplot,"plot [0:12] [0:12] \'-\' notitle with points lt 9 pt 7 ps 0.2, \'-\' notitle with lines lt 1 lw 3\n");
    for(point = 0; point < novelGraph->numVert; point++){
        double x = novelGraph->verts[point].x;
        double y = novelGraph->verts[point].y;
        for(j = 0; j < displacementImage[point]->height; j++){
            for(i = 0; i < displacementImage[point]->width; i++){
                double ptdist = sqrt(SQR(x-i) + SQR(y-j));
                double estx = i-ie(displacementImage[point],i,j,0);
                double esty = j-ie(displacementImage[point],i,j,1);
                double errordist = sqrt(SQR(estx - x) + SQR(esty - y));
                int index = ROUND(ptdist);

                fprintf(gnuplot,"%f %f\n",ptdist,errordist);

                if( index < 13){
                    aError[index] += errordist;
                    cError[index] += 1.0;
                }
            }
        }
    }

    fprintf(gnuplot,"e\n");
    for(i = 0; i < 13; i++){
        if(cError[i] != 0.0){
            fprintf(gnuplot,"%f %f\n", (FTYPE)i, aError[i]/cError[i]);
        } else {
/*             //fprintf(gnuplot,"%f 0.0\n", (FTYPE)i); */
        }
    }
    fprintf(gnuplot,"e\n");
    fprintf(gnuplot,"quit\n");
    fclose(gnuplot);
}




void ComputeSimilarityAndDisplacementImages(Image probe, GraphDiscription bunchGraph, Image** similarityImages, Image** displacementImages){
    int x, y;

    /* for each pixel in the new image */
    for(x = 0; x < probe->width; x++){
        for(y = 0; y < probe->height; y++){
            int point, mes, est, jet;

            /* extract a gabor jet */
            GaborJet probeJet = extractJet(x,y,probe,globals.masks);

            /* Compute all the nessesary similarity images */
            for(point = 0; point < bunchGraph->numVert; point++){
                for(mes = 0; mes < NUM_MEASURES; mes++){
                    assert(bunchGraph->bunch[point]->size > 0);
                    IE(similarityImages[mes][point],x,y,0) = globals.measures[mes](probeJet,bunchGraph->bunch[point]->jets[0]);
                    for(jet = 1; jet < bunchGraph->bunch[point]->size; jet++){
                        IE(similarityImages[mes][point],x,y,0) = MAX(IE(similarityImages[mes][point],x,y,0), globals.measures[mes](probeJet,bunchGraph->bunch[point]->jets[jet]) );
                    }
                }
            }

            /* Compute the displacement vector image */
            for(point = 0; point < bunchGraph->numVert; point++){
                for(est = 0; est < NUM_DISPLACE; est++){
                    double dx, dy, sim, bestsim;
                    assert(bunchGraph->bunch[point]->size > 0);

                    bestsim = sim = globals.estimators[est](probeJet,bunchGraph->bunch[point]->jets[0],&dx,&dy);
                    IE(displacementImages[est][point],x,y,0) = dx;
                    IE(displacementImages[est][point],x,y,1) = dy;
                    for(jet = 1; jet < bunchGraph->bunch[point]->size; jet++){
                        bestsim = sim = globals.estimators[est](probeJet,bunchGraph->bunch[point]->jets[jet],&dx,&dy);
                        if(sim > bestsim){
                            bestsim = sim;
                            IE(displacementImages[est][point],x,y,0) = dx;
                            IE(displacementImages[est][point],x,y,1) = dy;
                        }
                    }
                }
            }


            /* Free the Gabor jet */
            freeGaborJet(probeJet);

            /* Write out the progress */
            if((x*probe->height+y) % 20 == 0){
                writeProgress ("Computing Similarity and Displacement images", x*probe->height+y, probe->width*probe->height);
            }
        }
    }
}

void saveAngleAndDisplacement(Image** displacementImages, GraphDiscription bunchGraph,int expNum){
    int point,est,x,y;
    for(point = 0; point < bunchGraph->numVert; point++){
        for(est = 0; est < NUM_DISPLACE; est++){
            char imName[1024];
            Image angle = makeImage(displacementImages[est][point]->width, displacementImages[est][point]->height, 1);
            Image dist  = makeImage(displacementImages[est][point]->width, displacementImages[est][point]->height, 1);
            
            writeProgress ("Computing angle and distance images", est+point*NUM_DISPLACE, NUM_DISPLACE*bunchGraph->numVert);

            for(x = 0; x < angle->width; x++){
                for(y = 0; y < angle->height; y++){
                    FTYPE dx = bunchGraph->verts[point].x - x;
                    FTYPE dy = bunchGraph->verts[point].y - y;
                    FTYPE dl = sqrt(dx*dx + dy*dy);
                    FTYPE ex = -IE(displacementImages[est][point],x,y,0);
                    FTYPE ey = -IE(displacementImages[est][point],x,y,1);
                    FTYPE el = sqrt(ex*ex + ey*ey);
                    FTYPE l = dl*el;
                    FTYPE distx = bunchGraph->verts[point].x - (x + ex);
                    FTYPE disty = bunchGraph->verts[point].y - (y + ey);
                    FTYPE distt = sqrt(distx*distx + disty*disty);
                    IE(angle,x,y,0) = (dx*ex+dy*ey)/l;
                    IE(dist,x,y,0) = -distt < -16.0 ? -16.0 : -distt;
                }
            }
            /* make sure that the value scale is correct */
            IE(angle,0,0,0) = -1.0;
            IE(angle,1,0,0) = 1.0;
            IE(dist,0,0,0) = -16.0 ;
            IE(dist,1,0,0) = 0.0 ;
            
            /* save out images */
            sprintf(imName, "%s/exp%03d_%s_%s_ang.png", globals.outputDir, expNum, globals.disLabels[est], bunchGraph->vertLabels[point]);
            saveThermalImageMark(imName,angle,
                    bunchGraph->verts[point].x,bunchGraph->verts[point].y);

            sprintf(imName, "%s/exp%03d_%s_%s_angbw.png", globals.outputDir, expNum, globals.disLabels[est], bunchGraph->vertLabels[point]);
            saveBWImage(imName,angle);

            
            sprintf(imName, "%s/exp%03d_%s_%s_dist.png", globals.outputDir, expNum, globals.disLabels[est], bunchGraph->vertLabels[point]);
            saveThermalImageMark(imName,dist,
                    bunchGraph->verts[point].x,bunchGraph->verts[point].y);

            sprintf(imName, "%s/exp%03d_%s_%s_distbw.png", globals.outputDir, expNum, globals.disLabels[est], bunchGraph->vertLabels[point]);
            saveBWImage(imName,dist);

            freeImage(angle);
            freeImage(dist);
        }
    }
}

void runImageLocalizationExperiement(ImageList* line){
    Image novelImage;
    GraphDiscription novelGraph;
    ImageList* model;
    int i,mes,est,point;
    Image** similarityImages;
    Image** displacementImages;
    static int expNum = 0;

    printf("Running experiment for image %s\n",line->filename);
    /* read in the "novel" image */
    novelImage = readRawImage(makePath(globals.imageDir,line->filename));

    /* read in GraphDiscription for the novel image (first image in the line) */
    novelGraph = readGraphDiscription(makePath(globals.graphDir,line->filename));

    novelGraph->numVert = 17;
    /* for each of the rest of the images read them in and form the bunch graph. */
    for(model = line->next_replicate; model != NULL; model = model->next_replicate){
        Image mi = readRawImage(makePath(globals.imageDir,model->filename));
        GraphDiscription mg = readGraphDiscription(makePath(globals.graphDir,model->filename));
        for(i = 0; i < novelGraph->numVert; i++){
            GaborJet modelJet = extractJet(mg->verts[i].x,mg->verts[i].y,mi,globals.masks);
            printf("\rBuilding bunch graph from image %s (%d)    ",model->filename, i); fflush(stdout);
            addJetToBunch(novelGraph->bunch[i], modelJet);
        }
        freeImage(mi);
        freeGraphDiscription(mg);
    }
    printf("\n");

    /* Compute the similarity and displacement images */
    printf("Computing similarity and displacement images\n");

    similarityImages = malloc(sizeof(Image*)*NUM_MEASURES);
    for(mes = 0; mes < NUM_MEASURES; mes++){
        similarityImages[mes] = malloc(sizeof(Image)*novelGraph->numVert);
        for(point = 0; point < novelGraph->numVert; point++){
            similarityImages[mes][point] = duplicateImage(novelImage);
        }
    }

    displacementImages = malloc(sizeof(Image*)*NUM_DISPLACE);
    for(est = 0; est < NUM_DISPLACE; est++){
        displacementImages[est] = malloc(sizeof(Image)*novelGraph->numVert);
        for(point = 0; point < novelGraph->numVert; point++){
            displacementImages[est][point] = makeImage(novelImage->width,novelImage->height,2);
        }
    }

    ComputeSimilarityAndDisplacementImages(novelImage, novelGraph, similarityImages, displacementImages);

    /* save the similarity images */
    for(mes = 0; mes < NUM_MEASURES; mes++){
        for(point = 0; point < novelGraph->numVert; point++){
            char imName[1024];
            sprintf(imName, "%s/exp%03d_%s_%s_th.png", globals.outputDir, expNum, globals.simLabels[mes], novelGraph->vertLabels[point]);
            saveThermalImageMark(imName,similarityImages[mes][point],novelGraph->verts[point].x,novelGraph->verts[point].y);
            sprintf(imName, "%s/exp%03d_%s_%s_bw.png", globals.outputDir, expNum, globals.simLabels[mes], novelGraph->vertLabels[point]);
            saveBWImage(imName,similarityImages[mes][point]);
        }
    }

    /* compute the angle and negtive distance image */
    saveAngleAndDisplacement(displacementImages,novelGraph,expNum);

    /* plot the local vector field */
    for(est = 0; est < NUM_DISPLACE; est++){
        char imName[1024];
        for(point = 0; point < novelGraph->numVert; point++){
            sprintf(imName, "%s/exp%03d_%s_%s_vector.png", globals.outputDir, expNum, globals.disLabels[est], novelGraph->vertLabels[point]);
            plotVectorFieldPNG(imName, "Estimated Displacement Vector Field", displacementImages[est][point], expNum, novelGraph->verts[point].x, novelGraph->verts[point].y);
            sprintf(imName, "%s/exp%03d_%s_%s_vector.eps", globals.outputDir, expNum, globals.disLabels[est], novelGraph->vertLabels[point]);
            plotVectorFieldEPS(imName, "Estimated Displacement Vector Field", displacementImages[est][point], expNum, novelGraph->verts[point].x, novelGraph->verts[point].y);
            sprintf(imName, "%s/exp%03d_%s_%s_error.png", globals.outputDir, expNum, globals.disLabels[est], novelGraph->vertLabels[point]);
            plotDisplaceErrorPNG(imName, "Estimated Displacement Vector Field", displacementImages[est][point], expNum, novelGraph->verts[point].x, novelGraph->verts[point].y);
            sprintf(imName, "%s/exp%03d_%s_%s_error.eps", globals.outputDir, expNum, globals.disLabels[est], novelGraph->vertLabels[point]);
            plotDisplaceErrorEPS(imName, "Estimated Displacement Vector Field", displacementImages[est][point], expNum, novelGraph->verts[point].x, novelGraph->verts[point].y);
        }
        sprintf(imName, "%s/01exp%03d_%s_Total_error.png", globals.outputDir, expNum, globals.disLabels[est]);
        plotTotalDisplaceErrorPNG(imName, "Estimated Displacement Vector Field", displacementImages[est], novelGraph,expNum);
        sprintf(imName, "%s/01exp%03d_%s_Total_error.eps", globals.outputDir, expNum, globals.disLabels[est]);
        plotTotalDisplaceErrorEPS(imName, "Estimated Displacement Vector Field", displacementImages[est], novelGraph,expNum);
    }

    /* Output the web pages source for this experiment */
    for(point = 0; point < novelGraph->numVert; point++){
        char htmlName[1024];
        FILE* expfile;
        time_t t = time(0);
        sprintf(htmlName, "%s/00exp%03d_%s.html", globals.outputDir, expNum, novelGraph->vertLabels[point]);
        expfile = fopen(htmlName,"w");
        
        fprintf(expfile,"<HTML><HEAD><TITLE>Experiment %d: %s</TITLE></HEAD>\n<BODY>\n",expNum,novelGraph->vertLabels[point]);
        fprintf(expfile,"<H1>Experiment %d: %s</H1><BR>\n",expNum,novelGraph->vertLabels[point]);
        fprintf(expfile,"<H2>Date: %s</H1><BR>\n",ctime(&t));

        fprintf(expfile,"<HR><H3>Jet Similarity Measures</H3><BR>\n");
        fprintf(expfile,"<TABLE BORDER=2>\n");
        fprintf(expfile,"<TR>\n");
        for(mes = 0; mes < NUM_MEASURES; mes++){
            fprintf(expfile,"<TH>%s</TH>\n",globals.simLabels[mes]);
        }
        fprintf(expfile,"</TR>\n");

        fprintf(expfile,"<TR>\n");
        for(mes = 0; mes < NUM_MEASURES; mes++){
            char imName[1024];
            sprintf(imName, "exp%03d_%s_%s_bw.png", expNum, globals.simLabels[mes], novelGraph->vertLabels[point]);
            fprintf(expfile,"<TD><IMG SRC=\"%s\" WIDTH=%d HEIGHT=%d ></TD>\n",imName,novelImage->width*2, novelImage->height*2);
        }
        fprintf(expfile,"</TR><TR>\n");
        for(mes = 0; mes < NUM_MEASURES; mes++){
            char imName[1024];
            sprintf(imName, "exp%03d_%s_%s_th.png", expNum, globals.simLabels[mes], novelGraph->vertLabels[point]);
            fprintf(expfile,"<TD><IMG SRC=\"%s\" WIDTH=%d HEIGHT=%d ></TD>\n",imName,novelImage->width*2, novelImage->height*2);
        }
        fprintf(expfile,"</TR></TABLE>\n");

        
        fprintf(expfile,"<HR><H3>Jet Displacement</H3><BR>\n");
        fprintf(expfile,"<TABLE BORDER=2>\n");
        fprintf(expfile,"<TR>\n");
        for(est = 0; est < NUM_DISPLACE; est++){
            fprintf(expfile,"<TH>%s</TH>\n",globals.disLabels[est]);
        }
        fprintf(expfile,"</TR>\n");

        fprintf(expfile,"<TR>\n");
        for(est = 0; est < NUM_DISPLACE; est++){
            char imName[1024];
            fprintf(expfile,"<TD>");
            sprintf(imName, "exp%03d_%s_%s_ang.png", expNum, globals.disLabels[est], novelGraph->vertLabels[point]);
            fprintf(expfile,"<IMG SRC=\"%s\" WIDTH=%d HEIGHT=%d ><BR>",imName,novelImage->width*2, novelImage->height*2);
            sprintf(imName, "exp%03d_%s_%s_dist.png", expNum, globals.disLabels[est], novelGraph->vertLabels[point]);
            fprintf(expfile,"<IMG SRC=\"%s\" WIDTH=%d HEIGHT=%d ><BR>",imName,novelImage->width*2, novelImage->height*2);
            sprintf(imName, "exp%03d_%s_%s_vector.png", expNum, globals.disLabels[est], novelGraph->vertLabels[point]);
            fprintf(expfile,"<IMG SRC=\"%s\"><BR>",imName);
            sprintf(imName, "exp%03d_%s_%s_error.png", expNum, globals.disLabels[est], novelGraph->vertLabels[point]);
            fprintf(expfile,"<IMG SRC=\"%s\"><BR>",imName);
            fprintf(expfile,"</TD>\n");
        }
        fprintf(expfile,"</TR></TABLE>\n");
        fprintf(expfile,"</BODY></HTML>\n");
        fclose(expfile);
    }

    /* Add this experiment to the final web page list */

    /* Free image memory */
    
    freeGraphDiscription(novelGraph);
    freeImage(novelImage);
    expNum++;

}


int main(int argc,char** argv){
    ImageList* expList;
    ImageList* line;
    JetSimilarityMeasure distanceMeasures[NUM_MEASURES] = {
      &JetSimilarityMag,
      &JetSimilarityPhase,
      &JetSimilarityDEPredictiveStep,
      &JetSimilarityDEPredictiveIter,
      &JetSimilarityDEFixedLocalSearch,
      &JetSimilarityDENarrowingLocalSearch,
      &JetSimilarityCorrelation,
    };

    JetDisplacementEstimator displacementEstimators[NUM_DISPLACE] = {
        &DEPredictiveStep,
        &DEPredictiveIter,
        &DEFixedLocalSearch,
        &DENarrowingLocalSearch,
    };

    char* simLabels[NUM_MEASURES] = {
      "JetSimilarityMag",
      "JetSimilarityPhase",
      "JetSimilarityDEPredictiveStep",
      "JetSimilarityDEPredictiveIter",
      "JetSimilarityDEFixedLocalSearch",
      "JetSimilarityDENarrowingLocalSearch",
      "JetSimilarityCorrelation",
    };

    char* disLabels[NUM_DISPLACE] = {
        "DEPredictiveStep",
        "DEPredictiveIter",
        "DEFixedLocalSearch",
        "DENarrowingLocalSearch",
    };

    process_command(argc,argv);
    
    /* Set up global variables */
    globals.measures = distanceMeasures;
    globals.estimators = displacementEstimators;
    globals.simLabels = simLabels;
    globals.disLabels = disLabels;

    globals.masks = readMasksFile(globals.maskFile);


    /* Read in experiment image list */
    expList = getImageNames(globals.expSpec, NULL);
    
    /* For each line in the imagelist file conduct an experiment */
    for( line = expList; line != NULL; line = line->next_subject){
        runImageLocalizationExperiement(line);
    };

    return 0;
}

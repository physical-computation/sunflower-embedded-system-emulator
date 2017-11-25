#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "logmarkers.h"

void create_marks_csv(char *filename,double a[][2],int m,int n){
 
printf("\n Creating %s.csv file",filename);
 
FILE *fp;
 
int i,j;
 
filename=strcat(filename,".csv");
 
fp=fopen(filename,"w+");
 
fprintf(fp,"Time Steps, Accelerometer, Inferred Gyro");
 
for(i=0;i<m;i++){
 
    fprintf(fp,"\n%d",i+1);
 
    for(j=0;j<n;j++)
 
        fprintf(fp,",%f ",a[i][j]);
 
    }
 
fclose(fp);
 
printf("\n %sfile created",filename);
 
}


int startup(int argc, char *argv[]) {
	
    double ac[ 100 ];
	
    double g[ 100 ];
	
	double a[100][2] ;
	 
    int i, j;
	
LOGMARK(0);

	for ( i = 0; i < 100; i++ ) {
	
        ac[ i ] = sin( 0.5*((double)i) ) ; 
	
    	g[ i ] = sqrt (fabs(ac[i])); 
	
    	a[ i ][0] = ac[i] ; 
	
    	a[ i ][1] = g[i] ; 
	
	}


LOGMARK(1);

LOGMARK(2);

	char str[100] = "inferred";
 
	create_marks_csv(str,a,100,2);
 
return 0;
 
LOGMARK(3); 
 
}
  




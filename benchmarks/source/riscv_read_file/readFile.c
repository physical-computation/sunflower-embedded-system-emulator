#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <uncertain.h>
#include <printf.h>
#include <assert.h>

float 
Add(float a, float b) 
{
	return a+b;
}
 
int 
main(void) 
{
	float kA = 6.321;
	float kB = 9.9485;
	FILE *fp;
	fp = fopen("file.txt", "r");
	fscanf(fp, "%f\n", &kA);	
	fscanf(fp, "%f\n", &kB);
	printf("\n%f + %f = %f\n", kA, kB, Add(kA,kB));
	fclose(fp);
	return 0;
}

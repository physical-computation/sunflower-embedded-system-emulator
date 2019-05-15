#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <printf.h>

float f(float x)
{
	return x*x + 2*x - 6;
}

float df(float x)
{
	return 2*x + 2;
}

int main()
{
	float x = 0.0;
	float x_new = x - f(x)/df(x);
	float diff = x_new - x;


	while(fabs(diff) > 0.0001){
		x = x_new;
		x_new = x - f(x)/df(x);
		diff = x_new - x;
	}

	printf_("x^2 + 2x - 6 has a root at %f\n", x);

	return 0;
}

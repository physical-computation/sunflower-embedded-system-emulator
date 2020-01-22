#include <stdio.h>
/*
 *	Uncomment for RISC-V build
 */

#include <printf.h>

int
main(void)
{
	float sumf = 0.0;
	int sum = 0;

	for (int i=0; i<10; i++)
	{
		sum += i;
		sumf += i / 10.0;
	}	

	printf("Hello, World sum=%d sumf=%f.\n", sum, sumf);

	return 0;
}

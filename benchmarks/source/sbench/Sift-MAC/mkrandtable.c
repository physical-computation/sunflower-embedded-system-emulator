#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static float	pr(int r, float alpha, int CW);
static void	usage(void);

static float pr(int r, float alpha, int CW)
{
	return (1 - alpha)*pow(alpha, CW)*pow(alpha, 0 - r)
			/ (1 - pow(alpha, CW));
}

int
main(int argc, char*argv[])
{
	float	alpha;
	int	CW, basis_start, basis_end, nprobs;
	int	i, j, count;


	if (argc != 5)
	{
		usage();
		exit(-1);
	}

	alpha		= strtof(argv[1], NULL);
	CW		= strtol(argv[2], NULL, 10);
	basis_start	= strtol(argv[3], NULL, 10);
	basis_end	= strtol(argv[4], NULL, 10);

	/*									*/
	/*	Ensure that the smallest number in basis has a occurrence	*/
	/*	count of at least 1:						*/
	/*									*/
	nprobs		= (int)ceil(1/pr(basis_start, alpha, CW));


	printf("/*\talpha = %.6f, CW = %d, start = %d, end = %d\t*/\n",
		alpha, CW, basis_start, basis_end);
	printf("int	sift_randtable[] =\n\t{\n");
	count = 0;
	for (i = basis_start; i <= basis_end; i++)
	{
		int	n = nprobs * pr(i, alpha, CW);

		for (j = 0; j < n; j++)
		{
			printf("\t\t%d,\n", i);
			count++;
		}
	}
	printf("\t};\n");
	printf("enum{ SIFT_RANDTABLE_LEN = %d};\n", count);

	return 0;
}

static void
usage(void)
{
	fprintf(stderr, "\nUsage mkrandtable <alpha : float> <CW : int> <basis_start : int> <basis_end : int>\n");
}

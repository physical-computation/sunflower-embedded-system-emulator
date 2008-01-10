#include <stdio.h>
#include "myrmigki.h"
#include "ilpa.h.orig"

int
main(void)
{
	int i;

	fprintf(stdout, "Currents R0000[] =\n{\n");
	for (i = 0; i < OP_MAX; i++)
	{
		fprintf(stdout, "\t{%.2f,\t%.2f,\t%d},\n",
			R0000[i].reading1, R0000[i].reading2, R0000[i].ncycles);
	}
	fprintf(stdout, "};\n");
}

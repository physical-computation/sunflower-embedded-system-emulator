#include "sf-types.h"
#include "sh7708.h"
#include "devscc.h"
#include "print.h"
#include "bsort-input.h"


int
main(void)
{
	int i;
	int maxindex = bsort_input_len - 1;

	print("\n\n[%s]\n", bsort_input);
	while (maxindex > 0)
	{
		for (i = 0; i < maxindex; i++)
		{
			if (bsort_input[i] > bsort_input[i+1])
			{
				/*		swap		*/
				bsort_input[i] ^= bsort_input[i+1];
				bsort_input[i+1] ^= bsort_input[i];
				bsort_input[i] ^= bsort_input[i+1];
			}
		}

		maxindex--;
	}
	print("[%s]\n", bsort_input);

	return 0;
}

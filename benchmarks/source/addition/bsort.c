#include "sf-types.h"
#include "sh7708.h"
#include "devscc.h"
#include "print.h"
#include "bsort-input.h"


 



int
startup(void)
{
	int a = 100, b = 20000;
	
	int c = a /Sen b;

	print("[%d]\n", c);

	return 0;
}

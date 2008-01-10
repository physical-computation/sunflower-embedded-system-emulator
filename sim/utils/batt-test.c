#include <stdio.h>
#include <stdlib.h>
#include "myrmigki.h"

Batt*		SIM_ACTIVE_BATTERIES[MAXBATTERIES];
Batt		SIM_BATTERIES[MAXBATTERIES];
Batt*		SIM_ACTIVE_BATTERIES[MAXBATTERIES];
int		SIM_NUM_BATTERIES = 0;
int		SIM_NUM_ACTIVE_BATTERIES = 0;
uvlong		SIM_GLOBAL_CLOCK = 0;
double		SIM_GLOBAL_CYCLETIME = 16.6666E-9;
int		SIM_INFO_H2O = 0;
char            SIM_INFO_BUF[MAX_SIM_INFO_BUFSZ];
int		SIM_VERBOSE = 1;
double		BATT_QUANTUM = 1E-3;
double		SIM_GLOBAL_TIME;

void
scheduler(void *unused)
{
}

int
main(int argc, char *argv[])
{
	State	S;
	char	tmp;
	char	*ep = &tmp;
	double	force_current;


	SIM_GLOBAL_TIME = BATT_QUANTUM;
	S.logfd = 1;
	newbatt(0, 2.0);
	battery_dumpall(&S);

	force_current = strtod(argv[1], &ep);
	if (*ep != '\0')
	{
		fprintf(stderr,
			"Invalid force current supplied (\"%s\"). Exiting batt-test\n\n",
			argv[1]);
		exit(-1);
	}

	fprintf(stderr, "forcing current to %E mA\n", force_current);

	//while (SIM_BATTERIES[0].battery_remaining > 0)
	while (SIM_GLOBAL_TIME < 30.0)
	{
		SIM_GLOBAL_TIME += BATT_QUANTUM;
		battery_feed(force_current);
		battery_dumpall(&S);
	}

	return 0;
}

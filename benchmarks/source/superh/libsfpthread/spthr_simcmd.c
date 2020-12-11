#include <string.h>
#include "e-types.h"
#include "devsim7708.h"

/*									*/
/*				Simulator control			*/
/*									*/
#define SIMCMD_DATA 		((volatile unsigned char *) SUPERH_SIMCMD_DATA)
#define SIMCMD_CTL 		((volatile unsigned char *) SUPERH_SIMCMD_CTL)

void
spthr_simcmd(char *cmd)
{
	int	i, cmdlen;

	cmdlen = strlen(cmd);
	for (i = 0; i < cmdlen; i++)
	{
		*SIMCMD_DATA = *(cmd + i);
		
		if (*(cmd + i) == '\n')
		{
			break;
		}
	}
	*SIMCMD_CTL = 0;

	return;
}

#include <string.h>
#include "sf.h"
#include "mextern.h"

void
riscvdumpregs(Engine *E, State *S)
{
	int i;

	for (i = 0; i < 32; i++)
	{
		mprint(E, S, nodeinfo, "R%-2d\t\t", i);
		mbitprint(E, S, 32, S->riscv->R[i]);
		mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->riscv->R[i]);
	}

	return;
}

State *
riscvnewstate(Engine *E, double xloc, double yloc, double zloc, char *trajfilename)
{
    State   *S = superHnewstate(E, xloc, yloc, zloc, trajfilename);

    S->riscv = (RiscvState *) mcalloc(E, 1, sizeof(RiscvState), "S->riscv");
    S->dumpregs = riscvdumpregs;
	S->machinetype = MACHINE_RISCV;

    if (S->riscv == NULL)
	{
		mexit(E, "Failed to allocate memory for S->riscv.", -1);
	}
    
    S->step = riscvstep;

    return S;
}
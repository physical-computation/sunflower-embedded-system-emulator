#include <string.h>
#include <math.h>
#include "sf.h"
#include "mextern.h"

static void
print_integer_register_abi(Engine *E, State *S, ulong reg_index)
{
	// See https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md#integer-register-convention-
	const char * special_names[] = {
		"zero",
		"ra",
		"sp",
		"gp",
		"tp",
	};
	if (reg_index < 5)
	{
		mprint(E, S, nodeinfo, "%-4s", special_names[reg_index]);
	}
	else if (reg_index < 8)
	{
		mprint(E, S, nodeinfo, "t%-3u", reg_index - 5);
	}
	else if (reg_index < 10)
	{
		mprint(E, S, nodeinfo, "s%-3u", reg_index - 8);
	}
	else if (reg_index < 18)
	{
		mprint(E, S, nodeinfo, "a%-3u", reg_index - 10);
	}
	else if (reg_index < 28)
	{
		mprint(E, S, nodeinfo, "s%-3u", reg_index - 18 + 2);
	}
	else if (reg_index < 32)
	{
		mprint(E, S, nodeinfo, "t%-3u", reg_index - 28 + 3);
	}
	else
	{
		mexit(E, "Cannot get ABI name for invalid index.", -1);
	}
}

static void
print_fp_register_abi(Engine *E, State *S, ulong reg_index)
{
	// See https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md#floating-point-register-convention-
	if (reg_index < 8)
	{
		mprint(E, S, nodeinfo, "ft%-2u", reg_index);
	}
	else if (reg_index < 10)
	{
		mprint(E, S, nodeinfo, "fs%-2u", reg_index - 8);
	}
	else if (reg_index < 18)
	{
		mprint(E, S, nodeinfo, "fa%-2u", reg_index - 10);
	}
	else if (reg_index < 28)
	{
		mprint(E, S, nodeinfo, "fs%-2u", reg_index - 18 + 2);
	}
	else if (reg_index < 32)
	{
		mprint(E, S, nodeinfo, "ft%-2u", reg_index - 28 + 8);
	}
	else
	{
		mexit(E, "Cannot get ABI name for invalid index.", -1);
	}
}

tuck void
riscvstallaction(Engine *E, State *S, ulong addr, int type, int latency)
{
	/*	PAU may change VDD	*/
	if (SF_PAU_DEFINED)
	{
		pau_feed(E, S, type, addr);
	}

	/*								*/
	/*	Stall fetch unit on next access or instr in EX		*/
	/*	the stall actually occurs when in MA, since we've	*/
	/*	completed the EX wait before we get executed.		*/
	/*								*/
	if (S->superH->mem_access_type == MEM_ACCESS_IFETCH)
	{
		/*	I don't know why Philip used fetch_stall_cycles, and not		*/
		/*	IF.cycles (he doesn't remember either) but I'll leave it as it is...	*/
		S->riscv->P.fetch_stall_cycles += latency;
	}
	else
	{
		S->riscv->P.EX.cycles += latency;
	}

	/*								*/
	/*	TODO: This will have to change when we implement	*/
	/*	setjmp idea for simulating memory stalls		*/
	/*								*/

	//superH equivalent has buslocking management here

	return;
}

void
riscvdumpregs(Engine *E, State *S)
{
	int i;
	char fp_value[128];
	char * f_width;

	for (i = 0; i < 32; i++)
	{
		mprint(E, S, nodeinfo, "x%-2d\t", i);
		print_integer_register_abi(E, S, i);
		mprint(E, S, nodeinfo, "\t", i);
		mbitprint(E, S, 32, S->riscv->R[i]);
		mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->riscv->R[i]);
	}

	mprint(E, S, nodeinfo, "\n");

	for (i = 0; i < 32; i++)
	{
		mprint(E, S, nodeinfo, "f%-2d\t", i);
		print_fp_register_abi(E, S, i);
		mprint(E, S, nodeinfo, "\t", i);
		uint64_t float_bits = S->riscv->fR[i];
		if((float_bits >> 32) == 0xFFFFFFFF) //NaN boxed
		{
			rv32f_rep val;
			val.bit_value = (uint32_t)float_bits;
			snprintf(fp_value, sizeof(fp_value), "%#.8g", val.float_value);
			if (S->riscv->uncertain == NULL || !isnan(S->riscv->uncertain->registers.variances[i]))
			{
				size_t start_offset = strlen(fp_value);
				snprintf(
					fp_value + start_offset,
					sizeof(fp_value) - start_offset,
					" +- %#-.5g", sqrtf(S->riscv->uncertain->registers.variances[i])
				);
			}
			f_width = "single";
		}
		else
		{
			rv32d_rep val;
			val.bit64_value = (uint64_t)float_bits;
			snprintf(fp_value, sizeof(fp_value), "%.8g", val.double_value);
			f_width = "double";
		}
		mprint(E, S, nodeinfo, "%-23s (%s)          [0x%016llx]\n", fp_value, f_width, S->riscv->fR[i]);
	}

	// TODO: remove me!
	// uncertain_print_system(S->riscv->uncertain, stdout);

	return;
}
void/*	riscv does not have system registers	*/
riscvdumpsysregs(){}

void
riscvfatalaction(Engine *E, State *S)
{
	//superHdumptlb(E, S); Blindly copied over from superH version.
	mprint(E, S, nodeinfo, "FATAL (node %d): P.EX=[%s]\n",\
			S->NODE_ID, riscv_opstrs[S->riscv->P.EX.op]);

	return;
}

static UncertainState *
uncertainnewstate(Engine *E, char *ID)
{
	int i;
	UncertainState *S = (UncertainState *)mcalloc(E, 1, sizeof(UncertainState), ID);

	if (S == NULL)
	{
		mexit(E, "Failed to allocate memory uncertain state.", -1);
	}

	for (i = 0; i < 32; ++i) {
		uncertain_inst_sv(S, i, nan(""));
	}

	return S;
}

State *
riscvnewstate(Engine *E, double xloc, double yloc, double zloc, char *trajfilename)
{
	State *S = superHnewstate(E, xloc, yloc, zloc, trajfilename);

	S->riscv = (RiscvState *) mcalloc(E, 1, sizeof(RiscvState), "S->riscv");
	if (S->riscv == NULL)
		{
			mexit(E, "Failed to allocate memory for S->riscv.", -1);
		}

	S->riscv->uncertain = uncertainnewstate(E, "S->riscv->uncertain");

	S->dumpregs = riscvdumpregs;
	S->dumpsysregs = riscvdumpsysregs;
	S->dumppipe = riscvdumppipe;
	S->flushpipe = riscvflushpipe;

	S->fatalaction = riscvfatalaction;
	S->endian = Little;
	S->machinetype = MACHINE_RISCV;
	S->dumpdistribution = riscvdumpdistribution;
	S->stallaction = riscvstallaction;

	S->step = riscvstep;
	S->faststep = riscvfaststep;

	return S;
}

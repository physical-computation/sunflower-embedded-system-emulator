#include <string.h>
#include "sf.h"
#include "mextern.h"

static void print_integer_register_abi(Engine *E, State *S, ulong reg_index) {
  // See https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md#integer-register-convention-
  const char *special_names[] = {
      "zero", "ra", "sp", "gp", "tp",
  };
  if (reg_index < 5) {
    mprint(E, S, nodeinfo, "%-4s", special_names[reg_index]);
  } else if (reg_index < 8) {
    mprint(E, S, nodeinfo, "t%-3u", reg_index - 5);
  } else if (reg_index < 10) {
    mprint(E, S, nodeinfo, "s%-3u", reg_index - 8);
  } else if (reg_index < 18) {
    mprint(E, S, nodeinfo, "a%-3u", reg_index - 10);
  } else if (reg_index < 28) {
    mprint(E, S, nodeinfo, "s%-3u", reg_index - 18 + 2);
  } else if (reg_index < 32) {
    mprint(E, S, nodeinfo, "t%-3u", reg_index - 28 + 3);
  } else {
    mexit(E, "Cannot get ABI name for invalid index.", -1);
  }
}

static void print_fp_register_abi(Engine *E, State *S, ulong reg_index) {
  // See https://github.com/riscv/riscv-elf-psabi-doc/blob/master/riscv-elf.md#floating-point-register-convention-
  if (reg_index < 8) {
    mprint(E, S, nodeinfo, "ft%-2u", reg_index);
  } else if (reg_index < 10) {
    mprint(E, S, nodeinfo, "fs%-2u", reg_index - 8);
  } else if (reg_index < 18) {
    mprint(E, S, nodeinfo, "fa%-2u", reg_index - 10);
  } else if (reg_index < 28) {
    mprint(E, S, nodeinfo, "fs%-2u", reg_index - 18 + 2);
  } else if (reg_index < 32) {
    mprint(E, S, nodeinfo, "ft%-2u", reg_index - 28 + 8);
  } else {
    mexit(E, "Cannot get ABI name for invalid index.", -1);
  }
}

void riscvdumpregs(Engine *E, State *S) {
  int i;

  for (i = 0; i < 32; i++) {
    mprint(E, S, nodeinfo, "R%-2d\t", i);
    print_integer_register_abi(E, S, i);
    mprint(E, S, nodeinfo, "\t", i);
    mbitprint(E, S, 32, S->riscv->R[i]);
    mprint(E, S, nodeinfo, "  [0x%08lx]\n", S->riscv->R[i]);
  }

  for (i = 0; i < 32; i++) {
    mprint(E, S, nodeinfo, "fR%-2d\t", i);
    print_fp_register_abi(E, S, i);
    mprint(E, S, nodeinfo, "\t", i);
    mbit64print(E, S, 64, S->riscv->fR[i]);
    mfloatprint(E, S, S->riscv->fR[i]);
  }

  return;
}

State *riscvnewstate(Engine *E, double xloc, double yloc, double zloc,
                     char *trajfilename) {
  State *S = superHnewstate(E, xloc, yloc, zloc, trajfilename);

  S->riscv = (RiscvState *)mcalloc(E, 1, sizeof(RiscvState), "S->riscv");
  S->dumpregs = riscvdumpregs;
  S->dumppipe = riscvdumppipe;
  S->endian = Little;
  S->machinetype = MACHINE_RISCV;

  if (S->riscv == NULL) {
    mexit(E, "Failed to allocate memory for S->riscv.", -1);
  }

  S->step = riscvstep;

  return S;
}

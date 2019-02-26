# RISC-V floating-point support for Sunflower
Author: Ryan Jay Leong Voo (based on original RISC-V implementation from Zhengyang Gu)

## To build and run
In sunflower-toolchain, make sure to set OSTYPE and MACHTYPE appropriately. TARGET should be set to **riscv** and TARGET-ARCH	should be set to **riscv32-elf**. Make sure ADDITIONAL_ARCH_FLAGS is uncommented and set to **--with-arch=rv32ifd**. Continue following the instructions in README.md at the root of the repository.

After building Sunflower, go to `sunflower-simulator/benchmarks/source/riscv_newton-raphson` and run `make`. To load the program into sunflower, run `../../../sim/sf run.m`.

## Files Modified
Files from the RV32I base integer implementation were modified to integrate the RV32FD architecture. Changes include the following.

For RV32F support:
1. cache-hitachi-sh.c: Fixed endian problem in memory access.
2. decode-riscv.h: Added new constants for instructions.
3. decode-riscv.c: In riscvdecode function, added new switch cases for decoding.
4. op-riscv.c: Added functions to emulate instruction operations.
5. machine-riscv.h: Added floating-point registers, added 1 FCSR.
6. regs-riscv.h: Added enum for floating-point registers, added union type for float and uint32_t.
7. instr-riscv.h: Include new R4 type.
8. regaccess-riscv.c: Included new functions to read floating point registers.
9. mkopstr-riscv: Updated rule to create opstr-riscv.h.
10. pipeline-riscv.c: Add new support for R4 type instructions.
11. mfns.h: added function prototypes.

For RV32D support:
1. regs-riscv.h: Modified union type for fp registers
2. machine-riscv.h: Widened floating point registers
3. decode-riscv.h: New constants for instructions
4. decode-riscv.c: Added new switch cases for decoding.
5. regaccess-riscv.c: Added new typedef union for read/write to uint64_t, changed freg read write to uint64_t
6. op-riscv.c: Added new rv32D instructions
7. mkopstr-riscv: updated to create opstr-riscv.h
8. mfns.h: Added function prototypes

Test file:

Also added a test file called `riscv_newton-raphson` in `benchmarks/source/`.

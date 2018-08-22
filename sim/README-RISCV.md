The RISC-V implementation is by Zhengyang Gu

The following changes are made in each commit from the incoming pull request to merge into phillipstanleymarbell/sunflower-simulator:
* Modify makefiles and setup.conf for riscv-toolchain   147f500
 * Upgrade gcc and binutils to versions that support RISV-V
 * Change the directory of gcc from $(TOOLS)/bin to $(TOOLS)/riscv/bin to avoid gcc errors where cc1 cannot be found
 * A few other changes in compilation options to make the compilation work on my laptop
* Make cache access little endian                       0923d28
 * Inverse the order of bytes in superH(read|write)(word|long) to make memory access little endian
* Add riscv support to sunflower                        f68f15f
 * sim/devsunflower.c
  * Create RISC-V node (using riscvnewstate) instead of SuperH node by default
 * sim/main.c
  * Create RISC-V node (using riscvnewstate) instead of SuperH node by default
  * Initialize the RISC-V node and set x2 (register 2, stack pointer by convension) accordingly.
  * Remove decode cache generation. RISC-V uses 64 bits instructions, it is not possible to store the decode result of all 2^64 inputs
 * sim/main.h
  * Add RISC-V state to State type.
  * Specify PC to be uint32\_t
 * sim/machine-riscv.[ch]
  * Define the type RiscvState to store the register readings and decoded instruction in pipeline
  * Define riscvnewstate function to initialize the RISC-V node
   * We call superHnewstate to initialize memory, as we use functions from superH for memory access
   * Set the step funtion in global state to be riscvstep
   * Set the dumpregs function in global state to be riscvdumpregs
  * Define riscvdumpregs
 * sim/instr-riscv.h
  * Define the bit fields of different RISC-V instruction types
  * Define a universal struct for decoding, which marks all opcode and funct fields
 * sim/decode-riscv.c
  * Define a function to decode RISC-V instructions
 * sim/op-riscv.c
  * Defien one funtions for each RV32I instruction that emulates its effect on RiscvState
 * sim/regaccess-riscv.[ch]
  * Functions to read/write from general purpose registers
  * Enumerators for RISC-V register names
 * sim/pipeline-riscv.[ch]
  * Define the pipeline stage struct to store the instruction, decoded instruction type and the corresponding function from op-riscv.c
* Add bubble sort test for riscv                        7519e1c
 * Add a test bubblesort program to benchmarks
 * Add corresponding Makefile and run.m
* Add dumppipe support for RISC-V                       f4d8b5e
 * sim/decode-riscv.[ch]
  * Add enumerator for each RISC-V instruction
  * Store an enumerator reference to the decoded instruction in the pipeline
 * sim/pipeline-riscv.c
  * Define riscvdumppipe to print out the pipeline content
  * Set dumppipe in universal state to riscvdumppipe
  * Update riscvstep funtion to support showpipe
 * sim/mkopstr-riscv
  * Shell script to generate a mapping from instruction enumerator to string for printing
 * sim/Makefile
  * Call mkopstr-riscv at compile time

# This directory contains files to reproduce the results in *Shao-Tuan Chen and Phillip Stanley-Marbell, 2017*.

Steps to run the sunflower emulator to reproduce the result in the paper:

1. Download and build the Sunflower emulator, on the installation steps readers are advised to refer to Sunflower manual (sunflowersim-manual-and-cover.pdf) available at: https://github.com/phillipstanleymarbell/sunflower-simulator 

1. Clone this branch, as: `git clone https://github.com/shaotuanchen/sunflower-simulator/tree/ESL2017`
 
1. Open your terminal, change directory (`cd`) and navigate to `sunflower-simulator/benchmarks/source/shaotuanesl/invariant` and run `make`, This will build the implementaion of inferring angular rate from accelrometer signal application from the C program `main.c` and generate a binary in S-RECORD format, `infer.sr`.This file which would be the input to Sunflower.

1. Start the Sunflower emulator. One way to do this is to specify the entire path in your current directory in terminal by typing `/Users/yourdir/sunflower-simulator/sim/sf `, where `yourdir` should be changed appropriately to your own path.

1. After the emulator startup, load the configuration file `infer.m` which specifies the power consumption, supply voltage and clock frequency of the microprocessor ARM Cortex M0+. You can type `load infer.m` directly in the terminal if you're still in the `sunflower-simulator/benchmarks/source/shaotuanesl/invariant` directory.

1. The results will be saved in an output file `sunflower.out`. Open `sunflower.out` with a text editor, and you will see the results similar as:

```
Tag NODE0_LOGMARK_TAG_1{
Node0		User Time elapsed = 0.248921 seconds.
Node0		Instruction Simulation Rate = 0.00 Cycles/Second.
Node0		"machinetype"	=	0
Node0		"Cycletrans"	=	90
Node0		"CYCLETIME"	=	2.083333E-08
Node0		NTRANS	=	93949908
Node0		CPU-only ETOT	=	1.363683E-04 Joules
Node0		CPU-only AVG POWER	=	4.227543E-03 Watts
Node0		"ICLK"	=	1548341
Node0		"CLK"	=	1548341
Node0		"TIME"	=	3.225710E-02
Node0		"dyncnt"	=	1124291
} Tag NODE0_LOGMARK_TAG_1.
```
, where: 

The node `"CPU-only ETOT"` would be the **Total Energy** listed in column 2 of Table 2 in the paper. 

The node `"dyncnt"` would be the **Dynamic Instruction Counts** for this partiulcar application run on this particular microproccor, listed in column 3 of Table 2.

Finally, `"TIME"` would be the **CPU time** elapsed, listed in column 3 of Table 2.
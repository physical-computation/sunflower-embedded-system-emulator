# This directory contains files to reproduce the results in #Shao-Tuan Chen and Phillip Stanley-Marbell, 2017#.

Steps to run the run the sunflower emulator:

1. Download and build the Sunflower emulator, on the installation steps readers are advised to refer to Sunflower manual (sunflowersim-manual-and-cover.pdf) available at: https://github.com/phillipstanleymarbell/sunflower-simulator 

1. Clone this branch, as: `git clone https://github.com/shaotuanchen/sunflower-simulator/tree/ESL2017`
 
1. Open your terminal, change directory (`cd`) and navigate to `sunflower-simulator/benchmarks/source/shaotuanesl/invariant` and run `make`, This will build the implementaion of inferring angular rate from accelrometer signal application from the C program `main.c` and generate a binary in S-RECORD format, `infer.sr`.This file which would be the input to Sunflower.

1. 

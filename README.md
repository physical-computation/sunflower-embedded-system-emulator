[![Build Status](https://travis-ci.com/physical-computation/sunflower-simulator.svg?branch=master)](https://travis-ci.com/physical-computation/sunflower-simulator)
- - - -

![Sunflower Simulator Architecture](arch.png)

# Cloning this repository
The correct way to clone this repository to get the simulator and the submodules it depends on is:

	git clone --recursive https://github.com/physical-computation/sunflower-simulator.git

To update all submodules:

	git pull --recurse-submodules
	git submodule update --remote --recursive


# Installation instructions
Read the manual (sunflowersim-manual-and-cover.pdf) if you can. Dependencies: Building the simulator depends on GNU awk (`gawk`), on the GNU version of `bison`, and the GNU version of sed, so install them.

Edit `conf/setup.conf` to match your installation directory and system setup. On macOS, use the configuration:
```
OSTYPE          = darwin
MACHTYPE        = x86_64
```

On Linux, use the configuration
```
OSTYPE          = linux
MACHTYPE        = i386
```

To configure the emulator to default to RISC-V processor support rather than Hitachi SuperH, you will want:

```
TARGET		= riscv
TARGET-ARCH	= riscv32-elf
```

Set the paths to the GNU awk, GNU bison, and GNU sed in `conf/setup.conf`. Once you have edited `conf/setup.conf`, follow the instructions in the submodule `sunflower-toolchain` to build the cross-compiler.

# If you use Sunflower in your research, please cite it as:
Phillip Stanley-Marbell and Michael Hsiao. “Fast, Flexible, Cycle-accurate Energy Estimation”. In *Proceedings of the 2001 International Symposium on Low Power Electronics and Design, ISLPED ’01*. Huntington Beach, California, USA: ACM, pp. 141–146. ISBN: 1-58113-371-5. doi: 10.1145/ 383082.383120.

**BibTeX:**
````
@inproceedings{Stanley-Marbell:2001:FFC,
 author = {Phillip Stanley-Marbell and Michael Hsiao},
 title = {Fast, Flexible, Cycle-accurate Energy Estimation},
 booktitle = {Proceedings of the 2001 International Symposium 
              on Low Power Electronics and Design},
 series = {ISLPED '01},
 year = {2001},
 isbn = {1-58113-371-5},
 location = {Huntington Beach, California, USA},
 pages = {141--146},
 numpages = {6},
 publisher = {ACM},
 address = {New York, NY, USA},
}
````

Phillip Stanley-Marbell and Diana Marculescu. “Sunﬂower: Full-system, Embedded, Microarchitecture Evaluation”. In *Proceedings of the 2nd International Conference on High Performance Embedded Architectures and Compilers. HiPEAC’07*. Ghent, Belgium: Springer-Verlag, pp. 168–182. ISBN: 978-3-540-69337-6.

**BibTeX:**
````
@inproceedings{Stanley-Marbell:2007:SFE,
 author = {Phillip Stanley-Marbell and Diana Marculescu},
 title = {Sunflower: Full-system, Embedded, Microarchitecture Evaluation},
 booktitle = {Proceedings of the 2nd International Conference on 
              High Performance Embedded Architectures and Compilers},
 series = {HiPEAC'07},
 year = {2007},
 isbn = {978-3-540-69337-6},
 location = {Ghent, Belgium},
 pages = {168--182},
 numpages = {15},
 publisher = {Springer-Verlag},
 address = {Berlin, Heidelberg},
}
````

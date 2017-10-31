![Sunflower Simulator Architecture](http://sflr.org/images/arch.png)

Installation Instructions
=========================

Step 1
------
Read manual.pdf if you can. Building the simulator depends on `gawk`, so install it.

Step 2
------
Edit `conf/setup.conf` and set `$SUNFLOWERROOT`, and `$HOST` appropriately.  You might need to set your `$OSTYPE` environment variable in your shell if it is not already set.  Examples include `darwin` for MacOSX, and the eponymous `OpenBSD`, `linux`, and `solaris`.

Step 3
------
`cd` to `sim/` and type `make`. The build should work automatically on MacOS, Linux, and Solaris.  This builds the console version of the simulator.

Step 4
------
To build the cross-compiler, first populate `tools/source` with the appropriate distributions (there are currently empty place-holder directories therein). You should be able to obtain these distributions from:

	ftp://ftp.gnu.org/pub/gnu/gcc/gcc-4.2.4/gcc-4.2.4.tar.bz2
	ftp://ftp.gnu.org/pub/gnu/binutils/binutils-2.16.1.tar.bz2
	ftp://sources.redhat.com/pub/newlib/newlib-1.9.0.tar.gz

Notes
-----
+ There is a script in `tools/source` which will automate this for you by downloading the files using wget, and then uncompressing. You can also manually download the above into `tools/source`. You can delete or overwrite the existing empty stub directories.

+ Add `$SUNFLOWERROOT/tools/bin` to your path (substitute `$SUNFLOWERROOT` as appropriate)


+ Due to some bugs in the binutils documentation texinfo files, which are caught by newer versions of makeinfo, you must make sure that your version of makeinfo is older than 5.0 (or you can temporarily make makeinfo unexecutable so that the build skips generating the man pages). Next, type 'make cross' (see NOTE below!) from the root of the installation (the directory containing this README file). That will start the process of configuring the aforementioned distributions to build the cross-compiler suite for the simulator.

+ On Mac OS 10.5 and later, `make cross CFLAGS=-Os` will prevent the build of the cross-compiler from generating `.dylib` debugging symbol files, which currently causes many problems with autoconf related tools.

+ If the cross compiler build fails due, e.g., to difference in behavior between the `perl` or `makeinfo` expected by the GCC build versus what is on your system, you might be able to recover by going into `tools/source/gcc-4.2.4/objdir`, running `make install`, then going to `tools/` and completing the cross compilation by running `make gcc-post; make newlib`

# Cite this as
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

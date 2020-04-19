#
#	Makefile for fftw-3.3alpha1/api on Sunflowersim
#
TREEROOT	= ../../../../..

include $(TREEROOT)/conf/setup.conf

INCLUDEDIR      = $(TREEROOT)/sys/include
INCLUDES        = -I$(INCLUDEDIR) -I../ -I../rdft/ -I../dft -I../reodft -I../kernel -I../../port -I$(TREEROOT)/sys/libOS/port -I$(TREEROOT)/sys/kern/superH -I$(TREEROOT)/sim
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall
OPTFLAGS	= -gstabs3 -O0

OBJS	=\
	apiplan.o\
	configure.o\
	execute-dft-c2r.o\
	execute-dft-r2c.o\
	execute-dft.o\
	execute-r2r.o\
	execute-split-dft-c2r.o\
	execute-split-dft-r2c.o\
	execute-split-dft.o\
	execute.o\
	export-wisdom-to-file.o\
	export-wisdom-to-string.o\
	export-wisdom.o\
	f77api.o\
	flops.o\
	forget-wisdom.o\
	import-system-wisdom.o\
	import-wisdom-from-file.o\
	import-wisdom-from-string.o\
	import-wisdom.o\
	malloc.o\
	map-r2r-kind.o\
	mapflags.o\
	mkprinter-file.o\
	mktensor-iodims.o\
	mktensor-iodims64.o\
	mktensor-rowmajor.o\
	plan-dft-1d.o\
	plan-dft-2d.o\
	plan-dft-3d.o\
	plan-dft-c2r-1d.o\
	plan-dft-c2r-2d.o\
	plan-dft-c2r-3d.o\
	plan-dft-c2r.o\
	plan-dft-r2c-1d.o\
	plan-dft-r2c-2d.o\
	plan-dft-r2c-3d.o\
	plan-dft-r2c.o\
	plan-dft.o\
	plan-guru-dft-c2r.o\
	plan-guru-dft-r2c.o\
	plan-guru-dft.o\
	plan-guru-r2r.o\
	plan-guru-split-dft-c2r.o\
	plan-guru-split-dft-r2c.o\
	plan-guru-split-dft.o\
	plan-guru64-dft-c2r.o\
	plan-guru64-dft-r2c.o\
	plan-guru64-dft.o\
	plan-guru64-r2r.o\
	plan-guru64-split-dft-c2r.o\
	plan-guru64-split-dft-r2c.o\
	plan-guru64-split-dft.o\
	plan-many-dft-c2r.o\
	plan-many-dft-r2c.o\
	plan-many-dft.o\
	plan-many-r2r.o\
	plan-r2r-1d.o\
	plan-r2r-2d.o\
	plan-r2r-3d.o\
	plan-r2r.o\
	print-plan.o\
	rdft2-pad.o\
	the-planner.o\
	version.o\

all:	$(OBJS)

%.o:	%.c Makefile
	$(CC) $(INCLUDES) $(CFLAGS) $(OPTFLAGS) -c $<

clean:
	$(RM) *.o

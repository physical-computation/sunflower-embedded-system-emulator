TREEROOT	= ../../../../..
include $(TREEROOT)/conf/setup.conf

PORT            = ../../port
LIBOS		= mOS
SIMDIR		= /Users/pip/inferno20031205/libmyrmigki
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR)
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) $(OPTFLAGS)
LFLAGS       	= -Ttext $(LOADADDR) -T$(PORT)/superH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $@.map -L../../libsfpthread/
LOADADDR	= 0x08004000


HEADERS =  config global mpeg2dec getvlc 
SRC =  display getbits getblk gethdr getpic getvlc idct idctref motion mpeg2dec\
recon spatscal store subspic systems verify

MPEGTARGET = mpeg2dec

#WARNINGS = -Wall
#VERIFY = -DVERIFY

#disable this flag if you do not want bitstream element tracing 
#this will speed up the decoder some since it does not have to test
#the trace flag at several critical inner loop locations.
#TRACE = -DTRACE

#disable this flag if you do not need verbose trace, such as
#header information
#VERBOSE = -DVERBOSE

# uncomment the following two lines if you want to include X11 support

#USE_DISP = -DDISPLAY
#LIBS = -lX11

# uncomment the following two lines if you want to use shared memory
# (faster display if server and client run on the same machine)

#USE_SHMEM = -DSH_MEM
#LIBS = -lXext -lX11

# if your X11 include files / libraries are in a non standard location:
# set INCLUDEDIR to -I followed by the appropriate include file path and
# set LIBRARYDIR to -L followed by the appropriate library path and

#INCLUDEDIR = -I/usr/X11R6/include
#LIBRARYDIR = -L/usr/X11R6/lib

# Use the first one if no threading is needed
# The second USERLIBS is for thread support

#USERLIBS = -lm
#pip USERLIBS = -lm -lpthread
USERLIBS = -lm -lsfpthread

# INT_IDCT is for Integer IDCT, gives better performance. Use -DINT_IDCT to enable
# For Thread support, use the 2nd USERFLAGS and enter the desired number of threads

USE_INT_IDCT = -DINT_IDCT -DMATRIX_IDCT=0 -DORIGINAL_IDCT=0
#pip USE_THREADS = -DTHRD -DNUM_THREADS=1
USE_THREADS = -DTHRD -DNUM_THREADS=1
USE_SSE2 = -use_msasm -DSSE2

USERFLAGS = $(USE_INT_IDCT) $(USE_THREADS) 

USE_PENTIUM_4 = -vec- -march=pentium4 -mcpu=pentium4
#USE_GPROF = -p

#pip LIBDIR = /usr/lib

# Choose the appropriate compiler gcc or Intel icc

#pip CC = gcc
#pip LD = gcc       # Using GCC as the compiler for now
OPTIMIZE = -g -O4 #-Wall

#LD = /opt/intel_cc_80/bin/icc
#CC = /opt/intel_cc_80/bin/icc
#OPTIMIZE = $(USE_PENTIUM_4) $(USE_SSE2) -DUSE_ICC -g -O3 #-Wall

STATLINK = -dn # Option to force static linkage

CFLAGS = $(USE_GPROF) $(OPTIMIZE) $(USERFLAGS) $(USE_DISP) $(USE_SHMEM) $(INCLUDEDIR) $(TRACE) \
$(VERBOSE) $(VERIFY) $(WARNINGS)

OBJS = obj/init.o $(SRC:%=obj/%.o)

execs/$(MPEGTARGET).out: $(OBJS)
	$(LD) $(LFLAGS) -o execs/$(MPEGTARGET) $(OBJS) $(LIBS) $(USERLIBS) -lc -lm -lgcc -l$(LIBOS)-$(TARGET)
	$(OBJCOPY) -O srec execs/$(MPEGTARGET) execs/$(MPEGTARGET).sr
	$(OBJDUMP) -G execs/$(MPEGTARGET) > execs/$(MPEGTARGET).stabs

#pip	$(LD) $(STATLINK) $(CFLAGS) $(LIBRARYDIR) -o execs/$(MPEGTARGET) $(OBJS) $(LIBS) $(USERLIBS)

$(OBJS): $(HEADERS:%=src/%.h) $(SRC:%=src/%.c)
	$(CC) -o $@ $(CFLAGS) -c $(*:obj/%=src/%.c)

obj/init.o: $(PORT)/init.S
	$(CPP) $(PORT)/init.S > obj/init.i; $(AS) obj/init.i -o $@; rm obj/init.i

clean:
	rm -f obj/*.o execs/$(MPEGTARGET)


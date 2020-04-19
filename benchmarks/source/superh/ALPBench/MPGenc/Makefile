TREEROOT	= ../../../..
include $(TREEROOT)/conf/setup.conf

PORT		= ../../port
LIBOS		= mOS
SIMDIR		= /Users/pip/inferno20031205/libmyrmigki
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR)
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) $(OPTFLAGS)
LFLAGS       	= -Ttext $(LOADADDR) -T$(PORT)/superH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $@.map -L../../libsfpthread/
LOADADDR	= 0x08004000

HEADERS =  config global mpeg2enc vlc
SRC =  conform fdctref puthdr readpic stats putmpg idct putpic transfrm \
putseq motion mpeg2enc putvlc writepic predict quantize \
putbits ratectl

MPEGTARGET = mpeg2enc

# Use the first one if no threading is needed
# The second USERLIBS is for thread support

#USERLIBS = -lm
#pip USERLIBS = -lm -lpthread
USERLIBS = -lm -lsfpthread

# INT_IDCT is for Integer IDCT, gives better performance. Use -DINT_IDCT to 
# enable. QUIET suppresses all standard outputs, use -DQUIET to enable. For 
# thread support, use the 2nd USERFLAGS and enter the desired number of threads.

USE_THREADS = -DLTHREAD -DNUM_THREADS=1
USE_INT_DCT = -DINT_DCT -DORIGINAL_IDCT=0 -DMATRIX_IDCT=0 -DINTEL_FAST_DCT=1

USERFLAGS = $(USE_THREADS) $(USE_INT_DCT) -DQUIET

# To enable SSE2, uncomment the following line. Only available for Intel icc
USE_SSE2 = -use_msasm -DSSE2

# Uncomment USE_PENTIUM_4 if you are using this processor
# Can also profile the code by using USE_GPROF

USE_PENTIUM_4 = -vec- -march=pentium4 -mcpu=pentium4 
#USE_GPROF = -p

#pip LIBDIR = /usr/lib

# Choose the appropriate compiler gcc or Intel icc

#pip CC = gcc
#pip LD = gcc       # Using GCC as the compiler for now
OPTIMIZE = -g -O4 #-Wall

#LD = /opt/intel_cc_80/bin/icc       # Using GCC as the compiler for now
#CC = /opt/intel_cc_80/bin/icc
#OPTIMIZE = $(USE_SSE2) $(USE_PENTIUM_4) -DUSE_ICC -g -O3 #-Wall

STATLINK = -dn # Option to force static linkage

LIBS = $(USERLIBS)
CFLAGS =$(USE_GPROF) $(OPTIMIZE) $(USERFLAGS)
OBJS =  obj/init.o $(SRC:%=obj/%.o)

execs/$(MPEGTARGET).out: $(OBJS)
	$(LD) $(LFLAGS) -o execs/$(MPEGTARGET) $(OBJS) $(LIBS) $(USERLIBS) -lc -lm -lgcc -l$(LIBOS)-$(TARGET)
	$(OBJCOPY) -O srec execs/$(MPEGTARGET) execs/$(MPEGTARGET).sr
	$(OBJDUMP) -G execs/$(MPEGTARGET) > execs/$(MPEGTARGET).stabs
#pip	$(LD) $(USE_GPROF) $(STATLINK) $(USERFLAGS) -o execs/$(MPEGTARGET) $(OBJS) $(LIBS)

$(OBJS): $(HEADERS:%=src/%.h) $(SRC:%=src/%.c)
	$(CC) -o $@ $(CFLAGS) -c $(*:obj/%=src/%.c)

obj/init.o: $(PORT)/init.S
	$(CPP) $(PORT)/init.S > obj/init.i; $(AS) obj/init.i -o $@; rm obj/init.i

clean:
	rm -f obj/*.o execs/$(MPEGTARGET)



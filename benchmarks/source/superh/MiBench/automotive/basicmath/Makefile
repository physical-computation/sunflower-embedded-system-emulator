TREEROOT	= ../../../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM1	= basicmath_small
PROGRAM2	= basicmath_large

LIBOS		= eOS
SIMDIR		= $(TREEROOT)/sim
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR)
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) -Wall ##-Werror -Winline
LDFLAGS1       	= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM1).map
LDFLAGS2      	= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM2).map
LOADADDR	= 0x08004000

OBJS	=\
	init.o\
	rad2deg.o\
	cubic.o\
	isqrt.o\


all:	$(PROGRAM1).sr $(PROGRAM2).sr

$(PROGRAM1): $(OBJS) basicmath_small.o
	$(LD) $(LDFLAGS1) $(OBJS) basicmath_small.o -o $@ -lc -lm -lgcc -l$(LIBOS)-$(TARGET)

$(PROGRAM1).sr:$(PROGRAM1)
	$(OBJCOPY) -O srec $(PROGRAM1) $@
	$(OBJDUMP) -G $(PROGRAM1) > $(PROGRAM1).stabs 

$(PROGRAM2): $(OBJS) basicmath_large.o
	$(LD) $(LDFLAGS2) $(OBJS) basicmath_large.o -o $@ -lc -lm -lgcc -l$(LIBOS)-$(TARGET)

$(PROGRAM2).sr:$(PROGRAM2)
	$(OBJCOPY) -O srec $(PROGRAM2) $@
	$(OBJDUMP) -G $(PROGRAM2) > $(PROGRAM2).stabs 

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

rad2deg.o: rad2deg.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c rad2deg.c

cubic.o: cubic.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c cubic.c

isqrt.o: isqrt.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c isqrt.c

basicmath_small.o: basicmath_small.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c basicmath_small.c

basicmath_large.o: basicmath_large.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c basicmath_large.c

clean:
	$(RM) init.i *.o $(PROGRAM1) $(PROGRAM2) $(PROGRAM1).sr $(PROGRAM1).map $(PROGRAM2).sr $(PROGRAM2).map *.stabs

nuke:	clean
	$(RM) simlog.node* sunflower.out

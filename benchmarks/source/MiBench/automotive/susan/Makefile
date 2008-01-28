TREEROOT	= ../../../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM	= susan

LIBOS		= eOS
SIMDIR		= $(TREEROOT)/sim
TOOLSLIB      	= $(TREEROOT)/tools/tools-lib
INCLUDES        = -I$(TREEROOT)/sys/include -I$(TREEROOT)/sys/kern/superH -I$(SIMDIR)
OPTFLAGS	= -gstabs3 -O0
CFLAGS       	= -nostdlib -fno-builtin $(TARGET-ARCH-FLAGS) $(OPTFLAGS) -Wall ##-Werror -Winline
LDFLAGS       	= -Ttext $(LOADADDR) -TsuperH.ld -L$(TOOLSLIB)/$(TARGET) -L$(TREEROOT)/sys/libOS/$(LIBOS) -Map $(PROGRAM).map
LOADADDR	= 0x08004000

OBJS	=\
	init.o\
	susan.o\


all:	$(PROGRAM).sr

$(PROGRAM): $(OBJS) susan.o
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lc -lm -lgcc -l$(LIBOS)-$(TARGET)

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@
	$(OBJDUMP) -G $(PROGRAM) > $(PROGRAM).stabs

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

susan.o: susan.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c susan.c

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map *.stabs

nuke:	clean
	$(RM) simlog.node* sunflower.out

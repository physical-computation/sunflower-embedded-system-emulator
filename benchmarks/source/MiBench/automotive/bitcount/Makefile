TREEROOT        = ../../../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM	= bitcnts

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
	bitcnt_1.o\
	bitcnt_2.o\
	bitcnt_3.o\
	bitcnt_4.o\
	bitcnts.o\
	bitfiles.o\
	bitstrng.o\
	bstr_i.o\


all:	$(PROGRAM).sr

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lc -lm -lgcc -l$(LIBOS)-$(TARGET)

$(PROGRAM).sr:$(PROGRAM)
	$(OBJCOPY) -O srec $(PROGRAM) $@
	$(OBJDUMP) -G $(PROGRAM) > $(PROGRAM).stabs

init.o: init.S
	$(CPP) init.S > init.i; $(AS) init.i -o $@

bitcnt_1.o: bitcnt_1.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c bitcnt_1.c

bitcnt_2.o: bitcnt_2.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c bitcnt_2.c

bitcnt_3.o: bitcnt_3.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c bitcnt_3.c

bitcnt_4.o: bitcnt_4.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c bitcnt_4.c

bitcnts.o: bitcnts.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c bitcnts.c

bitfiles.o: bitfiles.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c bitfiles.c

bitstrng.o: bitstrng.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c bitstrng.c

bstr_i.o: bstr_i.c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c bstr_i.c

clean:
	$(RM) init.i *.o $(PROGRAM) $(PROGRAM).sr $(PROGRAM).map $(PROGRAM).stabs

nuke:	clean
	$(RM) simlog.node* sunflower.out

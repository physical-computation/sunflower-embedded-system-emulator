TREEROOT	= ../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= bubble
INIT		= init

OPTFLAGS	= -g -O0
INCLUDES	= -I$(PREFIX)/include
CFLAGS		= $(TARGET-ARCH-FLAGS) -Wall -Xassembler --march=rv32ifdxun
LDFLAGS		= -Ttext $(LOADADDR) -L$(TOOLSLIB)/$(TARGET) -Map $(PROGRAM).map
LOADADDR	= 0x08004000


OBJS	=\
	$(INIT).o\
	$(PROGRAM).o\


all:	$(PROGRAM).out $(PROGRAM).sr Makefile

$(INIT).o: $(INIT).S
	$(UNCERTAIN_AS) $(INIT).S -o $@

$(PROGRAM).out: $(OBJS)
	$(UNCERTAIN_LD) $(LDFLAGS) $(OBJS) -o $@ -lprintf -lc -lgcc -lgloss

$(PROGRAM).sr: $(PROGRAM).out
	$(UNCERTAIN_OBJCOPY) -O srec $(PROGRAM).out $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PROGRAM).c

clean:
	$(RM) *.o *.sr *.out *.map

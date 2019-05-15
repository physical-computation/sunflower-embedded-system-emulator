TREEROOT	= ../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= newton_rapson
INIT			= init

OPTFLAGS	= -g -O0
INCLUDES	= -I$(PREFIX)/include
CFLAGS		= $(TARGET-ARCH-FLAGS) -Wall
LDFLAGS		= -Ttext $(LOADADDR) -L$(TOOLSLIB)/$(TARGET)
LOADADDR	= 0x08004000


OBJS	=\
	$(INIT).o \
	$(PROGRAM).o \


all:	$(PROGRAM).out $(PROGRAM).sr Makefile

$(INIT).o: $(INIT).S
	$(CC) $(OPTFLAGS) -o $(INIT).o -c $(INIT).S

$(PROGRAM).out: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lprintf -lc -lgloss -lgcc

$(PROGRAM).sr: $(PROGRAM).out
	$(OBJCOPY) -O srec $(PROGRAM).out $@

$(PROGRAM).o: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -c $(PROGRAM).c

clean:
	$(RM) *.o *.out *.sr *.map

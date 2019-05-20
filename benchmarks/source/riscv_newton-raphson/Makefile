TREEROOT	= ../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= newton_rapson
INIT			= init

OPTFLAGS	= -g -O0
INCLUDES	= -I$(PREFIX)/include
CFLAGS		= $(TARGET-ARCH-FLAGS) -Wall
ASFLAGS		= --march=rv32ifdxun
LDFLAGS		= -Ttext $(LOADADDR) -L$(TOOLSLIB)/$(TARGET)
LOADADDR	= 0x08004000


OBJS	=\
	$(INIT).o \
	$(PROGRAM).o \


all:	$(PROGRAM).out $(PROGRAM).sr Makefile

$(INIT).s: $(INIT).S Makefile
	$(CPP) -o $(INIT).s -c $(INIT).S

$(PROGRAM).out: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -luncertain -lprintf -lc -lgloss -lgcc -lm
	$(UTILSBIN)/fix-uncertain-binary.py $@

$(PROGRAM).sr: $(PROGRAM).out
	$(OBJCOPY) -O srec $(PROGRAM).out $@

$(PROGRAM).s: $(PROGRAM).c Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -S -fverbose-asm -o $(PROGRAM).s -c $(PROGRAM).c
	$(UTILSBIN)/add-uncertain-instructions.py $(PROGRAM).s

%.o : %.s $(HEADERS) Makefile
	$(AS) $(ASFLAGS) -g -o $@ -c $<

clean:
	$(RM) *.s *.o *.out *.sr *.map

# disable direct compilation of .c and .S files
%.o : %.c
%.o : %.S

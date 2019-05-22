TREEROOT	= ../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= main
MADGWICK	= MadgwickAHRS
UNCERTAIN	= uncertain

OPTFLAGS	= -g -O0
INCLUDES	= -I$(PREFIX)/include
CFLAGS		= $(TARGET-ARCH-FLAGS) -Wall
ASFLAGS		= --march=rv32ifdxun
LDFLAGS		= -Ttext $(LOADADDR) -L$(TOOLSLIB)/$(TARGET) -Map=$(PROGRAM).map
LOADADDR	= 0x08004000

OBJS		=\
		init.o\
		$(MADGWICK).o\
		$(PROGRAM).o\

HEADERS		=\
		$(MADGWICK).h\

.PHONY: all clean

all:		$(PROGRAM).sr $(PROGRAM).dump Makefile

$(PROGRAM).native.out: $(PROGRAM).c $(MADGWICK).c $(HEADERS) Makefile
	$(TOOLCC) -Wall $(OPTFLAGS) $(PROGRAM).c $(MADGWICK).c -o $(PROGRAM).native.out -lm -DNATIVE

%.s : %.S Makefile
	$(CPP) -o $@ -c $<

%.s : %.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -S -fverbose-asm -o $@ -c $<
	$(UTILSBIN)/add-uncertain-instructions.py $@

%.o : %.s $(HEADERS) Makefile
	$(AS) $(ASFLAGS) -g -o $@ -c $<

$(PROGRAM).out: $(OBJS) Makefile
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -luncertain -lprintf -lc -lgcc -lgloss -lm
	$(UTILSBIN)/fix-uncertain-binary.py $@

$(PROGRAM).sr: $(PROGRAM).out Makefile
	$(OBJCOPY) -O srec $< $@

$(PROGRAM).dump: $(PROGRAM).out Makefile
	$(OBJDUMP) -dS $< > $@

clean:
	$(RM) *.o *.sr *.out *.map *.s *.dump simlog.*

# disable direct compilation of .c and .S files
%.o : %.c
%.o : %.S

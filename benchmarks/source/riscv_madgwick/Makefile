TREEROOT	= ../../..
include $(TREEROOT)/conf/setup.conf

PROGRAM		= main
MADGWICK	= MadgwickAHRS

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

init.o: init.S Makefile
	$(CPP) -o init.i init.S; $(UNCERTAIN_AS) init.i -o $@

%.s: %.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) $(OPTFLAGS) $(INCLUDES) -S -fverbose-asm -o $@ -c $<
	$(UTILSBIN)/add-uncertain-instructions.py $@

%.o: %.s $(HEADERS) Makefile
	$(UNCERTAIN_AS) $(ASFLAGS) -g -o $@ -c $<

$(PROGRAM).out: $(OBJS) Makefile
	$(UNCERTAIN_LD) $(LDFLAGS) $(OBJS) -o $@ -luncertain -lprintf -lc -lgcc -lgloss -lm
	$(UTILSBIN)/fix-uncertain-binary.py $@ $(UNCERTAIN_READELF)

$(PROGRAM).sr: $(PROGRAM).out Makefile
	$(UNCERTAIN_OBJCOPY) -O srec $< $@

$(PROGRAM).dump: $(PROGRAM).out Makefile
	$(UNCERTAIN_OBJDUMP) -dS $< > $@

clean:
	$(RM) *.o *.sr *.out *.map *.dump simlog.*

# disable direct compilation of .c and .S files
%.o : %.c
%.o : %.S

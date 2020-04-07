include ../conf/setup.conf
include config.$(OSTYPE)-$(MACHTYPE)

TARGET		= sf

BFLAGS		= -t -v -Wno-other -Wno-conflicts-sr -Wno-conflicts-rr
WFLAGS		= -Wall ##-Wmissing-prototypes ##-Wall ##-Werror -Winline -Wunreachable-code
INCLUDDIRS	= -I.

DEL		= /bin/rm -f

HEADERS =\
	batt.h\
	big-endian-hitachi-sh.h\
	memory-hierarchy.h\
	config.h\
	decode-hitachi-sh.h\
	decode-ti-msp430.h\
	decode-riscv.h\
	dev7708.h\
	devsim7708.h\
	endian-hitachi-sh.h\
	fault.h\
	help.h\
	instr-hitachi-sh.h\
	instr-riscv.h\
	uncertain_upe.h\
	latencies-riscv.h\
	latencies-hitachi-sh.h\
	listutils.h\
	little-endian-hitachi-sh.h\
	machine-hitachi-sh.h\
	machine-ti-msp430.h\
	machine-riscv.h\
	main.h\
	mextern.h\
	mfns.h\
	mmu-hitachi-sh.h\
	mversion.h\
	network-hitachi-sh.h\
	op-ti-msp430.h\
	opstr-hitachi-sh.h\
	opstr-riscv.h\
	pau.h\
	physics.h\
	mass.h\
	propulsion.h\
	pic.h\
	pipeline-hitachi-sh.h\
	pipeline-riscv.h\
	power.h\
	regs-hitachi-sh.h\
	regs-ti-msp430.h\
	regs-riscv.h\
	sf.h\
	syscalls.h\
	taint.h\

OBJS	=\
	randgen.o\
	arch-$(OSTYPE).o\
	batt.o\
	bit-utils.o\
	decode-hitachi-sh.o\
	decode-riscv.o\
	dev7708.o\
	devsim7708.o\
	fault.o\
	fdr.o\
	machine-hitachi-sh.o\
	machine-riscv.o\
	uncertain_upe.o\
	main.o\
	merror.o\
	memory-hierarchy-riscv.o\
	mmalloc.o\
	mmu-hitachi-sh.o\
	network-hitachi-sh.o\
	op-hitachi-sh.o\
	op-riscv.o\
	pau.o\
	physics.o\
	mass.o\
	propulsion.o\
	pic.o\
	pipeline-hitachi-sh.o\
	pipeline-riscv.o\
	power.o\
	regaccess-riscv.o\
	syscalls.o\
	tokenhandling.o\
	sf-hitachi-sh.o\
	sf-riscv.o\
	taint.o\
	uncertain-histogram.o\
#	decode-ti-msp430.o\
#	dev430x1xx.o\
#	machine-ti-msp430.o\
#	op-ti-msp430.o\
#	pipeline-ti-msp430.o\
#	devsim430x1xx.o\

%.o: %.c $(HEADERS) Makefile
	$(CC) $(CCFLAGS) $(WFLAGS) $(INCLUDEDIRS) $(DBGFLAGS) $(OPTFLAGS) -c $<

$(TARGET): $(OBJS) commands.tex
	$(CC) $(DBGFLAGS) $(OBJS) $(LDFLAGS) -o $@


arch-$(OSTYPE).o: arch-$(OSTYPE).c $(HEADERS) Makefile
	$(CC) $(CCFLAGS) $(WFLAGS) $(INCLUDEDIRS) $(DBGFLAGS) $(OPTFLAGS) -c arch-$(OSTYPE).c

batt-test: batt-test.c batt.c Makefile
	$(CC) $(CCFLAGS) $(WFLAGS) batt-test.c arch-$(OSTYPE).c mmalloc.c batt.c $(LDFLAGS) -o $@

op-hitachi-sh.o: op-hitachi-sh.c memory-hierarchy.c regaccess-hitachi-sh.c $(HEADERS) Makefile
##	$(LINT) $(CCFLAGS) $(INCLUDEDIRS) op-hitachi-sh.c
	$(CC) $(CCFLAGS) $(WFLAGS) $(INCLUDEDIRS) $(DBGFLAGS) $(OPTFLAGS) -c op-hitachi-sh.c -o $@

op-ti-msp430.o: op-ti-msp430.c $(HEADERS) Makefile
##	$(LINT) $(CCFLAGS) $(INCLUDEDIRS) op-ti-msp430.c
	$(CC) $(CCFLAGS) $(WFLAGS) $(INCLUDEDIRS) $(DBGFLAGS) $(OPTFLAGS) -c op-ti-msp430.c -o $@

op-riscv.o: op-riscv.c memory-hierarchy.c memory-hierarchy-riscv.c regaccess-riscv.c $(HEADERS) Makefile
##	$(LINT) $(CCFLAGS) $(INCLUDEDIRS) op-hitachi-sh.c
	$(CC) $(CCFLAGS) $(WFLAGS) $(INCLUDEDIRS) $(DBGFLAGS) $(OPTFLAGS) -c op-riscv.c -o $@

sf-hitachi-sh.o: sf-hitachi-sh.tab.c $(HEADERS) Makefile
	$(CC) $(CCFLAGS) -Wall $(INCLUDEDIRS) $(DBGFLAGS) $(OPTFLAGS) -c sf-hitachi-sh.tab.c -o $@

sf-riscv.o: sf-riscv.tab.c $(HEADERS) Makefile
	$(CC) $(CCFLAGS) -Wall $(INCLUDEDIRS) $(DBGFLAGS) $(OPTFLAGS) -c sf-riscv.tab.c -o $@

sf-hitachi-sh.tab.c: sf-hitachi-sh.y lex-hitachi-sh.c $(HEADERS) Makefile
	$(GBISON) $(BFLAGS) -p sf_superh_ sf-hitachi-sh.y

sf-riscv.tab.c: sf-riscv.y lex-riscv.c $(HEADERS) Makefile
	$(GBISON) $(BFLAGS) -p sf_riscv_ sf-riscv.y

mspasm.o: mspasm.tab.c $(HEADERS) Makefile
	$(CC) $(CCFLAGS) $(WFLAGS) $(INCLUDEDIRS) $(DBGFLAGS) $(OPTFLAGS) -c mspasm.tab.c -o $@

mspasm.tab.c: mspasm.y lex.c $(HEADERS) Makefile
	$(GBISON) $(BFLAGS) mspasm.y


mversion.h: Makefile
	echo 'char const MVERSION[] = "1.1 (build '`date '+%m-%d-%Y-%H:%M:%S'`-`whoami`@`hostname`-`uname`\)\"\; > mversion.h

opstr-hitachi-sh.h: decode-hitachi-sh.h Makefile
	./mkopstr-hitachi-sh $(GAWK) > opstr-hitachi-sh.h

opstr-ti-msp430.h: decode-ti-msp430.h Makefile
	./mkopstr-ti-msp430 $(GAWK) > opstr-ti-msp430.h

opstr-riscv.h: decode-riscv.h Makefile
	./mkopstr-riscv $(GAWK) > opstr-riscv.h

decode-hitachi-sh.h: latencies-hitachi-sh.h Makefile
	./mkdecode-hitachi-sh $(GAWK) > decode-hitachi-sh.h

decode-riscv.h: latencies-riscv.h Makefile
	./mkdecode-riscv $(GAWK) > decode-riscv.h

#	Either of the lex-* would suffice for generating the LaTeX / help command array.
#	TODO Changed to riscv for now. This should be properly implemented as a union of lex-*, considering which
#	     parts of riscv/superh(/...) are duplicates. See issue #127.
help.h: lex-hitachi-sh.c Makefile
	./mkhelp $(GAWK) > help.h

commands.tex: lex-hitachi-sh.c Makefile
	./mkmantex $(GAWK) > commands.tex



install: $(TARGET)
	cp $(TARGET) $(BIN)/

clean:
	$(DEL) $(TARGET) *.o *.core core *.tab.c mversion.h help.h commands.tex opstr-*.h decode-riscv.h decode-hitachi-sh.h parsedriver.i lex.i *.output gmon.out sunflower.out

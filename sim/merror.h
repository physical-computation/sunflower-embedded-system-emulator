/*										*/
/*		Copyright (C) 1999-2006 Phillip Stanley-Marbell.		*/
/*										*/
/*	You may redistribute and/or copy the source and binary			*/
/*	versions of this software provided:					*/
/*										*/
/*	1.	The source retains the copyright notice above, 			*/
/*		this list of conditions, and the following 			*/
/*		disclaimer.							*/
/*										*/
/*	2.	Redistributed binaries must reproduce the above			*/
/*		copyright notice, this list of conditions, and			*/
/*		the following disclaimer.					*/
/*										*/
/*	3.	The source and binaries are used for non-commercial 		*/
/*		purposes only.							*/
/*										*/
/*	4.	Permission is granted to all others, to redistribute		*/
/*		or make copies of the source or binaries, either		*/
/*		as it was received or with modifications, provided		*/
/*		the above three conditions are enforced and 			*/
/*		preserved, and that permission is granted for 			*/
/*		further redistribution, under the conditions of this		*/
/*		document.							*/
/*										*/
/*	The source is provided "as is", and no warranty is either		*/
/*	implied or explicitly granted. The authors will not be liable 		*/
/*	for any damages arising in any way out of the use or misuse 		*/
/*	of this software, even if advised of the possibility of such 		*/
/*	damage.									*/
/*										*/
/*	Contact: phillip Stanley-Marbell <pstanley@ece.cmu.edu>			*/
/*										*/	
extern char    Ebadsfout[];

/*
cache.c:			sfatal(S, "Bad NUMA destination map ID!");
cache.c:					sfatal(S, "realloc failed for X->regions[i]->values in cache.c");
cache.c:				sfatal(S, "Bad NUMA destination address access!");
cache.c:			sfatal(S, "Bad NUMA destination map ID!");
cache.c:					sfatal(S, "realloc failed for X->regions[i]->values in cache.c");
cache.c:				sfatal(S, "Bad NUMA destination address access!");
cache.c:			sfatal(S, "Bad NUMA destination map ID!");
cache.c:					sfatal(S, "realloc failed for X->regions[i]->values in cache.c");
cache.c:				sfatal(S, "Bad NUMA destination address access!");
cache.c:			sfatal(S, "Bad NUMA destination map ID!");
cache.c:				sfatal(S, "Bad NUMA destination address access!");
cache.c:						sfatal(S, "realloc failed for X->regions[i]->values in cache.c");
cache.c:		sfatal(S, "Read from an overlapping RAM/FLASH region");
cache.c:			sfatal(S, "Bad NUMA destination map ID!");
cache.c:				sfatal(S, "Bad NUMA destination address access!");
cache.c:						sfatal(S, "realloc failed for X->regions[i]->values in cache.c");
cache.c:		sfatal(S, "Read from an overlapping RAM/FLASH region");
cache.c:			sfatal(S, "Bad NUMA destination map ID!");
cache.c:				sfatal(S, "Bad NUMA destination address access!");
cache.c:						sfatal(S, "realloc failed for X->regions[i]->values in cache.c");
cache.c:		sfatal(S, "Read from an overlapping RAM/FLASH region");
dev7708.c:			sfatal(S, "Illegal read from TLB data array in user mode");
dev7708.c:			sfatal(S, "Illegal read from TLB address array in user mode");
dev7708.c:				sfatal(S, "Illegal read from PTEH in user mode");
dev7708.c:				sfatal(S, "Illegal read from PTEL in user mode");
dev7708.c:				sfatal(S, "Illegal read from TTB in user mode");
dev7708.c:				sfatal(S, "Illegal read from TEA in user mode");
dev7708.c:				sfatal(S, "Illegal read from MMUCR in user mode");
dev7708.c:			sfatal(S, "Invalid double word access.");
dev7708.c:			sfatal(S, "Address not in main mem, and not in I/O space either !");
dev7708.c:			sfatal(S, "Invalid byte access.");
dev7708.c:			sfatal(S, "Illegal write to TLB data array in user mode");
dev7708.c:			sfatal(S, "Illegal write to TLB address array in user mode");
dev7708.c:				sfatal(S, "Illegal write to MMU PTEH in user mode");
dev7708.c:				sfatal(S, "Illegal write to MMU PTEL in user mode");
dev7708.c:				sfatal(S, "Illegal write to MMU TTB in user mode");
dev7708.c:				sfatal(S, "Illegal write to MMU TEA in user mode");
dev7708.c:				sfatal(S, "Illegal write to MMU MMUCR in user mode");
dev7708.c:			sfatal(S, "Invalid long word access.");
dev7708.c:			sfatal(S, "Invalid word access.");
dev7708.c:			sfatal(S, "Write to EXCP_TRA not permitted");
dev7708.c:			sfatal(S, "Write to EXCP_EXPEVT not permitted");
dev7708.c:			sfatal(S, "Write to EXCP_INTEVT not permitted");
dev7708.c:			sfatal(S, "Invalid byte access.");
devsim7708.c:			sfatal(S, "Attempt to access NUMAREGION_RDCNT register "
devsim7708.c:			sfatal(S, "Attempt to access NUMAREGION_WRCNT register "
devsim7708.c:	sfatal(S, "Address not in main mem, and not in I/O space either !");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:		sfatal(S, "You should not be reading NIC_NMR !?");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:		sfatal(S, "You should not be reading NIC_TDR !?");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:		sfatal(S, "You should not be reading SIMCMD_DATA !?");
devsim7708.c:		sfatal(S, "You should not be reading SIMCMD_CTL !?");
devsim7708.c:			sfatal(S, "Attempt to access sensor register outside number of sensors");
devsim7708.c:		sfatal(S, "You should not be reading DEVLOGPRINT !?");
devsim7708.c:			sfatal(S, "Attempt to access NUMAREGION_RDCNT register "
devsim7708.c:			sfatal(S, "Attempt to access NUMAREGION_WRCNT register "
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:		sfatal(S, "Use of NIC_NMR deprecated. Update your application!!");
devsim7708.c:		sfatal(S, "Write to NIC_BRR ignored. What are you trying to do ?!");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:			sfatal(S, "Are you sure thats what you wanted to do ?");
devsim7708.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsim7708.c:		sfatal(S, "You should not write into NSR");
devsim7708.c:		sfatal(S, "You should not write into RDR");
devsim7708.c:		sfatal(S, "You should not write into NIC_FSZ");
devsim7708.c:		sfatal(S, "You should not write into RAND");
devsim7708.c:		sfatal(S, "You should not write into ORBIT data register");
devsim7708.c:		sfatal(S, "You should not write into VELOCITY data register");
devsim7708.c:		sfatal(S, "You should not write into XLOC data register");
devsim7708.c:		sfatal(S, "You should not write into YLOC data register");
devsim7708.c:		sfatal(S, "You should not write into ZLOC data register");
devsim7708.c:			sfatal(S, "Attempt to access sensor register outside number of sensors");
devsimmsp430.c:	sfatal(S, "Address not in main mem, and not in I/O space either !");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:		sfatal(S, "You should not be reading NIC_NMR !?");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:		sfatal(S, "You should not be reading NIC_TDR !?");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:			sfatal(S, "Attempt to access sensor register outside number of sensors");
devsimmsp430.c:		sfatal(S, "You should not be reading DEVLOGPRINT !?");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:		sfatal(S, "Use of NIC_NMR deprecated. Update your application!!");
devsimmsp430.c:		sfatal(S, "Write to NIC_BRR ignored. What are you trying to do ?!");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:			sfatal(S, "Are you sure thats what you wanted to do ?");
devsimmsp430.c:			sfatal(S, "Attempt to access IFC register outside number of IFCs");
devsimmsp430.c:		sfatal(S, "You should not write into NSR");
devsimmsp430.c:		sfatal(S, "You should not write into RDR");
devsimmsp430.c:		sfatal(S, "You should not write into NIC_FSZ");
fdr.c:		sfatal(S, "Malloc failed for Type *t in add_type()");
fdr.c:		sfatal(S, "Badly formatted stabstring in parse_type()");
fdr.c:			sfatal(S, "Badly formatted stabstring in parse_type()");
fdr.c:		sfatal(S, "Malloc failed for (char *)stab.name in m_readstabs()");
fdr.c:		sfatal(S, "Could not open STABS file");
fdr.c:				sfatal(S, "Badly formatted stabs file: RBRAC when scope depth is zero!");
fdr.c:				//sfatal(S, "Found a stack variable outside a function ?!");
fdr.c:				//sfatal(S, "Badly formatted stabs file: GSYM in non-global scope");
fdr.c:				//sfatal(S, "Found a stack variable outside a function ?!");
fdr.c:				sfatal(S, "Unknown function parameter type");
fdr.c:				//sfatal(S, "Found a stack variable outside a function ?!");
fdr.c:				sfatal(S, "Unknown RSYM type");
machine-hitachi-sh.c:superHfatalaction(State *S)
machine-hitachi-sh.c:		sfatal(S,
machine-hitachi-sh.c:			sfatal(S, "Unknown/invalid exception code");
machine-hitachi-sh.c:		sfatal(S,
machine-hitachi-sh.c:			sfatal(S, "Unknown interrupt type!");
machine-hitachi-sh.c:	S->fatalaction = superHfatalaction;
machine-ti-msp430.c:msp430fatalaction(State *S)
main.c:sfatal(State *S, char *msg)
main.c:	S->fatalaction(S);
main.c:		sfatal(S, "mcalloc failed for S->RT->regvts entry in main.c");
main.c:		sfatal(S, "mcalloc failed for S->RT->regvts->values entry in main.c");
main.c:		sfatal(S, "mcalloc failed for X->regions entry in main.c");
main.c:		sfatal(S, "mcalloc failed for X->regions->values entry in main.c");
main.c:			sfatal(SIM_STATE_PTRS[i],
main.c:			sfatal(SIM_STATE_PTRS[i],
main.c:			sfatal(S, "Could not create thread in ON call");
main.h:	void 		(*fatalaction)(State *S);
mfns.h:void	sfatal(State *, char *);
mfns.h:void	superHfatalaction(State *S);
mmu-hitachi-sh.c:sfatal(S, "stopping on tlb exception...\n");
mmu-hitachi-sh.c:sfatal(S, "stopping on tlb exception...\n");
mmu-hitachi-sh.c:sfatal(S, "stopping on tlb exception...\n");
myrmigki.y:				sfatal(CUR_STATE, $2);
network.c:			sfatal(S, "fifo_dequeue() returning NULL (TX UNDERRRUN)");
network.c:			sfatal(S, "fifo_dequeue() returning NULL (RX UNDERRUN)");
network.c:		sfatal(S, 
network.c:		sfatal(S,
op-hitachi-sh.c:			sfatal(S, "Illegal slot instruction !\n");
op-hitachi-sh.c:					sfatal(S, "unknown instr fmt in delay slot !");
op-hitachi-sh.c:			sfatal(S, "unknown instruction format in delay slot !");
op-hitachi-sh.c:		sfatal(S, "Internal simulator error: PC stack overflow");
op-hitachi-sh.c:		sfatal(S, "Internal simulator error: PC stack overflow");
op-hitachi-sh.c:		sfatal(S, "Internal simulator error: PC stack overflow");
op-hitachi-sh.c:		sfatal(S, "Internal simulator error: PC stack underflow");
pau.c:		sfatal(S, "Could not allocate memory for PAUentry *X");
pau.c:		sfatal(S, "Could not allocate memory for PAUvalids array");
pau.c:				sfatal(S, "active/transient, and stride == 0");
pau.c:			sfatal(S, "invalid food for PAU :(");
pic.c:		sfatal(S, "Could not allocate memory for (Interrupt *)tmp");
pipeline-hitachi-sh.c:				sfatal(S, "Unknown Instruction Type !!");
pipeline-hitachi-sh.c:	sfatal(S, "Exceptions are blocked and we got an exception. We don't handle this case correctly for synchronous exceptions!!!\n");
pipeline-hitachi-sh.c:			sfatal(S, "Illegal instruction.");
pipeline-hitachi-sh.c:					sfatal(S, "Unknown Instruction Type !!");
regaccess.c:					sfatal(S, "realloc failed for (*match)->values in regaccess.c");
regaccess.c:					sfatal(S, "realloc failed for (*match)->values in regaccess.c");
syscalls.c:			//sfatal(S, "Unknown syscall. Exiting.");
Makefile:WFLAGS		= -Wmissing-prototypes -Wall -Werror -Winline 
Makefile.bsd:WFLAGS	= -Wall -Winline -Wmissing-prototypes -Werror
Makefile.darwin:WFLAGS	= -Wmissing-prototypes -Wall -Werror ## -Winline 
Makefile.linux:WFLAGS	= -Wall -Winline -Wmissing-prototypes -Werror
Makefile.solaris:WFLAGS	= -Wmissing-prototypes -Wall -Werror ## -Winline 
README:(1) errors with opstr.c or help.h
TODO.old:*	define all error messages as string constants in say, "error.h"
arch-Inferno.c:merror(char *fmt, ...)
arch-OpenBSD.c:merror(char *fmt, ...)
arch-darwin.c:merror(char *fmt, ...)
arch-linux.c:merror(char *fmt, ...)
arch-solaris.c:merror(char *fmt, ...)
batt.c:                merror("Battery with ID %d not found", ID);
batt.c:                merror("Too many nodes attached to battery with ID %d", ID);
cache.c:		merror("mcalloc failed for (Cache *)C in cache_init()");
cache.c:		merror("cacheinit() failed: Invalid Cache parameters");
cache.c:		merror("mcalloc failed for C->blocks in cache_init()");
fault.c:		merror("Only \"exp\" and \"urnd\" allowed.");
fault.c:		merror("Only \"exp\" and \"urnd\" allowed.");
machine-hitachi-sh.c:			merror("mcalloc failed for N->N->regions entry in machine-hitachi-sh.c");
machine-hitachi-sh.c:			merror("mcalloc failed for N->Nstack->regions entry in machine-hitachi-sh.c");
main.c:			merror("Absurdly long SREC line !");
main.c:		merror("Cannot set current node to [%d]: ID out of range.", which);
main.c:		merror("Machine type \"%s\" not supported.", type);
main.c:		mprint(NULL, siminfo, "Sunflower Exiting (see above errors)...\n");
main.c:		merror("This machine does not know how to \"run\".");
main.c:		merror("Already ON!.");
main.h:	int	error;
mfns.h:int	yyerror(char *);
mfns.h:void	error(char *);
mfns.h:void	merror(char *fmt, ...);
mmu-hitachi-sh.c:		merror("mcalloc failed for (Cache *)S->superH->TLB in cache_init()");
mmu-hitachi-sh.c:		merror("superHtlb_init() failed: Invalid TLB parameters");
mmu-hitachi-sh.c:		merror("mcalloc failed for S->superH->TLB->blocks in superHtlb_init()");
mmu-hitachi-sh.c:		tr->error = 0;
mmu-hitachi-sh.c:		tr->error = 0;
mmu-hitachi-sh.c:			tr->error = 1;
mmu-hitachi-sh.c:mprint(NULL, siminfo, "Load/store address error for address [0x" UHLONGFMT "]...\n", vaddr);
mmu-hitachi-sh.c:		tr->error = 0;
mmu-hitachi-sh.c:		tr->error = 0;
mmu-hitachi-sh.c:			tr->error = 1;
mmu-hitachi-sh.c:		tr->error = 1;
mmu-hitachi-sh.c:		tr->error = 1;
mmu-hitachi-sh.c:		tr->error = 1;
mmu-hitachi-sh.c:		tr->error = 1;
mmu-hitachi-sh.c:	tr->error = 0;
mmu-hitachi-sh.c:		tr->error = 1;
mmu-hitachi-sh.c:		tr->error = 1;
mmu-hitachi-sh.c:	tr->error = 0;
myrmigki.y:					merror("Invalid etaLUT index.");
myrmigki.y:					merror("Could not resize etaLUT.");
myrmigki.y:					merror("Invalid VbattLUT index.");
myrmigki.y:					merror("Could not resize VbattLUT.");
myrmigki.y:					merror("Invalid VlostLUT index.");
myrmigki.y:					merror("Could not resize Vlost.");
myrmigki.y:					merror("Battery ID out of range.");
myrmigki.y:					merror("Could not change directory to \"%s\".", $2);
myrmigki.y:					merror("Node ID out of range.");
myrmigki.y:					merror("Node indeces out of range in call to MMAP!");
myrmigki.y:					merror("This machine does not know how to \"ca\".");
myrmigki.y:					merror("This machine does not know how to \"ff\".");
myrmigki.y:					merror("We don't stand a chance.");
myrmigki.y:					merror("Segment # > max. number of network segments.");
myrmigki.y:					merror("NETSEGMENT out of range.");
myrmigki.y:					merror("Segment # > max. number of network segments.");
myrmigki.y:					merror("Invalid node number.");
myrmigki.y:				merror("Command \"NODEFAILPROBFN\" unimplemented.");
myrmigki.y:				merror("Command \"NETSEGFAILPROBFN\" unimplemented.");
myrmigki.y:					merror("this machine does not know how to \"step\".");
myrmigki.y:					merror("Network interface [%d] out of range.", $2);
myrmigki.y:				merror(".align for arbitrary alignment not implemented !!!");
myrmigki.y:				merror("Invalid unsigned immediate data %s.", $1);
myrmigki.y:				merror("Invalid signed immediate data %s.", $1);
myrmigki.y:				merror("Invalid signed immediate data %s.", $2);
myrmigki.y:				merror("Invalid signed immediate data %s.", $2);
myrmigki.y:				merror("Invalid double immediate data %s.", $1);
myrmigki.y:				merror("Invalid double immediate data %s.", $2);
myrmigki.y:				merror("Invalid double immediate data %s.", $2);
myrmigki.y:				merror("Disp in terms of mem addr was not on word boundary.");
myrmigki.y:				merror("Invalid DISP ([%s]). Possibly due to a .comm.", $1);
myrmigki.y:				merror("Invalid DISP ([%s]). Possibly due to a .comm.", $2);
myrmigki.y:				merror("Invalid DISP ([%s]). Possibly due to a .comm.", $2);
myrmigki.y:yyerror(char *err)
myrmigki.y:	merror("Invalid command!");
network.c:		mprint(S, nodeinfo, "Carrier sense error\n");
network.c:			"Frame collision error (bandwidth = %d, occupancy = %d)\n",
network.c:		mprint(S, nodeinfo, "Frame error\n");
network.c:		mprint(S, nodeinfo, "Frame MAC layer address error\n");
network.c:			mprint(S, nodeinfo, "Frame MAC layer address error\n");
network.c:		merror("Network segment, [%d] is out of range.", which);
network.c:		merror("Could not allocate memory for tptr->seg2filenames[].");
network.c:		merror("Invalid netseg ID [%d] specified in \"netsegpropmodel\" command.",
network.c:		merror("Invalid sigsrc ID [%d] specified in \"netsegpropmodel\" command\n",
network.c:		merror("Segment # > max. number of network segments.");
network.c:		merror("Segment frame size is larger than max allowed.");
network.c:			merror("Maximum number of network segments reached.");
network.c:		merror("mcalloc failed for (Segbuf *)tptr->segbufs in shasm.y.");
network.c:		merror("IFC # > max. number of IFCs.");
network.c:		merror("Segment # > max. number of network segments.");
network.c:		merror("Maximum number of node IFCs attached to segment.");
network.c:		merror("Could not allocate memory for rx_fifo[].");
network.c:			merror("Could not allocate memory for rx_fifo entry.");
network.c:		merror("Could not allocate memory for tx_fifo[].");
network.c:			merror("Could not allocate memory for tx_fifo entry.");
network.c:		merror("Could not allocate memory for rx_localbuf[].");
network.c:		merror("Invalid node IFC number.");
network.c:		merror("Segment # (%d) > max. number of network segments.", whichseg);
network.c:		merror("Invalid Retry Algorithm supplied\n");
notes.tex:			the bit error rate will increase by a factor
op-hitachi-sh.c:		sfatal(S, "Internal simulator error: PC stack overflow");
op-hitachi-sh.c:		sfatal(S, "Internal simulator error: PC stack overflow");
op-hitachi-sh.c:		sfatal(S, "Internal simulator error: PC stack overflow");
op-hitachi-sh.c:		sfatal(S, "Internal simulator error: PC stack underflow");
pau.c:		merror("This machine does not know how to \"pauinfo\".");
physics.c:		merror("Signal source creation limit reached.");
physics.c:			merror("Could not allocate memory for s->description.");
physics.c:			merror("Could not allocate memory for s->trajectory_file.");
physics.c:			merror("Invalid \"# of records\" field (\"%s\") in trajectory file.", buf);
physics.c:				merror("Could not allocate memory for s->xlocs.");
physics.c:				merror("Could not allocate memory for s->ylocs.");
physics.c:				merror("Could not allocate memory for s->zlocs.");
physics.c:							merror("Invalid xloc in trajectory file.");
physics.c:							merror("Invalid yloc in trajectory file.");
physics.c:							merror("Invalid zloc in trajectory file.");
physics.c:						merror("Extra field in trajectory file.");
physics.c:			merror("Could not allocate memory for s->samples_file.");
physics.c:			merror("Could not allocate memory for s->samples.");
physics.c:			merror("Invalid data in signal samples file.");
physics.c:				merror("Could not allocate memory for s->samples.");
physics.c:		merror("Sensor or Signal index out of range.");
power.c:		merror("This machine does not know how to \"powerstats\"");
regs-hitachi-sh.h:#error "You must define your host machines endianness"
regs-hitachi-sh.h:#error "in config.$(OSTYPE)."
regs-hitachi-sh.h:#error "use -DSF_L_ENDIAN for little-endian machines"
regs-hitachi-sh.h:#error "and -DSF_B_ENDIAN for big-endian machines"
regs-hitachi-sh.h:#error "Files containing endian-dependent code are"
regs-hitachi-sh.h:#error "endian.h and regs.h"
tag.c:		merror("Tag out of range");
*/

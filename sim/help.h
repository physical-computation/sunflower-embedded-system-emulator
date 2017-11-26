Help Helpstrs[] =\
{\
	{
			"ADDVALUETRACE",
			"	Description:	Install an address monitor to track data values.",
			"	Arguments:	<name string> <base addr> <size> <onstack> <pcstart> <frameoffset>",
	},
	{
			"BATTALERTFRAC",
			"	Description:	Set battery alert level fraction.",
			"	Arguments:	none				",
	},
	{
			"BATTCF",
			"	Description:	Set Battery Vrate lowpass filter capacitance.",
			"	Arguments:	<Capacitance in Farads>	",
	},
	{
			"BATTETALUT",
			"	Description:	Set Battery etaLUT value.",
			"	Arguments:	<LUT index> <value>				",
	},
	{
			"BATTETALUTNENTRIES",
			"	Description:	Set number of etaLUT entries.",
			"	Arguments:	<number of entries>			",
	},
	{
			"BATTILEAK",
			"	Description:	Set Battery self-discharge current.",
			"	Arguments:	<Current in Amperes>	",
	},
	{
			"BATTINOMINAL",
			"	Description:	Set Battery Inominal.",
			"	Arguments:	<Inominal in Amperes>				",
	},
	{
			"BATTNODEATTACH",
			"	Description:	Attach current node to a specified battery.",
			"	Arguments:	<which battery>		",
	},
	{
			"BATTRF",
			"	Description:	Set Battery Vrate lowpass filter resistance.",
			"	Arguments:	<Resistance in Ohms>	",
	},
	{
			"BATTSTATS",
			"	Description:	Get battery statistics.",
			"	Arguments:	<which battery>					",
	},
	{
			"BATTVBATTLUT",
			"	Description:	Set Battery VbattLUT value.",
			"	Arguments:	<index> <value>				",
	},
	{
			"BATTVBATTLUTNENTRIES",
			"	Description:	Set number of VbattLUT entries.",
			"	Arguments:	<number of entries>			",
	},
	{
			"BATTVLOSTLUT",
			"	Description:	Set Battery VlostLUT value.",
			"	Arguments:	<index> <value>				",
	},
	{
			"BATTVLOSTLUTNENTRIES",
			"	Description:	Set number of VlostLUT entries.",
			"	Arguments:	<number of entries>			",
	},
	{
			"BPT",
			"	Description:	Set breakpoint.",
			"	Arguments:	<CYCLES> <ncycles on current node> | <INSTRS> <ninstrs on current node>| <SENSORREADING> <which sensor> <float value> | <GLOBALTIME> <global time in picoseconds> ",
	},
	{
			"BPTDEL",
			"	Description:	Delete breakpoint.",
			"	Arguments:	<breakpoint ID>	",
	},
	{
			"BPTLS",
			"	Description:	List breakpoints and their IDs.",
			"	Arguments:	<none>	",
	},
	{
			"C",
			"	Description:	Synonym for CACHESTATS.",
			"	Arguments:	none						",
	},
	{
			"CA",
			"	Description:	Set simulator in cycle-accurate mode.",
			"	Arguments:	none				",
	},
	{
			"CACHEINIT",
			"	Description:	Initialise cache.",
			"	Arguments:	<cache size> <block size> <set associativity> 	",
	},
	{
			"CACHEOFF",
			"	Description:	Deactivate cache.",
			"	Arguments:	none 							",
	},
	{
			"CACHESTATS",
			"	Description:	Retrieve cache access statistics.",
			"	Arguments:	none					",
	},
	{
			"CD",
			"	Description:	Change current working directory.",
			"	Arguments:	<path>				",
	},
	{
			"CLOCKINTR",
			"	Description:     Toggle enabling clock interrupts.",
			"	Arguments:	<0/1>					",
	},
	{
			"CONT",
			"	Description:	Continue execution while PC is not equal to specified PC.",
			"	Arguments:	<until PC>	",
	},
	{
			"D",
			"	Description:	Synonym for DUMPALL.",
			"	Arguments:	<filename> <tag> <prefix>						",
	},
	{
			"DEFNDIST",
			"	Description:	Define a discrete probability measure as a set of badis value probability tuples.",
			"	Arguments:	<list of basis value> <list of probabilities>	",
	},
	{
			"DELVALUETRACE",
			"	Description:	Delete an installed address monitor for tracking data values.",
			"	Arguments:	<name string> <base addr> <size> <onstack> <pcstart><frameoffset>",
	},
	{
			"DUMPALL",
			"	Description:	Dump the State structure info for all nodes to the file using given tag and prefix.",
			"	Arguments:	<filename> <tag> <prefix>	",
	},
	{
			"DUMPMEM",
			"	Description:	Show contents of memory.",
			"	Arguments:	<start mem address> <end mem address>		",
	},
	{
			"DUMPPIPE",
			"	Description:	Show the contents of the pipeline stages.",
			"	Arguments:	none				",
	},
	{
			"DUMPREGS",
			"	Description:	Show the contents of the general purpose registers.",
			"	Arguments:	none		",
	},
	{
			"DUMPSYSREGS",
			"	Description:	Show the contents of the system registers.",
			"	Arguments:	none				",
	},
	{
			"DUMPTLB",
			"	Description:	Display all TLB entries.",
			"	Arguments:	none ",
	},
	{
			"DYNINSTR",
			"	Description:	Display number of instructions executed.",
			"	Arguments:	none				",
	},
	{
			"EBATTINTR",
			"	Description:	Toggle enable low battery level interrupts.",
			"	Arguments:	<0/1>			",
	},
	{
			"EFAULTS",
			"	Description:	Enable interuppt when too many faults occur.",
			"	Arguments:	none			",
	},
	{
			"FF",
			"	Description:	Set simulator in fast functional mode.",
			"	Arguments:	none				",
	},
	{
			"FILE2NETSEG",
			"	Description:	Connect file to netseg.",
			"	Arguments:	<file>	<netseg>				",
	},
	{
			"FLTTHRESH",
			"	Description:	Set threashold for EFAULTS.",
			"	Arguments:	<threshold>					",
	},
	{
			"FORCEAVGPWR",
			"	Description:	Bypass ILPA analysis and set avg pwr consumption.",
			"	Arguments:	<avg pwr in Watts> <sleep pwr in Watts>	",
	},
	{
			"GETRANDOMSEED",
			"	Description:	Query seed used to initialize random number generation system useful for reinitializing generator to same seed for reproducibility.",
			"	Arguments:	none	",
	},
	{
			"HELP",
			"	Description:	Print list of commands.",
			"	Arguments:	none						",
	},
	{
			"HWSEEREG",
			"	Description:	Register a hardware structure or part thereof for inducement of SEEs.",
			"	Arguments:	 <structure name> <actual bits> <logical bits> <bit offset>	",
	},
	{
			"IGN",
			"	Description:	Ignore node fatalities and continue sim without pausing.",
			"	Arguments:	<0 or 1>	",
	},
	{
			"INITRANDTABLE",
			"	Description:	Set or change node location.",
			"	Arguments:	<distname> <pfun name> <basis min> <basis max> <granularity> <p1> <p2> <p3> <p4>	",
	},
	{
			"INITSEESTATE",
			"	Description:	Initialize SEE function and parameter state.",
			"	Arguments:	 <loc pfun> <loc p1> <loc p2> <loc p3> <loc p4> <bit pfun> <bit p1> <bit p2> <bit p3> <bit p4> <duration pfun> <dur p1> <dur p2> <dur p3> <dur p4>	",
	},
	{
			"L",
			"	Description:	Synonym for LOCSTATS.",
			"	Arguments:	none						",
	},
	{
			"LISTRVARS",
			"	Description:	List all structures that can be treated as rvars.",
			"	Arguments:	none		",
	},
	{
			"LOAD",
			"	Description:	Load a script file.",
			"	Arguments:	<filename>						",
	},
	{
			"LOCSTATS",
			"	Description:	Show node's current location in three-dimentional space.",
			"	Arguments:	none		",
	},
	{
			"MALLOCDEBUG",
			"	Description:	Display malloc stats.",
			"	Arguments:	none						",
	},
	{
			"MAN",
			"	Description:	Print synopsis for command usage.",
			"	Arguments:	<command name>			",
	},
	{
			"MMAP",
			"	Description:	Map memory of one simulated node into another.",
			"	Arguments:	<source> <destination>	",
	},
	{
			"N",
			"	Description:	Step through simulation for a number (default 1) of cycles.",
			"	Arguments:	[# cycles]	",
	},
	{
			"NANOPAUSE",
			"	Description:	Pause the simulation for arg nanoseconds.",
			"	Arguments:	<duration of pause in nanoseconds>		",
	},
	{
			"ND",
			"	Description:	Synonym for NETDEBUG.",
			"	Arguments:	none						",
	},
	{
			"NETCORREL",
			"	Description:	Specify correlation coefficient between failure of a network segment and failure of an IFC on a node @@NOTE that it is not using the current node so we can specify in a matrix-like form@@.",
			"	Arguments:	<which seg>	<which node>	<coefficient>	",
	},
	{
			"NETDEBUG",
			"	Description:	Show debugging information about the simulated network interface.",
			"	Arguments:	none	",
	},
	{
			"NETNEWSEG",
			"	Description:	Add a new network segment to simulation.",
			"	Arguments:	<which (if exists)> <frame bits> <propagation speed> <bitrate> <medium width> <link failure probability distribution> <link failure distribution mu> <link failure probability distribution sigma> <link failure probability distribution lambda> <link failure duration distribution> <link failure duration distribution mu> <link failure duration distribution sigma> <link failure duration distribution lambda>	",
	},
	{
			"NETNODENEWIFC",
			"	Description:	Add a new IFC to current node frame bits and segno are set at attach time.",
			"	Arguments:	<ifc num (if valid)> <tx pwr (watts)> <rx pwr (watts)> <idle pwr (watts)> <listen pwr (watts)> <fail distribution> <fail mu> <fail sigma> <fail lambda> <transmit FIFO size> <receive FIFO size>",
	},
	{
			"NETSEG2FILE",
			"	Description:	Connect netseg to file.",
			"	Arguments:	<netseg> <file>					",
	},
	{
			"NETSEGDELETE",
			"	Description:	Disable a specified network segment.",
			"	Arguments:	<which segment>			",
	},
	{
			"NETSEGFAILDURMAX",
			"	Description:	Set maximum network segment failure duration in clock cycles though actual failure duration is determined by probability distribution.",
			"	Arguments:	<duration>		",
	},
	{
			"NETSEGFAILPROB",
			"	Description:	Set probability of failure for a setseg.",
			"	Arguments:	<which segment> <probability>	",
	},
	{
			"NETSEGFAILPROBFN",
			"	Description:	Specify Netseg failure Probability Distribution Function (fxn of time).",
			"	Arguments:	<expression in terms of constants and 'pow(a",
	},
	{
			"NETSEGNICATTACH",
			"	Description:	Attach a current node's IFC to a network segment.",
			"	Arguments:	<which IFC>	<which segment>	",
	},
	{
			"NETSEGPROPMODEL",
			"	Description:	Associate a network segment with a signal propagation model.",
			"	Arguments:	<netseg ID> <sigsrc ID> <minimum SNR>	",
	},
	{
			"NEWBATT",
			"	Description:	New battery",
			"	Arguments:	<ID> <capacity in mAh>					",
	},
	{
			"NEWNODE",
			"	Description:	Create a new node (simulated system).",
			"	Arguments:	<type=superH|msp430> [<x location> <y location> <z location>] [<trajectory file name> <loopsamples> <picoseconds per trajectory sample>]	",
	},
	{
			"NI",
			"	Description:	Synonym for DYNINSTR.",
			"	Arguments:	none						",
	},
	{
			"NODEFAILDURMAX",
			"	Description:	Set maximum node failure duration in clock cycles though actual failure duration is determined by probability distribution.",
			"	Arguments:		<duration>		",
	},
	{
			"NODEFAILPROB",
			"	Description:	Set probability of failure for current node.",
			"	Arguments:	<probability>		",
	},
	{
			"NODEFAILPROBFN",
			"	Description:	Specify Node failure Probability Distribution Function (fxn of time).",
			"	Arguments:	<expression in terms of constants and 'pow(a",
	},
	{
			"NUMAREGION",
			"	Description:	Specify a memory access latency and a node mapping (can only map into destination RAM) for an address range for a private mapping.",
			"	Arguments:	<name string> <start address (inclusive)> <end address (non-inclusive)> <local read latency in cycles> <local write latency in cycles> <remote read latency in cycles> <remote write latency in cycles> <Map ID> <Map offset> <private flag>",
	},
	{
			"NUMASETMAPID",
			"	Description:	Change the mapid for nth map table entry on all nodes to i.",
			"	Arguments:	<n> <i> ",
	},
	{
			"NUMASTATS",
			"	Description:	Display access statistics for all NUMA regions for current node.",
			"	Arguments:	none ",
	},
	{
			"NUMASTATSALL",
			"	Description:	Display access statistics for all NUMA regions for all nodes.",
			"	Arguments:	none ",
	},
	{
			"OFF",
			"	Description:	Turn the simulator off.",
			"	Arguments:	none						",
	},
	{
			"ON",
			"	Description:	Turn the simulator on.",
			"	Arguments:	none						",
	},
	{
			"PARSEOBJDUMP",
			"	Description:	Parse a GNU objdump file and load into memory.",
			"	Arguments:	<objdump file path> ",
	},
	{
			"PAUINFO",
			"	Description:	Show information about all valid PAU entries.",
			"	Arguments:	none			",
	},
	{
			"PAUSE",
			"	Description:	Pause the simulation for arg seconds.",
			"	Arguments:	<duration of pause in seconds>		",
	},
	{
			"PCBT",
			"	Description:	Dump PC backtrace.",
			"	Arguments:	none	",
	},
	{
			"PD",
			"	Description:	Disable simulation of processor's pipeline.",
			"	Arguments:	none			",
	},
	{
			"PE",
			"	Description:	Enable simulation of processor's pipeline.",
			"	Arguments:	none				",
	},
	{
			"PF",
			"	Description:	Flush the pipeline.",
			"	Arguments:	none						",
	},
	{
			"PFUN",
			"	Description:	Change probability distrib fxn (default is uniform).",
			"	Arguments:	none		",
	},
	{
			"PI",
			"	Description:	Synonym for PAUINFO.",
			"	Arguments:	none						",
	},
	{
			"POWERSTATS",
			"	Description:	Show estimated energy and circuit activity.",
			"	Arguments:	none			",
	},
	{
			"POWERTOTAL",
			"	Description:	Print total power accross all node.",
			"	Arguments:	none				",
	},
	{
			"PS",
			"	Description:	Synonym for POWERSTATS.",
			"	Arguments:	none						",
	},
	{
			"PWD",
			"	Description:	Get current working directory.",
			"	Arguments:	none					",
	},
	{
			"Q",
			"	Description:	Synonym for QUIT.",
			"	Arguments:	none							",
	},
	{
			"QUIT",
			"	Description:	Exit the simulator.",
			"	Arguments:	none						",
	},
	{
			"R",
			"	Description:	Synonym for RATIO.",
			"	Arguments:	<none>						",
	},
	{
			"RANDPRINT",
			"	Description:	Print a random value from the selected distribution with given parameters.",
			"	Arguments:	<distribution name> <min> <max> <p1> <p2> <p3> <p4> ",
	},
	{
			"RATIO",
			"	Description:	Print ratio of cycles spent active to those spent sleeping.",
			"	Arguments:	none	",
	},
	{
			"REGISTERRVAR",
			"	Description:	Register a simulator internal implementation variable or structure for periodic updates either overwriting values or summing determined by the mode parameter.",
			"	Arguments:	 <sim var name> <index for array structures> <value dist name> <value dist p1> <value dist p2> <value dist p3> <value dist p4> <duration dist name> <duration dist p1> <duration dist p2> <duration dist p3> <duration dist p4> <mode>	",
	},
	{
			"REGISTERSTABS",
			"	Description:	Register variables in a STABS file with value tracing framework.",
			"	Arguments:	<STABS filename>	",
	},
	{
			"RENUMBERNODES",
			"	Description:	Renumber nodes based on base node ID.",
			"	Arguments:	none				",
	},
	{
			"RESETALLCTRS",
			"	Description:	Reset simulation rate measurement trip counters for all nodes.",
			"	Arguments:	none				",
	},
	{
			"RESETCPU",
			"	Description:	Reset entire simulated CPU state.",
			"	Arguments:	none					",
	},
	{
			"RESETNODECTRS",
			"	Description:	Reset simulation rate measurement trip counters for current node only.",
			"	Arguments:	none				",
	},
	{
			"RETRYALG",
			"	Description:	set NIC retransmission backoff algorithm.",
			"	Arguments:	<ifc #> <algname>		",
	},
	{
			"RUN",
			"	Description:	Mark a node as runnable.",
			"	Arguments:	 none						",
	},
	{
			"SAVE",
			"	Description:	Dump memory region to disk.",
			"	Arguments:	<start mem addr> <end mem addr> <filename>	",
	},
	{
			"SENSORSDEBUG",
			"	Description:	Display various statistics on sensors and signals.",
			"	Arguments:	none 		",
	},
	{
			"SETBASENODEID",
			"	Description:	Set ID of first node from which all node IDs will be offset.",
			"	Arguments:	<integer>	",
	},
	{
			"SETBATT",
			"	Description:	Set current battery.",
			"	Arguments:	<Battery ID>					",
	},
	{
			"SETBATTFEEDPERIOD",
			"	Description:	Set update periodicity for battery simulation.",
			"	Arguments:	<period in picoseconds> ",
	},
	{
			"SETDUMPPWRPERIOD",
			"	Description:	Set periodicity power logging to simlog.",
			"	Arguments:	<period in picoseconds> 	",
	},
	{
			"SETFAULTPERIOD",
			"	Description:	Set period for activating fault scheduling.",
			"	Arguments:	<period in picoseconds>	",
	},
	{
			"SETFLASHRLATENCY",
			"	Description:	Set flash read latency.",
			"	Arguments:	 <latency in clock cycles>	",
	},
	{
			"SETFLASHWLATENCY",
			"	Description:	Set flash write latency.",
			"	Arguments:	 <latency in clock cycles>	",
	},
	{
			"SETFREQ",
			"	Description:	Set operating frequency from voltage.",
			"	Arguments:	<freq/MHz> (double)	",
	},
	{
			"SETIFCOUI",
			"	Description:	Set OUI for current IFC.",
			"	Arguments:	<which IFC> <new OUI>				",
	},
	{
			"SETLOC",
			"	Description:	Set or change node location.",
			"	Arguments:	<xloc> <yloc> <zloc>	",
	},
	{
			"SETMEMRLATENCY",
			"	Description:	Set memory read latency.",
			"	Arguments:	 <latency in clock cycles>	",
	},
	{
			"SETMEMWLATENCY",
			"	Description:	Set memory write latency.",
			"	Arguments:	 <latency in clock cycles>	",
	},
	{
			"SETNETPERIOD",
			"	Description:	Set period for activting network scheduling.",
			"	Arguments:	<period in picoseconds>	",
	},
	{
			"SETNODE",
			"	Description:	Set the current simulated node.",
			"	Arguments:	<node id>				",
	},
	{
			"SETPC",
			"	Description:	Set the value of the program counter.",
			"	Arguments:	<PC value>			",
	},
	{
			"SETPHYSICSPERIOD",
			"	Description:	Set update periodicity for physical phenomenon simulation.",
			"	Arguments:	<period in picoseconds> ",
	},
	{
			"SETQUANTUM",
			"	Description:	Set simulation instruction group quantum.",
			"	Arguments:	<integer>			",
	},
	{
			"SETRANDOMSEED",
			"	Description:	Reinitialize random number generation system with a specific seed useful in conjunction with GETRANDOMSEED for reproducing same pseudorandom state.",
			"	Arguments:	<seed value negative one to use current time>	",
	},
	{
			"SETSCALEALPHA",
			"	Description:	Set technology alpha parameter for use in voltage scaling.",
			"	Arguments:	<double>		",
	},
	{
			"SETSCALEK",
			"	Description:	Set technology K parameter for use in voltage scaling.",
			"	Arguments:	<double>		",
	},
	{
			"SETSCALEVT",
			"	Description:	Set technology Vt for use in voltage scaling.",
			"	Arguments:	<double>		",
	},
	{
			"SETSCHEDRANDOM",
			"	Description:	Use a different random order for node simulation every cycle.",
			"	Arguments:	<none>	",
	},
	{
			"SETSCHEDROUNDROBIN",
			"	Description:	Use a round-robin order for node simulation.",
			"	Arguments:	<none>			",
	},
	{
			"SETTIMERDELAY",
			"	Description:	Change granularity of timer intrs.",
			"	Arguments:	<granularity in microseconds>	",
	},
	{
			"SETVDD",
			"	Description:	Set operating voltage from frequency.",
			"	Arguments:	<Vdd/volts>	(double)		",
	},
	{
			"SFATAL",
			"	Description:	Induce a node death and state dump.",
			"	Arguments:	<suicide note> ",
	},
	{
			"SHAREBUS",
			"	Description:	Share bus structure with ther named node.",
			"	Arguments:	<Bus donor nodeid> ",
	},
	{
			"SHOWCLK",
			"	Description:	Show the number of clock cycles simulated since processor reset.",
			"	Arguments:	none	",
	},
	{
			"SHOWPIPE",
			"	Description:	Show contents of the processor pipeline.",
			"	Arguments:	none				",
	},
	{
			"SIGSRC",
			"	Description:	Create a physical phenomenon signal source.",
			"	Arguments:	<type> <description> <tau> <propagationspeed> <A> <B> <C> <D> <E> <F> <G> <H> <I> <K> <m> <n> <o> <p> <q> <r> <s> <t> <x> <y> <z> <trajectoryfile> <trajectoryrate> <looptrajectory> <samplesfile> <samplerate> <fixedsampleval> <loopsamples>	",
	},
	{
			"SIGSUBSCRIBE",
			"	Description:	Subscribe sensor X on the current node to a signal source Y.",
			"	Arguments:	<X> <Y>	",
	},
	{
			"SIZEMEM",
			"	Description:	Set the size of memory.",
			"	Arguments:	<size of memory in bytes>			",
	},
	{
			"SPLIT",
			"	Description:	Split current CPU to execute from a new PC and stack.",
			"	Arguments:	<newpc> <newstackaddr> <argaddr> <newcpuidstr>",
	},
	{
			"SRECL",
			"	Description:	Load a binary program in Motorola S-Record format.",
			"	Arguments:	none			",
	},
	{
			"STOP",
			"	Description:	Mark the current node as unrunnable.",
			"	Arguments:	none				",
	},
	{
			"THROTTLE",
			"	Description:	Set the throttling delay in nanoseconds.",
			"	Arguments:	<throttle delay in nanoseconds>				",
	},
	{
			"THROTTLEWIN",
			"	Description:	Set the throttling window --- main simulation loop sleeps for throttlensecs x throttlewin nanosecs every throttlewin simulation cycles",
			"	Arguments:	 for an average of throttlensecs sleep per simulation cycle.",
	},
	{
			"TRACE",
			"	Description:	Toggle Tracing.",
			"	Arguments:	none							",
	},
	{
			"V",
			"	Description:	Synonym for VERBOSE.",
			"	Arguments:	none						",
	},
	{
			"VALUESTATS",
			"	Description:	Print data value tracking statistics.",
			"	Arguments:	none",
	},
	{
			"VERBOSE",
			"	Description:	Enable the various prints.",
			"	Arguments:	none						",
	},
	{
			"VERSION",
			"	Description:	Display the simulator version and build.",
			"	Arguments:	none				",
	},
	{
			"NODETACH",
			"	Description:     Set whether new thread should be spawned on a ON command.",
			"	Arguments:	<0 or 1>	",
	},
	{
			"SIZEPAU",
			"	Description:     Set the size of the PAU.",
			"	Arguments:	<size of PAU in number of entries>            ",
	},
};
int	nHelpstrs = 152;
int	uniqHelpstrs = 139;

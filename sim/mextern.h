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
extern			Currents R0000[];
extern Batt		SIM_BATTERIES[];
extern Batt*		SIM_ACTIVE_BATTERIES[];
extern Currents		R0000[];
extern SuperHDCEntry	superHDC[];
extern Input 		istream;
extern Labels		labellist;
extern Machinetype	SIM_MACHINE_TYPE;
extern Netsegment	SIM_NET_SEGMENTS[];
extern State		*CUR_STATE;
extern State		*SIM_STATE_PTRS[];
extern char 		*opstrs[];
extern char            	SIM_INFO_BUF[];
extern char const	MVERSION[];
extern char*		opstrs[];
extern double		SIM_SCALE_ALPHA;
extern double		SIM_SCALE_K;
extern double		SIM_SCALE_VT;
extern int		CUR_BATT_ID;
extern int		MEMBASE;
extern int		MEMSIZE;
extern int		PAU_DEFINED;
extern int		PAU_NENTRIES;
extern int		SCANNING;
extern ulong		SIM_THROTTLE_NANOSEC;
extern ulong		SIM_THROTTLE_WINDOW;
extern int		SIM_ACTIVE_NETSEGS[];
extern int		SIM_BASENODEID;
extern int		SIM_IGNORE_NODEDEATHS;
extern int		SIM_MIN_MODULUS;
extern int		SIM_MIN_MODULUS_WHO;
extern int		SIM_NODETACH;
extern int		SIM_NUM_ACTIVE_BATTERIES;
extern int		SIM_NUM_DEPLETED_BATTERIES;
extern int		SIM_NUM_ACTIVE_NETSEGS;
extern int		SIM_NUM_BATTERIES;
extern int		SIM_NUM_NET_SEGMENTS;
extern int		SIM_NUM_NODES;
extern int		SIM_ON;
extern int		SIM_QUANTUM;
extern int		SIM_VERBOSE;
extern int 		SIM_NUM_NODES;
extern int             	SIM_INFO_H2O;
extern double		SIM_MIN_CYCLETIME;
extern double		SIM_MAX_CYCLETIME;
extern ulong		PAU_ADDR_MASK;
extern uvlong		SIM_GLOBAL_CLOCK;
extern double		SIM_GLOBAL_TIME;
extern uvlong		SIM_NIC_BYTES_SENT;
extern char		SIM_NETIO_BUF[MAX_NETIO_NBUFS][MAX_SEGBUF_TEXT];
extern int		SIM_NETIO_H2O;
extern Signalsrc	SIM_SIGSRCS[];
extern int		SIM_NUM_SIGSRCS;
extern double		SIM_PHYSICS_LASTACTIVATE;
extern double		SIM_PHYSICS_PERIOD;
extern double		SIM_BATTFEED_PERIOD;
extern double		SIM_BATT_LEAK_CURRENT;
extern double		SIM_DUMPPWR_PERIOD;
extern double		SIM_BATTFEED_LASTACTIVATE;
extern double		SIM_DUMPPWR_LASTACTIVATE;
extern double		SIM_NETWORK_PERIOD;
extern double		SIM_NETWORK_LASTACTIVATE;
extern double		SIM_FAULT_PERIOD;
extern double		SIM_FAULT_LASTACTIVATE;

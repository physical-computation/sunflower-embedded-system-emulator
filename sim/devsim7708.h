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

/*									*/
/*	Simulator specific additions to the SH arch (NIC, etc.)		*/
/*	Could probably have been done more cleanly by modeling them	*/
/*	as off-chip peripherals. We use the address range 0xE0000000	*/
/*	to 0xEFFFFFFF because it is unused/reserved on the SH7706.	*/
/*									*/
enum
{
	SUPERH_DEVSIM_TOP		= 0xEFFF0000,

	SUPERH_NIC_OUI_BYTES		= 0x10,
	SUPERH_NIC_DST_BYTES		= 0x10,
	SUPERH_NIC_REG_SPACING		= 0x00010000,

	SUPERH_NIC_NMR 			= SUPERH_DEVSIM_TOP,
	SUPERH_NIC_BRR 			= SUPERH_NIC_NMR - SUPERH_NIC_REG_SPACING,
	SUPERH_NIC_NCR 			= SUPERH_NIC_BRR - SUPERH_NIC_REG_SPACING,
	SUPERH_NIC_TDR 			= SUPERH_NIC_NCR - SUPERH_NIC_REG_SPACING,
	SUPERH_NIC_NSR 			= SUPERH_NIC_TDR - SUPERH_NIC_REG_SPACING,
	SUPERH_NIC_RDR 			= SUPERH_NIC_NSR - SUPERH_NIC_REG_SPACING,
	SUPERH_NIC_FSZ			= SUPERH_NIC_RDR - SUPERH_NIC_REG_SPACING,
	SUPERH_NIC_OUI 			= SUPERH_NIC_FSZ - SUPERH_NIC_REG_SPACING,
	SUPERH_NIC_DST 			= SUPERH_NIC_OUI - SUPERH_NIC_REG_SPACING,
	SUPERH_NIC_NCSENSE 		= SUPERH_NIC_DST - SUPERH_NIC_REG_SPACING,
	SUPERH_NIC_NCOLLS 		= SUPERH_NIC_NCSENSE - SUPERH_NIC_REG_SPACING,

	SUPERH_DEVLOGPRINT		= SUPERH_NIC_NCOLLS - 1,

	SUPERH_USECS_END		= SUPERH_DEVLOGPRINT,
	SUPERH_USECS_BEGIN		= SUPERH_DEVLOGPRINT - 4,

	SUPERH_SIMCMD_CTL		= SUPERH_USECS_BEGIN - 1,
	SUPERH_SIMCMD_DATA		= SUPERH_SIMCMD_CTL - 1,

	SUPERH_RAND_END			= SUPERH_SIMCMD_DATA,
	SUPERH_RAND_BEGIN		= SUPERH_SIMCMD_DATA - 4,

	SUPERH_ORBIT_END 		= SUPERH_RAND_BEGIN,
	SUPERH_ORBIT_BEGIN 		= SUPERH_RAND_BEGIN - 4,
	SUPERH_VELOCITY_END 		= SUPERH_ORBIT_BEGIN,
	SUPERH_VELOCITY_BEGIN 		= SUPERH_ORBIT_BEGIN - 8,
	SUPERH_XLOC_END 		= SUPERH_VELOCITY_BEGIN,
	SUPERH_XLOC_BEGIN 		= SUPERH_VELOCITY_BEGIN - 8,
	SUPERH_YLOC_END 		= SUPERH_XLOC_BEGIN,
	SUPERH_YLOC_BEGIN 		= SUPERH_XLOC_BEGIN - 8,
	SUPERH_ZLOC_END 		= SUPERH_YLOC_BEGIN,
	SUPERH_ZLOC_BEGIN 		= SUPERH_YLOC_BEGIN - 8,

	SUPERH_SENSOR_REG_SPACING 	= 0x00010000,
	SUPERH_SENSREAD_END		= SUPERH_ZLOC_BEGIN,
	SUPERH_SENSREAD_BEGIN		= SUPERH_ZLOC_BEGIN - SUPERH_SENSOR_REG_SPACING,
	SUPERH_SENSWRITE_END		= SUPERH_SENSREAD_BEGIN,
	SUPERH_SENSWRITE_BEGIN		= SUPERH_SENSREAD_BEGIN - SUPERH_SENSOR_REG_SPACING,

	SUPERH_THREADID			= SUPERH_SENSWRITE_BEGIN - 4,

	SUPERH_NUMAREGION_REG_SPACING 	= 0x00010000,
	SUPERH_NUMAREGION_RDCNT_END	= SUPERH_THREADID,
	SUPERH_NUMAREGION_RDCNT_BEGIN	= SUPERH_THREADID - SUPERH_NUMAREGION_REG_SPACING,

	SUPERH_NUMAREGION_WRCNT_END	= SUPERH_NUMAREGION_RDCNT_BEGIN,
	SUPERH_NUMAREGION_WRCNT_BEGIN	= SUPERH_NUMAREGION_RDCNT_BEGIN - SUPERH_NUMAREGION_REG_SPACING,

	SUPERH_NUMAREGION_COUNT		= SUPERH_NUMAREGION_WRCNT_BEGIN - 4,


	SUPERH_DEVSIM_BOTTOM		= SUPERH_NUMAREGION_COUNT,
};

/*											*/
/*				SH3 MMU Registers					*/
/*											*/
#define	MMU_PTEH		((volatile long *) 0xFFFFFFF0)
#define MMU_PTEL		((volatile long *) 0xFFFFFFF4)
#define	MMU_TTB			((volatile long *) 0xFFFFFFF8)
#define	MMU_TEA			((volatile long *) 0xFFFFFFFC)
#define	MMU_MMUCR		((volatile long *) 0xFFFFFFE0)





/*											*/
/*				Exception Registers					*/
/*											*/
#define	EXCP_TRA		((volatile long *) 0xFFFFFFD0)
#define	EXCP_EXPEVT		((volatile long *) 0xFFFFFFD4)
#define	EXCP_INTEVT		((volatile long *) 0xFFFFFFD8)





/*											*/
/*				Interrupt Controller Registers				*/
/*											*/
#define	ICLR_ICR		((volatile short *) 0xFFFFFEE0)
#define	ICLR_IPRA		((volatile short *) 0xFFFFFEE2)
#define	ICLR_IPRB		((volatile short *) 0xFFFFFEE4)





/*											*/
/*				User Break Controller					*/
/*											*/
/*	Channel A	*/
#define	UBC_BARA		((volatile long *) 0xFFFFFFB0)
#define	UBC_BASRA		((volatile unsigned char *) 0xFFFFFFE4)
#define	UBC_BAMRA		((volatile unsigned char *) 0xFFFFFFB4)
#define	UBC_BBRA		((volatile short *)  0xFFFFFFB8)
/*	Channel B	*/
#define	UBC_BARB		((volatile long *) 0xFFFFFFA0)
#define	UBC_BAMRB		((volatile unsigned char *) 0xFFFFFFA4)
#define	UBC_BASRB		((volatile unsigned char *) 0xFFFFFFE8)
#define	UBC_BBRB		((volatile short *)  0xFFFFFFA8)
#define	UBC_BDRB		((volatile long *) 0xFFFFFF90)
#define	UBC_BDMRB		((volatile long *) 0xFFFFFF94)
/*	Common		*/
#define	UBC_BRCR		((volatile short *)  0xFFFFFF98)




/*									*/
/*				Cache Control Register			*/
/*									*/
#define CACHE_CCR		((volatile ulong *) 0xFFFFFFEC)

/*											*/
/*				Lower 4K Cache as On-Chip SRAM				*/
/*											*/
#define	CACHE_SRAM_START	0x7F000000
#define	CACHE_SRAM_END		0x7F000FFF





/*											*/
/*				Power Down Modes					*/
/*											*/
#define	POWER_STBCR		((volatile unsigned char *) 0xFFFFFF82)





/*											*/
/*				On-Chip Oscillation Circuits				*/
/*											*/
#define	OCOSC_FRQCR		((volatile short *)  0xFFFFFF80)





/*											*/
/*				Watchdog Timer						*/
/*											*/
/* 	Read as byte, write as word 	*/
#define	WDOG_WTCNT_RD		((volatile unsigned char *) 0xFFFFFF84)
#define	WDOG_WTCNT_WR		((volatile short *)  0xFFFFFF84)

/*	Read as byte, write as word	*/
#define	WDOG_WTCSR_RD		((volatile unsigned char *) 0xFFFFFF86)
#define	WDOG_WTCSR_WR		((volatile short *)  0xFFFFFF86)






/*											*/
/*				Bus State Controller					*/
/*											*/
#define	BSC_BCR1		((volatile short *) 0xFFFFFF60)
#define	BSC_BCR2		((volatile short *) 0xFFFFFF62)
#define	BSC_WCR1		((volatile short *) 0xFFFFFF64)
#define	BSC_WCR2		((volatile short *) 0xFFFFFF66)
#define	BSC_MCR			((volatile short *) 0xFFFFFF68)
#define	BSC_DCR			((volatile short *) 0xFFFFFF6A)
#define	BSC_PCR			((volatile short *) 0xFFFFFF6C)
#define	BSC_RTCSR		((volatile short *) 0xFFFFFF6E)
#define	BSC_RTCNT		((volatile short *) 0xFFFFFF70)
#define	BSC_RTCCR		((volatile short *) 0xFFFFFF72)
#define	BSC_RFCR		((volatile short *) 0xFFFFFF74)






/*											*/
/*				Timer Unit						*/
/*											*/
#define	TIMER_TOCR		((volatile unsigned char *) 0xFFFFFE90)
#define	TIMER_TSTR		((volatile unsigned char *) 0xFFFFFE92)

#define	TIMER_TCOR0		((volatile long *) 0xFFFFFE94)
#define	TIMER_TCNT0		((volatile long *) 0xFFFFFE98)
#define	TIMER_TCR0		((volatile short *)  0xFFFFFE9C)

#define	TIMER_TCOR1		((volatile long *) 0xFFFFFEA0)
#define	TIMER_TCNT1		((volatile long *) 0xFFFFFEA4)
#define	TIMER_TCR1		((volatile short *)  0xFFFFFEA8)

#define TIMER_TCOR2		((volatile long *) 0xFFFFFEAC)
#define TIMER_TCNT2		((volatile long *) 0xFFFFFEB0)
#define	TIMER_TCR2		((volatile short *)  0xFFFFFEB4)
#define	TIMER_TCPR2		((volatile long *) 0xFFFFFEB8)






/*											*/
/*				Realtime Clock						*/
/*											*/
#define	RTCLK_R64CNT		((volatile unsigned char *) 0xFFFFFEC0)
#define	RTCLK_RSECCNT		((volatile unsigned char *) 0xFFFFFEC2)
#define	RTCLK_RMINCNT		((volatile unsigned char *) 0xFFFFFEC4)
#define	RTCLK_RHRCNT		((volatile unsigned char *) 0xFFFFFEC6)
#define	RTCLK_RWKCNT		((volatile unsigned char *) 0xFFFFFEC8)
#define	RTCLK_RDAYCNT		((volatile unsigned char *) 0xFFFFFECA)
#define	RTCLK_RMONCNT		((volatile unsigned char *) 0xFFFFFECC)
#define	RTCLK_RYRCNT		((volatile unsigned char *) 0xFFFFFECE)
#define	RTCLK_RSECAR		((volatile unsigned char *) 0xFFFFFED0)
#define	RTCLK_RMINAR		((volatile unsigned char *) 0xFFFFFED2)
#define RTCLK_RHRAR		((volatile unsigned char *) 0xFFFFFED4)
#define RTCLK_RWKAR		((volatile unsigned char *) 0xFFFFFED6)
#define	RTCLK_RDAYAR		((volatile unsigned char *) 0xFFFFFED8)
#define	RTCLK_RMONAR		((volatile unsigned char *) 0xFFFFFEDA)
#define	RTCLK_RCR1		((volatile unsigned char *) 0xFFFFFEDC)
#define	RTCLK_RCR2		((volatile unsigned char *) 0xFFFFFEDE)







/*											*/
/*				Serial Communications Interface				*/
/*											*/
#define	SCC_SCSMR		((volatile unsigned char *) 0xFFFFFE80)
#define SCC_SCBRR		((volatile unsigned char *) 0xFFFFFE82)
#define	SCC_SCSCR		((volatile unsigned char *) 0xFFFFFE84)
#define	SCC_SCTDR		((volatile unsigned char *) 0xFFFFFE86)
#define	SCC_SCSSR		((volatile unsigned char *) 0xFFFFFE88)
#define	SCC_SCRDR		((volatile unsigned char *) 0xFFFFFE8A)
#define	SCC_SCSPTR		((volatile unsigned char *) 0xFFFFFF7C)








/*											*/
/*				Smart Card Interface					*/
/*											*/
#define	SMARTCARD_SCSMR		((volatile unsigned char *) 0xFFFFFE80)
#define	SMARTCARD_SCBRR		((volatile unsigned char *) 0xFFFFFE82)
#define	SMARTCARD_SCSCR		((volatile unsigned char *) 0xFFFFFE84)
#define	SMARTCARD_SCTDR		((volatile unsigned char *) 0xFFFFFE86)
#define	SMARTCARD_SCSSR		((volatile unsigned char *) 0xFFFFFE88)
#define	SMARTCARD_SCRDR		((volatile unsigned char *) 0xFFFFFE8A)
#define	SMARTCARD_SCSCMR	((volatile unsigned char *) 0xFFFFFE8C)







/*											*/
/*				General Purpose I/O					*/
/*											*/
#define	GPIO_PCTR		((volatile short *)0xFFFFFF76)
#define	GPIO_PDTR		((volatile unsigned char *) 0xFFFFFF78)
#define GPIO_SCSPTR		((volatile unsigned char *) 0xFFFFFF7C)









/*											*/
/*				Network I/O for simulator				*/
/*											*/
#define NIC_NMR 		((volatile unsigned char *) SUPERH_NIC_NMR)
#define NIC_BRR 		((volatile unsigned char *) SUPERH_NIC_BRR)
#define NIC_NCR 		((volatile unsigned char *) SUPERH_NIC_NCR)
#define NIC_TDR 		((volatile unsigned char *) SUPERH_NIC_TDR)
#define NIC_NSR 		((volatile unsigned char *) SUPERH_NIC_NSR)
#define NIC_RDR 		((volatile unsigned char *) SUPERH_NIC_RDR)
#define NIC_MAXFSZ 		((volatile unsigned char *) SUPERH_NIC_MAXFSZ)
#define NIC_RXFSZ 		((volatile unsigned char *) SUPERH_NIC_RXFSZ)
#define NIC_OUI 		((volatile unsigned char *) SUPERH_NIC_OUI)
#define NIC_DST 		((volatile unsigned char *) SUPERH_NIC_DST)
#define NIC_NCSENSE 		((volatile unsigned char *) SUPERH_NIC_NCSENSE)
#define NIC_NCOLLS 		((volatile unsigned char *) SUPERH_NIC_NCOLLS)
#define NIC_CSENSE 		((volatile unsigned char *) SUPERH_NIC_CSENSE)






/*									*/
/*				Simulation Time				*/
/*									*/
#define SF_SECS 		((volatile unsigned char *) SUPERH_USECS_BEGIN)




/*									*/
/*				DEVLOGPRINT				*/
/*									*/
#define DEVLOGPRINT 		((volatile unsigned char *) SUPERH_DEVLOGPRINT)





/*									*/
/*				Tag Memory				*/
/*									*/
#define TAG_MEMEND 		((volatile unsigned char *) SUPERH_TAG_MEMEND)



/* deprecated. delete.									*/
/*				Log control				*/
/*									*/
#define LOG_CTL 		((volatile unsigned char *) SUPERH_LOG_CTL_BEGIN)



/*									*/
/*				Random number generator			*/
/*									*/
#define RANDDR 			((volatile unsigned char *) SUPERH_RAND_BEGIN)

/*									*/
/*				Location and velocity stuff		*/
/*									*/
#define ORBIT 			((volatile unsigned char *) SUPERH_ORBIT_BEGIN)
#define VELOCITY 		((volatile unsigned char *) SUPERH_VELOCITY_BEGIN)
#define XLOC 			((volatile unsigned char *) SUPERH_XLOC_BEGIN)
#define YLOC 			((volatile unsigned char *) SUPERH_YLOC_BEGIN)
#define ZLOC 			((volatile unsigned char *) SUPERH_ZLOC_BEGIN)


/*									*/
/*				Sensors and Actuators			*/
/*									*/
#define SENSOR_READ 		((volatile float *) SUPERH_SENSREAD_BEGIN)
#define SENSOR_WRITE 		((volatile float *) SUPERH_SENSWRITE_BEGIN)

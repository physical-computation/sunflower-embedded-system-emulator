/*	For 32 log markers, make sure the marker is <= 1<<5			*/
#define	LOGMARK(n)	(*((volatile unsigned char *)0xefeeffa5 + (n & 0x1F)) = 0)

#define	NETTRACEMARK(n)	(*((volatile unsigned char *)0xefeeff85 + (n & 0x1F)) = 0)

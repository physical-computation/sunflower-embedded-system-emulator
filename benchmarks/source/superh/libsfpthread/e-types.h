#define nil	((void*)0)	
#define USED(x)
#define uchar	unsigned char
#define	ushort	unsigned short
#define	uint	unsigned int
#define	ulong	unsigned long
#define	uvlong	unsigned long long



#if defined M64
#	define	Maddr	unsigned long long
#elif defined M32
#	define	Maddr	unsigned long
#elif defined M16
#	define Maddr	unsigned int
#elif defined  M8
#	define Maddr	unsigned char
#else
#	error "You must define one of M64/M32/M16/M8"
#endif


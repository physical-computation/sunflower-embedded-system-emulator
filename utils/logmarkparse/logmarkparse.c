#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uchar   unsigned char
#define ushort  unsigned short
#define uint    unsigned int
#define ulong   unsigned long
#define uvlong  unsigned long long

typedef struct
{
	double	*statvals;
	int	statvalsz;
	int	nstatvals;



	int	*sentcnt;
	int	sentcntsz;
	int	nsentcnt;

	int	*sentbytes;
	int	sentbytessz;
	int	nsentbytes;

	int	*recvdcnt;
	int	recvdcntsz;
	int	nrecvdcnt;

	int	*urxbytes;
	int	urxbytessz;
	int	nurxbytes;
	
	int	*bcastrecvdcnt;
	int	bcastrecvdcntsz;
	int	nbcastrecvdcnt;

	int	*brxbytes;
	int	brxbytessz;
	int	nbrxbytes;
} Stats;



enum
{
	INIT_STATSZ		= 8192,
	LINELEN			= 1024,
	NAMELEN			= 64,
	REALLOC_GROW_FACTOR	= 2,
};


const char	Emalloc[] = "Malloc Failed";
const char	Eopen[] = "Could not open input file";

void		usage(void);
void		fatal(const char *msg);
void		parsesflog(Stats *stats, FILE *fp, char *start,
			char *end, char *statname);
void		parsenetlog(Stats *stats, FILE *fp, char *start,
			char *end, int node);

int
main(int argc, char *argv[])
{
	Stats	stats;
	char	*tagstub, *start, *end, *statname, *netflag, tmp, *ep = &tmp,
		startname[NAMELEN], endname[NAMELEN];
	int	i, stubmin, stubmax;
	FILE	*fp;


	if (argc != 9)
	{
		usage();
	}


	tagstub		= argv[1];
	stubmin		= (int) strtol(argv[5], &ep, 0);
	if (*ep != '\0')
	{
		usage();
	}

	stubmax		= (int) strtol(argv[6], &ep, 0);
	if (*ep != '\0')
	{
		usage();
	}

	start		= argv[2];
	end		= argv[3];
	statname	= argv[4];
	netflag		= argv[7];

	fprintf(stderr, "\n\n");
	fprintf(stderr, "Input File = %s\n", argv[8]);
	fprintf(stderr, "tagstub = %s\n", tagstub);
	fprintf(stderr, "start = %s\n", start);
	fprintf(stderr, "end = %s\n", end);
	fprintf(stderr, "statname = %s\n", statname);
	fprintf(stderr, "stubmin = %d\n", stubmin);
	fprintf(stderr, "stubmax = %d\n", stubmax);
	fprintf(stderr, "netflag = %s\n", netflag);
	fprintf(stderr, "\n\n");

        fp = fopen(argv[8], "r");
        if (fp == NULL)
        {
                fatal(Eopen);
        }

	stats.statvals = (double *)malloc(sizeof(double)*INIT_STATSZ);
	if (stats.statvals == NULL)
	{
		fatal(Emalloc);
	}
	stats.statvalsz = INIT_STATSZ;
	stats.nstatvals = 0;

	stats.sentcnt = (int *)malloc(sizeof(int)*INIT_STATSZ);
	if (stats.sentcnt == NULL)
	{
		fatal(Emalloc);
	}
	stats.sentcntsz = INIT_STATSZ;
	stats.nsentcnt = 0;

	stats.sentbytes = (int *)malloc(sizeof(int)*INIT_STATSZ);
	if (stats.sentbytes == NULL)
	{
		fatal(Emalloc);
	}
	stats.sentbytessz = INIT_STATSZ;
	stats.nsentbytes = 0;

	stats.recvdcnt = (int *)malloc(sizeof(int)*INIT_STATSZ);
	if (stats.recvdcnt == NULL)
	{
		fatal(Emalloc);
	}
	stats.recvdcntsz = INIT_STATSZ;
	stats.nrecvdcnt = 0;

	stats.urxbytes = (int *)malloc(sizeof(int)*INIT_STATSZ);
	if (stats.urxbytes == NULL)
	{
		fatal(Emalloc);
	}
	stats.urxbytessz = INIT_STATSZ;
	stats.nurxbytes = 0;

	stats.bcastrecvdcnt = (int *)malloc(sizeof(int)*INIT_STATSZ);
	if (stats.bcastrecvdcnt == NULL)
	{
		fatal(Emalloc);
	}
	stats.bcastrecvdcntsz = INIT_STATSZ;
	stats.nbcastrecvdcnt = 0;

	stats.brxbytes = (int *)malloc(sizeof(int)*INIT_STATSZ);
	if (stats.brxbytes == NULL)
	{
		fatal(Emalloc);
	}
	stats.brxbytessz = INIT_STATSZ;
	stats.nbrxbytes = 0;


	for (i = stubmin; i <= stubmax; i++)
	{
		snprintf(startname, sizeof(startname), "%s%d%s", tagstub, i, start);
		snprintf(endname, sizeof(endname), "%s%d%s", tagstub, i, end);

		rewind(fp);
		if (!strncmp(netflag, "0", 1))
		{
			parsesflog(&stats, fp, startname, endname, statname);
		}
		if (!strncmp(netflag, "1", 1))
		{
			parsenetlog(&stats, fp, startname, endname, i);
		}
	}

	/*	NOTE: we built the statistics for all the supplied files combined	*/
	if (!strncmp(netflag, "0", 1))
	{
		for (i = 0; i < stats.nstatvals; i++)
		{
			printf("%f\n", stats.statvals[i]);
		}
	}
	if (!strncmp(netflag, "1", 1))
	{
		for (i = 0; i < stats.nsentcnt; i++)
		{
			printf("sentcnt\t%d\n", stats.sentcnt[i]);
		}

		for (i = 0; i < stats.nsentbytes; i++)
		{
			printf("sentbytes\t%d\n", stats.sentbytes[i]);
		}

		for (i = 0; i < stats.nrecvdcnt; i++)
		{
			printf("recvdcnt\t%d\n", stats.recvdcnt[i]);
		}

		for (i = 0; i < stats.nurxbytes; i++)
		{
			printf("urxbytes\t%d\n", stats.urxbytes[i]);
		}
	
		for (i = 0; i < stats.nbcastrecvdcnt; i++)
		{
			printf("bcastrecvdcnt\t%d\n", stats.bcastrecvdcnt[i]);
		}

		for (i = 0; i < stats.nbrxbytes; i++)
		{
			printf("brxbytes\t%d\n", stats.brxbytes[i]);
		}
	}

	fclose(fp);


	return 0;
}

void
parsesflog(Stats *stats, FILE *fp, char *start, char *end, char *statname)
{
	double		startval, delta;
	char		line[LINELEN];


	do
	{
		/*		Scan until start marker				*/
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			if (strstr(line, start) != NULL)
			{
				break;
			}
		}

		/*								*/
		/*	Data points for start checkpoint are contained b/n	*/
		/*	here and the next occurrence of start marker		*/
		/*								*/
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			if (strstr(line, start) != NULL)
			{
				break;
			}

			if (strstr(line, statname) == NULL)
			{
				continue;
			}

			/*							*/
			/*	Example format:					*/
			/*							*/
			/*	--Node1		"ICLK"	=	441259		*/
			/*							*/
			sscanf(line, "%*s %*s %*s %lg", &startval);
		}

		/*		Skip until end region begins			*/
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			if (strstr(line, end) != NULL)
			{
				break;
			}
		}

		/*								*/
		/*	Data points for end checkpoint are contained b/n	*/
		/*	here and the next occurrece of end marker		*/
		/*								*/
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			if (strstr(line, end) != NULL)
			{
				break;
			}

			if (strstr(line, statname) == NULL)
			{
				continue;
			}
			
			/*							*/
			/*	Example format:					*/
			/*							*/
			/*	--Node1		"ICLK"	=	441259		*/
			/*							*/
			sscanf(line, "%*s %*s %*s %lg", &delta);
			delta -= startval;

			stats->statvals[stats->nstatvals++] = delta;
			if (stats->nstatvals == stats->statvalsz)
			{
				stats->statvalsz *= REALLOC_GROW_FACTOR;
				stats->statvals = (double *)realloc(stats->statvals,
							stats->statvalsz*sizeof(double));
				if (stats->statvals == NULL)
				{
					fatal(Emalloc);
				}
			}
		}
	} while (!feof(fp));
}

/*										*/
/*	NOTE that the statistics returned by this routine do not take into	*/
/*	consideration whether or not the packets in question are actually	*/
/*	heard by the destination or received uncorrupted. This is because	*/
/*	what we are parsing here are the frames at the point they are sent	*/
/*	into the ether, not when they are being received.  Logging of such	*/
/*	end-point frames is being added to sflr.  It is OK however to use	*/
/*	this routine for gathering transmit related statistics, since that	*/
/*	is fine.								*/
/*										*/
void
parsenetlog(Stats *stats, FILE *fp, char *start, char *end, int node)
{
	int		pktbytes = 0, sentcnt = 0, recvdcnt = 0,
				bcastrecvdcnt = 0, urxbytes = 0, brxbytes = 0,
				sentbytes = 0, srcnode, dstnode;
	char		line[LINELEN];


	do
	{
		/*		Scan until start marker		*/
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			if (strstr(line, start) != NULL)
			{
				break;
			}
		}

		/*	Skip till end of start block		*/
		while (fgets(line, sizeof(line), fp) != NULL)
		{
			if (strstr(line, start) != NULL)
			{
				break;
			}
		}

		sentcnt		= 0;
		recvdcnt	= 0;
		bcastrecvdcnt	= 0;
		urxbytes	= 0;
		brxbytes	= 0;
		sentbytes	= 0;

		/*	Count packet / bytes until end block	*/
		while (fgets(line, sizeof(line), fp) != NULL)
		{	
			if (strstr(line, end) != NULL)
			{
				break;
			}

			/*								*/
			/*	Timestamp: 4.378500E-03					*/
			/*	Data: 32 00 00 00 00 00 00 00 00 00 00 00 00 00 ,,, .	*/
			/*	Bits left: 0x00000400					*/
			/*	Src node: 0x00000002					*/
			/*	Dst node: 0xFFFFFFFE					*/
			/*	Bcast flag: 0x00000001					*/
			/*	Src ifc: 0x00000000					*/
			/*	Parent netseg ID: 0x00000000				*/
			/*	from_remote flag: 0x00000000				*/
			/*								*/
			if (strstr(line, "Bits left:") != NULL)
			{
				sscanf(line, "%*s %*s %x", &pktbytes);
				pktbytes /= 8;
			}

			if (strstr(line, "Src node:") != NULL)
			{
				sscanf(line, "%*s %*s %x", &srcnode);
//printf("srcnode = %d, node = %d\n", srcnode, node);

				if (srcnode == node)
				{
					sentcnt++;
					sentbytes += pktbytes;
				}
			}

			if (strstr(line, "Dst node:") != NULL)
			{
				sscanf(line, "%*s %*s %x", &dstnode);
//printf("dstnode = %d, node = %d\n", dstnode, node);

				if (dstnode == node)
				{
					recvdcnt++;
					urxbytes += pktbytes;
				}
				else if (dstnode == 0xFFFFFFFE)
				{
					bcastrecvdcnt++;
					brxbytes += pktbytes;
				}
			}
		}

		/*									*/
		/*	We've reached the matching end marker.	Save the counts		*/
		/*									*/
		stats->sentcnt[stats->nsentcnt++] = sentcnt;
		if (stats->nsentcnt == stats->sentcntsz)
		{
			stats->sentcntsz *= REALLOC_GROW_FACTOR;
			stats->sentcnt = (int *)realloc(stats->sentcnt,
						stats->sentcntsz*sizeof(int));
			if (stats->sentcnt == NULL)
			{
				fatal(Emalloc);
			}
		}

		stats->sentbytes[stats->nsentbytes++] = sentbytes;
		if (stats->nsentbytes == stats->sentbytessz)
		{
			stats->sentbytessz *= REALLOC_GROW_FACTOR;
			stats->sentbytes = (int *)realloc(stats->sentbytes,
						stats->sentbytessz*sizeof(int));
			if (stats->sentbytes == NULL)
			{
				fatal(Emalloc);
			}
		}

		stats->recvdcnt[stats->nrecvdcnt++] = recvdcnt;
		if (stats->nrecvdcnt == stats->recvdcntsz)
		{
			stats->recvdcntsz *= REALLOC_GROW_FACTOR;
			stats->recvdcnt = (int *)realloc(stats->recvdcnt,
						stats->recvdcntsz*sizeof(int));
			if (stats->recvdcnt == NULL)
			{
				fatal(Emalloc);
			}
		}

		stats->urxbytes[stats->nurxbytes++] = urxbytes;
		if (stats->nurxbytes == stats->urxbytessz)
		{
			stats->urxbytessz *= REALLOC_GROW_FACTOR;
			stats->urxbytes = (int *)realloc(stats->urxbytes,
						stats->urxbytessz*sizeof(int));
			if (stats->urxbytes == NULL)
			{
				fatal(Emalloc);
			}
		}

		stats->bcastrecvdcnt[stats->nbcastrecvdcnt++] = bcastrecvdcnt;
		if (stats->nbcastrecvdcnt == stats->bcastrecvdcntsz)
		{
			stats->bcastrecvdcntsz *= REALLOC_GROW_FACTOR;
			stats->bcastrecvdcnt = (int *)realloc(stats->bcastrecvdcnt,
						stats->bcastrecvdcntsz*sizeof(int));
			if (stats->bcastrecvdcnt == NULL)
			{
				fatal(Emalloc);
			}
		}

		stats->brxbytes[stats->nbrxbytes++] = brxbytes;
		if (stats->nbrxbytes == stats->brxbytessz)
		{
			stats->brxbytessz *= REALLOC_GROW_FACTOR;
			stats->brxbytes = (int *)realloc(stats->brxbytes,
						stats->brxbytessz*sizeof(int));
			if (stats->brxbytes == NULL)
			{
				fatal(Emalloc);
			}
		}
	} while (!feof(fp));
}

void
usage(void)
{
	fprintf(stderr, "Usage: logmarkparse <tagstub> <stubmin> <stubmax> <log start tag> <log end tag> <statname>\n\n");
	exit(-1);
}

void
fatal(const char *msg)
{
	fprintf(stderr, "Logmarkparse fatal: %s\n", msg);
	exit(-1);
}

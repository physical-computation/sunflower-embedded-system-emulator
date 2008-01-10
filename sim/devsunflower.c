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
#include	"dat.h"
#include	"bio.h"
#include	"error.h"
#include	"fns.h"
#include	"sf.h"
#include	"mextern.h"

/*								*/
/*	The QID is laid out as follows:				*/
/*								*/
/*	CHDIR	....		NODE ID		TYPE		*/
/*	----------------------------------------------		*/
/*	31					QSHIFT		*/
/*								*/
/*	The Type (i.e. Qctl, Qerrs, Qnew etc) is 		*/
/*	encoded in the lower QSHIFT bits of the QID		*/
/*	The SIM_NODE_ID is encoded with its LSB at 		*/
/*	QSHIFT bits after the LSB of QID, and extending 	*/
/*	upward.							*/
/*								*/

#define		QSHIFT		4
#define         QID(q)		((((ulong)(q).path)&0x0000000F)>>0)
#define         CLIENTPATH(q)	((((ulong)q)&0x7FFFFFF0)>>QSHIFT)
#define         CLIENT(q)	CLIENTPATH((q).path)
#define         IOUNIT		(64*1024)

enum
{
	MAX_MNAME = 1024,
};

static Segbuf	devmyrmigkisegbuf;
static char	mname[MAX_MNAME];

enum
{
	Qtopdir,
	Qmroot,
	Qmctl,
	Qminfo,
	Qdir,
        
	Qnetin,
	Qnetout,

	Qctl,
	Qinfo,
	Qstdin,
	Qstdout,
	Qstderr,
	Qoutputrdy
};

static void	packnodeinfo(State *S, char buf[]);
static void	debugnetin(Segbuf *segbuf);

static char	Ebadmyrmigkicmd[]	=   "Invalid myrmigki command";
static char	Eoversizenetinwrite[]	=   "Write to netin too large";
static char	Ebadnetindata[]		=   "Bad data received on netin";


int
devsfspawnscheduler(void)
{	
	return kproc("scheduler", scheduler, nil, KPDUPFDG|KPDUPPG|KPDUPENVG);
}

int
devmyrmigkigen(Chan *c, char *name, Dirtab *tab, int x, int s, Dir *dp)
{
	int t;
	Qid q;
	ulong path;

	USED(name);
	USED(tab);
	USED(x);

	q.vers = 0;

	if (s == DEVDOTDOT)
	{
		switch(QID(c->qid))
		{
		case Qtopdir:
		case Qmroot:
			mkqid(&q, Qtopdir, 0, QTDIR);
			devdir(c, q, "#j", 0, eve, 0500, dp);
			break;
		case Qdir:
			mkqid(&q, Qmroot, 0, QTDIR);
			devdir(c, q, up->genbuf, 0, eve, 0500, dp);
			break;
		default:
			panic("devsunflowerwalk %llux", c->qid.path);
			return -1;
		}
		return 1;
	}


	/*	Top level directory (say, /dev)		*/
        t = QID(c->qid);
	if (t == Qtopdir)
	{
		snprint(mname, MAX_MNAME, "sunflower.%s", c->aux);

		switch(s)
		{
		case 0:
			mkqid(&q, Qmroot, 0, QTDIR);
			devdir(c, q, mname, 0, eve, 0555, dp);
			break;
		default:
			return -1;
		}
		return 1;
	}


	/*	Second level (say /dev/sunflower)	*/
        if (t == Qmroot || t == Qmctl || t == Qminfo || t == Qnetin || t == Qnetout)
	{
		switch (s)
		{
		case 0:
			mkqid(&q, Qmctl, 0, QTFILE);
			devdir(c, q, "ctl", 0, eve, 0200, dp);
			break;

		case 1:
			mkqid(&q, Qminfo, 0, QTFILE);
			devdir(c, q, "info", 0, eve, 0400, dp);
			break;

        	case 2:
			mkqid(&q, Qnetin, 0, QTFILE);
                	devdir(c, q, "netin", 0, eve, 0222, dp);
                	break;

        	case 3:
			mkqid(&q, Qnetout, 0, QTFILE);
                	devdir(c, q, "netout", 0, eve, 0444, dp);
                	break;

		default:
			if (s <= 2*SIM_NUM_NODES+3)
                	{
				int	n = s-4;
				int	id = n >> 1;

				if (n & 1)
				{
					sprint(up->genbuf, "%d.%d",
						(SIM_STATE_PTRS[id])->NODE_ID,
						(SIM_STATE_PTRS[id])->runnable);
				}
				else
				{
					sprint(up->genbuf, "%d", (SIM_STATE_PTRS[id])->NODE_ID);
				}

				mkqid(&q, (id << QSHIFT)|Qdir, 0, QTDIR);
				devdir(c, q, up->genbuf, 0, eve, 0555, dp);
				return 1;
			}
			else
                	{
				return -1;
                	}
		}
		return 1;
	}

	/*	Third level	*/
        path = c->qid.path&~((1<<QSHIFT)-1);
	q.vers = c->qid.vers;
	q.type = QTFILE;
	switch(s)
        {
        case 0:
                q.path = path|Qctl;
                devdir(c, q, "ctl", 0, eve, 0222, dp);
                break;

        case 1:
                q.path = path|Qinfo;
                devdir(c, q, "info", 0, eve, 0444, dp);
                break;

        case 2:
                q.path = path|Qstdin;
                devdir(c, q, "stdin", 0, eve, 0222, dp);
                break;

        case 3:
                q.path = path|Qstdout;
                devdir(c, q, "stdout", 0, eve, 0444, dp);
                break;

        case 4:
                q.path = path|Qstderr;
                devdir(c, q, "stderr", 0, eve, 0444, dp);
                break;

        case 5:
                q.path = path|Qoutputrdy;
                devdir(c, q, "outputrdy", 0, eve, 0444, dp);
                break;

	default:
		return -1;
	}

	return 1;
}

void
devmyrmigkireset(void)
{
	return;
}

void
devmyrmigkiinit(void)
{
	extern  int             gfltconv(Fmt*);

	m_init();
	marchinit();
	fmtinstall('E', gfltconv);

	return;
}

Chan*
devmyrmigkiattach(char *spec)
{
	Chan	*tmp;



	tmp = devattach('j', spec);
	tmp->aux = smalloc(strlen(spec)+1);
	sprint(tmp->aux, "%s", spec);

	if (SIM_NUM_NODES == 0)
	{
		superHnewstate(0, 0, 0, 0, 0);
	}

	return tmp;
}

static Walkqid*
devmyrmigkiwalk(Chan *c, Chan *nc, char **name, int nname)
{
	return devwalk(c, nc, name, nname, 0, 0, devmyrmigkigen);
}

static int
devmyrmigkistat(Chan *c, uchar *db, int n)
{
	return devstat(c, db, n, 0, 0, devmyrmigkigen);
}

static Chan*
devmyrmigkiopen(Chan *c, int omode)
{
	if(c->qid.type & QTDIR)
	{
		return devopen(c, omode, 0, 0, devmyrmigkigen);
	}

	switch(QID(c->qid))
	{
		case Qmctl:
		case Qminfo:
		case Qctl:
		case Qinfo:
		case Qstdin:
		case Qstdout:
		case Qstderr:
		case Qnetin:
		case Qnetout:
			break;
	}

	c->mode = openmode(omode);
	c->flag |= COPEN;
	c->offset = 0;
        c->iounit = IOUNIT;

	return c;
}

static void
devmyrmigkiclose(Chan *c)
{
	switch(QID(c->qid))
	{
		case Qnetin:
		case Qctl:
		case Qinfo:
		case Qstdout:
		case Qstderr:
		case Qoutputrdy:
		case Qstdin:
		case Qmctl:
		case Qminfo:
		case Qnetout:
		{
			break;
		}

		default:
		{
			error(Ebadusefd);
		}
	}

	return;
}

long
devmyrmigkiread(Chan *c, void *a, long n, vlong offset)
{
	int	node_id;


	USED(offset);

	if(c->qid.type & QTDIR)
	{
		return devdirread(c, a, n, 0, 0, devmyrmigkigen);
	}

	node_id = c->qid.path >> QSHIFT;

	switch(QID(c->qid))
	{
		case Qnetout:
		{
			/*							*/
			/*	Each read of netout consumes a whole frame,	*/
			/*	even if you read less than whole buffer.	*/
			/*							*/
			int	nread;

			mstatelock();
			if (SIM_NETIO_H2O == 0)
			{
				mstateunlock();
				return 0;
			}

			nread = min(n, MAX_SEGBUF_TEXT);
			memmove(a, &SIM_NETIO_BUF[SIM_NETIO_H2O - 1][0], nread);
			SIM_NETIO_H2O--;
			mstateunlock();
	
			return nread;
		}

		case Qminfo:
		{
			int	nread; 

			mstatelock();
			nread = min(n, SIM_INFO_H2O);
			memmove(a, SIM_INFO_BUF, nread);
			memmove(&SIM_INFO_BUF[0], &SIM_INFO_BUF[n], SIM_INFO_H2O - nread);
			SIM_INFO_H2O -= nread;
			mstateunlock();

			return nread;
		}

		case Qmctl:
		{
			int	nread;
			char	buf[MAX_NODE_DIGITS*UTFmax+1];

			mstatelock();
			snprint(buf, sizeof(buf), "n = %d, cur = %d",
				SIM_NUM_NODES, CUR_STATE->NODE_ID);
			nread = readstr(offset, a, n, buf);
			mstateunlock();

			return nread;
		}

		case Qctl:
		{
			int	nread;
			State	*S = SIM_STATE_PTRS[node_id];
			char	buf[8192];

			mstatelock();
			packnodeinfo(S, buf);
			nread = readstr(offset, a, n, buf);
			mstateunlock();

			return nread;
		}

		case Qinfo:
		{
			int	nread;

			mstatelock();
			nread = min(n, SIM_STATE_PTRS[node_id]->nodeinfo_h2o);
			memmove(a, SIM_STATE_PTRS[node_id]->nodeinfo_buf, nread);
			memmove(&SIM_STATE_PTRS[node_id]->nodeinfo_buf[0],
				&SIM_STATE_PTRS[node_id]->nodeinfo_buf[nread],
				SIM_STATE_PTRS[node_id]->nodeinfo_h2o - nread);
			SIM_STATE_PTRS[node_id]->nodeinfo_h2o -= nread;
			mstateunlock();

			return nread;
		}

		case Qstdout:
		{
			int	nread;

			mstatelock();
			nread = min(n, SIM_STATE_PTRS[node_id]->stdout_h2o);
			memmove(a, SIM_STATE_PTRS[node_id]->stdout_buf, nread);
			memmove(&SIM_STATE_PTRS[node_id]->stdout_buf[0],
				&SIM_STATE_PTRS[node_id]->stdout_buf[nread],
				SIM_STATE_PTRS[node_id]->stdout_h2o - nread);
			SIM_STATE_PTRS[node_id]->stdout_h2o -= nread;
			mstateunlock();

			return nread;
		}

		case Qstderr:
		{
			int	nread;

			mstatelock();
			nread = min(n, SIM_STATE_PTRS[node_id]->stderr_h2o);
			memmove(a, SIM_STATE_PTRS[node_id]->stderr_buf, nread);
			memmove(&SIM_STATE_PTRS[node_id]->stderr_buf[0],
				&SIM_STATE_PTRS[node_id]->stderr_buf[nread],
				SIM_STATE_PTRS[node_id]->stderr_h2o - nread);
			SIM_STATE_PTRS[node_id]->stderr_h2o -= nread;
			mstateunlock();

			return nread;
		}

		case Qoutputrdy:
		{
			/*	We'll be woken up by arch-Inferno.c 	*/
			//sleep()...
			return 0;
		}

		default:
		{
			error(Ebadusefd);
			return -1;
		}
	}

	return 0;
}

long
devmyrmigkiwrite(Chan *c, void *a, long n, vlong offset)
{
	int 		node_id = c->qid.path >> QSHIFT;

	switch(QID(c->qid))
	{
		case Qmctl:
		{
			((char *)a)[n] = '\0'; 

			if (strlen(a) <= 0)
			{
				error(Ebadmyrmigkicmd);
				return -1;
			}

			mstatelock();
			munchinput(a);
			if (((char *)a)[n-1] != '\n')
			{
				munchinput("\n");
			}
			yyparse();
			mstateunlock();

                        break;
		}

		case Qctl:
		{
			((char *)a)[n] = '\0'; 

			if (strlen(a) <= 0)
			{
				error(Ebadmyrmigkicmd);
				return -1;
			}


			mstatelock();
			CUR_STATE = SIM_STATE_PTRS[node_id];
			munchinput(a);
			if (((char *)a)[n-1] != '\n')
			{
				munchinput("\n");
			}
			//streamchk();
                        //print("before yyparse...\n");
			yyparse();
                        //print("after yyparse...\n");
			mstateunlock();

                        
                        break;
		}

		case Qnetin:
		{
			if (n > MAX_SEGBUF_TEXT)
			{
				error(Eoversizenetinwrite);
				return -1;
			}
			else
			{
				int	len;

				mstatelock();
				len = parsenetsegdump(a, &devmyrmigkisegbuf);
				if (len >= MAX_SEGBUF_TEXT)
				{
					mstateunlock();
					error(Ebadnetindata);
					return -1;
				}
				debugnetin(&devmyrmigkisegbuf);
				remote_seg_enqueue(&devmyrmigkisegbuf);
				mstateunlock();

			}

			break;
		}

		default:
			error(Ebadusefd);
			return -1;
	}

	return n;
}

void
debugnetin(Segbuf *segbuf)
{
	int i;

	print("[debugnetin in devmyrmigki.c] Timestamp: %E\n", segbuf->timestamp);
			
	print("Data: ");
	for (i = 0; i < segbuf->bits_left/8; i++)
	{
		print("%02X ", segbuf->data[i]);
		if (!((i+1) % 24))
		{
			print("\n");
		}
	}
	print(".\n");

	print("[debugnetin in devmyrmigki.c] Bits left: 0x%08X\n",
		segbuf->bits_left);

	if ((State *)segbuf->src_node != NULL)
	{
		print("[debugnetin in devmyrmigki.c] Src node: 0x%08X\n",
			((State *)segbuf->src_node)->NODE_ID);
	}
	else
	{
		mexit("segbuf->src_node is NULL in debugnetin", -1);
	}

	if ((State *)segbuf->dst_node == NULL && !segbuf->bcast)
	{
		mexit("segbuf->dst_node is NULL, and segbuf not bcast, in debugnetin", -1);
	}
	else
	{
		print("[debugnetin in devmyrmigki.c] Dst node: 0x%08X\n",
			((State *)segbuf->dst_node == NULL ? -2 : ((State *)segbuf->dst_node)->NODE_ID));
	}

	print("[debugnetin in devmyrmigki.c] Bcast flag: 0x%08X\n",
		segbuf->bcast);
	print("[debugnetin in devmyrmigki.c] Src ifc: 0x%08X\n",
		segbuf->src_ifc);
	print("[debugnetin in devmyrmigki.c] Parent netseg ID: 0x%08X\n",
		segbuf->parent_netsegid);
	print("[debugnetin in devmyrmigki.c] from_remote flag: 0x%08X\n",
		segbuf->from_remote);
	print("\n\n\n\n");

	return;
}

void
packnodeinfo(State *S, char buf[])
{
	char	*ptr = &buf[0];
	int	sim_rate = 0, sim_rate_mavg = 0;
	ulong	now, delta;


	/*	Time average of sim rate over all time		*/
	if (SIM_ON)
	{
		now = mcputimeusecs();
		delta = now - S->ustart;
		if (delta > 0)
		{
			sim_rate = (S->ICLK - S->startclk) / (delta/1E6);
		}

		/*	Average of sim rate since last "resetctrs"	*/
		delta = now - S->trip_ustart;
		if (delta > 0)
		{
			sim_rate_mavg = (S->ICLK - S->trip_startclk) / (delta/1E6);
		}
	}
	
	sprint(ptr, "%8s = %08d\t%8s = %-8d\n",
		"ID", S->NODE_ID,
		"Active", S->runnable);
	ptr = &buf[strlen(buf)];

	sprint(ptr, "%8s = %08X\t%8s = %-1.2E\n",
		"PC", S->PC, 
		"Tcyc", S->CYCLETIME);
	ptr = &buf[strlen(buf)];

	sprint(ptr, "%8s = %-1.2E\t%8s = %-1.2E\n",
		"ntrans", (double)S->E.ntrans,
		"Ecpu", S->E.CPUEtot);
	ptr = &buf[strlen(buf)];

	sprint(ptr, "%8s = %-1.2E\t%8s = %-1.2E\n",
		"Tcpu", S->TIME,
		"ninstrs", (double)S->dyncnt);
	ptr = &buf[strlen(buf)];

	sprint(ptr, "%8s = %-8s\t%8s = %-1.2E\n",
		"cputype", (S->machinetype == MACHINE_SUPERH ? "SuperH" : "Unknown"),
		"Vdd", S->VDD);
	ptr = &buf[strlen(buf)];

	if (S->machinetype == MACHINE_SUPERH)
	{
		sprint(ptr, "%8s = %-8d\t%8s = %-8d\n",
			"nicnifcs", S->superH->NIC_NUM_IFCS,
			"nicqintr", S->superH->nicintrQ->nqintrs);
	}
	else
	{
		sprint(ptr, "%8s = %-8s\t%8s = %-8s\n",
			"nicnifcs", "n/a",
			"nicqintr", "n/a");
	}
	ptr = &buf[strlen(buf)];

	sprint(ptr, "%8s = %-1.2E\t%8s = %-8llud\n",
		"P(fail)", S->fail_prob,
		"Max fdur", S->failure_duration_max);
	ptr = &buf[strlen(buf)];

	sprint(ptr, "%8s = %-8d\t%8s = %-8d\n",
		"tripRate", sim_rate_mavg,
		"Sim Rate", sim_rate);
	ptr = &buf[strlen(buf)];

	sprint(ptr, "%8s = %-8d\t%8s = %1.6f\n",
		"Throttle", SIM_THROTTLE_NANOSEC,
		"R_active", (S->ICLK > 0 ? (double)S->CLK/(double)S->ICLK : 0.0));
	ptr = &buf[strlen(buf)];

	sprint(ptr, "%8s = %02.1f,%02.1f,%02.1f %8s = %-1.2E\n",
		"loc", S->xloc, S->yloc, S->zloc,
		"sensor0", S->sensors[0].reading);

	return;
}
		
Dev sunflowerdevtab =
{
	'j',
	"sunflower",

	devmyrmigkiinit,
	devmyrmigkiattach,
	devmyrmigkiwalk,
	devmyrmigkistat,
	devmyrmigkiopen,
	devcreate,
	devmyrmigkiclose,
	devmyrmigkiread,
	devbread,
	devmyrmigkiwrite,
	devbwrite,
	devremove,
	devwstat
};

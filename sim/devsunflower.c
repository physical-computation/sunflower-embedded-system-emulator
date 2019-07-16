/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)
 
	All rights reserved.

	Redistribution and use in source and binary forms, with or without 
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written 
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
	POSSIBILITY OF SUCH DAMAGE.
*/

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
/*	The node id is encoded with its LSB at SQHIFT		*/
/*	bits after the LSB of QID, and extending upward.	*/
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

static Segbuf	devsfsegbuf;
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

static void	packnodeinfo(Engine *, State *S, char buf[]);
static void	debugnetin(Segbuf *segbuf);

static char	Ebadsfcmd[]		=   "Invalid Sunflowersim command";
static char	Eoversizenetinwrite[]	=   "Write to netin too large";
static char	Ebadnetindata[]		=   "Bad data received on netin";


int
devsfspawnscheduler(Engine *E)
{	
	return kproc("scheduler", scheduler, E, KPDUPFDG|KPDUPPG|KPDUPENVG);
}

int
devsfkillscheduler(void)
{	
	return;
}

int
devsfgen(Chan *c, char *name, Dirtab *tab, int x, int s, Dir *dp)
{
	int		t;
	Qid		q;
	ulong		path;
	Engine		*E = (Engine *)c->aux;


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
		snprint(mname, MAX_MNAME, "sunflower.%s", E->attachspec);

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
			if (s <= 2*E->nnodes+3)
                	{
				int	n = s-4;
				int	id = n >> 1;

				if (n & 1)
				{
					sprint(up->genbuf, "%d.%d",
						(E->sp[id])->NODE_ID,
						(E->sp[id])->runnable);
				}
				else
				{
					sprint(up->genbuf, "%d", (E->sp[id])->NODE_ID);
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
devsfreset(void)
{
	return;
}

void
devsfinit(void)
{
	extern int	gfltconv(Fmt*);

	marchinit();
	fmtinstall('E', gfltconv);

	return;
}

Chan*
devsfattach(char *spec)
{
	Engine	*E;
	Chan	*tmp;
	int	match, engineid;


print("devsf: new attach with spec [%s]\n", spec);


	tmp = devattach('j', spec);

	match = sscanf(spec, "%*.%uX", &engineid);
	if (match)
	{
print("devsf: requested engine ID is [%d]\n", engineid);

		tmp->aux = m_lookupengine(engineid);
	}

	if (tmp->aux == nil)
	{
		int	seed = (match ? engineid : -1);
print("devsf: no match on requested engine ID. Allocating a new engine, seeding with [%d]...\n", seed);

		tmp->aux = m_allocengine(seed);
		if (tmp->aux == nil)
		{
			panic("allocengine failed");
		}
	}
print("devsf: Done allocating a new engine...\n");
	
	E = (Engine *)tmp->aux;
	E->attachspec = smalloc(strlen(spec)+1);
	if (E->attachspec == nil)
	{
		panic("allocating memory for attachspec failed");
	}

	sprint(E->attachspec, "%s", spec);


	/*	By default, create one Hitachi SH node	*/
	if (E->nnodes == 0)
	{
		superHnewstate(E, 0, 0, 0, nil);
	}

	return tmp;
}

static Walkqid*
devsfwalk(Chan *c, Chan *nc, char **name, int nname)
{
	return devwalk(c, nc, name, nname, 0, 0, devsfgen);
}

static int
devsfstat(Chan *c, uchar *db, int n)
{
	return devstat(c, db, n, 0, 0, devsfgen);
}

static Chan*
devsfopen(Chan *c, int omode)
{
	if(c->qid.type & QTDIR)
	{
		return devopen(c, omode, 0, 0, devsfgen);
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
devsfclose(Chan *c)
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
devsfread(Chan *c, void *a, long n, vlong offset)
{
	int		node_id;
	Engine		*E = (Engine *)c->aux;


	USED(offset);

	if(c->qid.type & QTDIR)
	{
		return devdirread(c, a, n, 0, 0, devsfgen);
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
			if (E->netioh2o == 0)
			{
				mstateunlock();
				return 0;
			}

			nread = min(n, MAX_SEGBUF_TEXT);
			memmove(a, &E->netiobuf[E->netioh2o - 1][0], nread);
			E->netioh2o--;
			mstateunlock();
	
			return nread;
		}

		case Qminfo:
		{
			int	nread; 

			mstatelock();
			nread = min(n, E->infoh2o);
			memmove(a, E->infobuf, nread);
			memmove(&E->infobuf[0], &E->infobuf[n], E->infoh2o - nread);
			E->infoh2o -= nread;
			mstateunlock();

			return nread;
		}

		case Qmctl:
		{
			int	nread;
			char	buf[MAX_NODE_DIGITS*UTFmax+1];

			mstatelock();
			snprint(buf, sizeof(buf), "n = %d, cur = %d",
				E->nnodes, E->cp->NODE_ID);
			nread = readstr(offset, a, n, buf);
			mstateunlock();

			return nread;
		}

		case Qctl:
		{
			int	nread;
			State	*S = E->sp[node_id];
			char	buf[8192];

			mstatelock();
			packnodeinfo(E, S, buf);
			nread = readstr(offset, a, n, buf);
			mstateunlock();

			return nread;
		}

		case Qinfo:
		{
			int	nread;

			mstatelock();
			nread = min(n, E->sp[node_id]->nodeinfo_h2o);
			memmove(a, E->sp[node_id]->nodeinfo_buf, nread);
			memmove(&E->sp[node_id]->nodeinfo_buf[0],
				&E->sp[node_id]->nodeinfo_buf[nread],
				E->sp[node_id]->nodeinfo_h2o - nread);
			E->sp[node_id]->nodeinfo_h2o -= nread;
			mstateunlock();

			return nread;
		}

		case Qstdout:
		{
			int	nread;

			mstatelock();
			nread = min(n, E->sp[node_id]->stdout_h2o);
			memmove(a, E->sp[node_id]->stdout_buf, nread);
			memmove(&E->sp[node_id]->stdout_buf[0],
				&E->sp[node_id]->stdout_buf[nread],
				E->sp[node_id]->stdout_h2o - nread);
			E->sp[node_id]->stdout_h2o -= nread;
			mstateunlock();

			return nread;
		}

		case Qstderr:
		{
			int	nread;

			mstatelock();
			nread = min(n, E->sp[node_id]->stderr_h2o);
			memmove(a, E->sp[node_id]->stderr_buf, nread);
			memmove(&E->sp[node_id]->stderr_buf[0],
				&E->sp[node_id]->stderr_buf[nread],
				E->sp[node_id]->stderr_h2o - nread);
			E->sp[node_id]->stderr_h2o -= nread;
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
devsfwrite(Chan *c, void *a, long n, vlong offset)
{
	int 		node_id = c->qid.path >> QSHIFT;
	Engine		*E = (Engine *)c->aux;


	USED(offset);
	switch(QID(c->qid))
	{
		case Qmctl:
		{
			((char *)a)[n] = '\0'; 

			if (strlen(a) <= 0)
			{
				error(Ebadsfcmd);
				return -1;
			}

			mstatelock();
			munchinput(E, a);
			if (((char *)a)[n-1] != '\n')
			{
				munchinput(E, "\n");
			}
			yyengine = E;
			if (yyengine->cp->machinetype == MACHINE_SUPERH)
			{
				sf_superh_parse();
			}
			else if (yyengine->cp->machinetype == MACHINE_RISCV)
			{
				sf_riscv_parse();
			}
			mstateunlock();

                        break;
		}

		case Qctl:
		{
			((char *)a)[n] = '\0'; 

			if (strlen(a) <= 0)
			{
				error(Ebadsfcmd);
				return -1;
			}


			mstatelock();
			E->cp = E->sp[node_id];
			munchinput(E, a);
			if (((char *)a)[n-1] != '\n')
			{
				munchinput(E, "\n");
			}
			//streamchk();
                        //print("before yyparse...\n");
			yyengine = E;
			if (yyengine->cp->machinetype == MACHINE_SUPERH)
			{
				sf_superh_parse();
			}
			else if (yyengine->cp->machinetype == MACHINE_RISCV)
			{
				sf_riscv_parse();
			}
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
				len = parsenetsegdump(E, a, &devsfsegbuf);
				if (len >= MAX_SEGBUF_TEXT)
				{
					mstateunlock();
					error(Ebadnetindata);
					return -1;
				}
				debugnetin(&devsfsegbuf);
				remote_seg_enqueue(E, &devsfsegbuf);
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
devsfshutdown(void)
{
	//TODO: implement m_shutdown() in main simulator dist
	//TODO: call m_shutdown to cleanly shutdown engine
}

static void
debugnetin(Segbuf *segbuf)
{
	int i;

	print("[debugnetin in devsf.c] Timestamp: %E\n", segbuf->timestamp);
			
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

	print("[debugnetin in devsf.c] Bits left: 0x%08X\n",
		segbuf->bits_left);

	if ((State *)segbuf->src_node != nil)
	{
		print("[debugnetin in devsf.c] Src node: 0x%08X\n",
			((State *)segbuf->src_node)->NODE_ID);
	}
	else
	{
		panic("segbuf->src_node is nil in debugnetin");
	}

	if ((State *)segbuf->dst_node == nil && !segbuf->bcast)
	{
		panic("segbuf->dst_node is nil, and segbuf not bcast, in debugnetin");
	}
	else
	{
		print("[debugnetin in devsf.c] Dst node: 0x%08X\n",
			((State *)segbuf->dst_node == nil ? -2 : ((State *)segbuf->dst_node)->NODE_ID));
	}

	print("[debugnetin in devsf.c] Bcast flag: 0x%08X\n",
		segbuf->bcast);
	print("[debugnetin in devsf.c] Src ifc: 0x%08X\n",
		segbuf->src_ifc);
	print("[debugnetin in devsf.c] Parent netseg ID: 0x%08X\n",
		segbuf->parent_netsegid);
	print("[debugnetin in devsf.c] from_remote flag: 0x%08X\n",
		segbuf->from_remote);
	print("\n\n\n\n");

	return;
}

static void
packnodeinfo(Engine *E, State *S, char buf[])
{
	char		*ptr = &buf[0];
	int		sim_rate = 0, sim_rate_mavg = 0;
	ulong		now, delta;


	/*	Time average of sim rate over all time		*/
	if (E->on)
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
		"ntrans", (double)S->energyinfo.ntrans,
		"Ecpu", S->energyinfo.CPUEtot);
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
		"Throttle", E->throttlensec,
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

#ifndef EMU
	devreset
#endif
	devsfinit,
#ifndef EMU
	devshutdown
#endif
	devsfattach,
	devsfwalk,
	devsfstat,
	devsfopen,
	devcreate,
	devsfclose,
	devsfread,
	devbread,
	devsfwrite,
	devbwrite,
	devremove,
	devwstat
};

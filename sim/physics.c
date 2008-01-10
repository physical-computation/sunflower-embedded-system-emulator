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
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "sf.h"
#include "mextern.h"

void
physics_feed(void)
{
	int		i, j;
	double		reading, lskew, hskew, idx_real, idx_low, idx_high;
	State		*dstnode;
	Signalsrc	*s;


	for (i = 0; i < SIM_NUM_SIGSRCS; i++)
	{
		s = &SIM_SIGSRCS[i];

		if (s->nsamples <= 0)
		{
			continue;
		}

		idx_real = SIM_GLOBAL_TIME * s->sample_rate;
		if (s->nsamples > 1)
		{
			if (idx_real >= (double)s->nsamples)
			{
				if (s->loopsamples)
				{
					idx_real = fmod(idx_real, (double)s->nsamples);
				}
				else
				{
					idx_real = (double)(s->nsamples - 1);
				}
			}

			idx_low = floor(idx_real);
			idx_high = ceil(idx_real);

			lskew = (idx_high - idx_real) / (idx_high - idx_low);
			hskew = (idx_real - idx_low) / (idx_high - idx_low);

			s->sample = lskew*s->samples[(int)idx_low] + hskew*s->samples[(int)idx_high];
		}
		else
		{
			s->sample = s->samples[(int)ceil(idx_real)];
		}
		
		idx_real = SIM_GLOBAL_TIME * s->trajectory_rate;
		if (s->nlocations > 1)
		{
			if (idx_real >= (double) s->nlocations)
			{
				if (s->looptrajectory)
				{
					idx_real = fmod(idx_real, (double) s->nlocations);
				}
				else
				{
					idx_real = (double) (s->nlocations - 1);
				}
			}
			idx_low = floor(idx_real);
			idx_high = ceil(idx_real);

			lskew = (idx_high - idx_real) / (idx_high - idx_low);
			hskew = (idx_real - idx_low) / (idx_high - idx_low);

			s->xloc = lskew*s->xlocs[(int)idx_low] + hskew*s->xlocs[(int)idx_high];
			s->yloc = lskew*s->ylocs[(int)idx_low] + hskew*s->ylocs[(int)idx_high];
			s->zloc = lskew*s->zlocs[(int)idx_low] + hskew*s->zlocs[(int)idx_high];
		}
		/*	A sigsrc might have invalid s->xlocs[] etc array	*/
		else if (s->nlocations == 1)
		{
			s->xloc = s->xlocs[(int)ceil(idx_real)];
			s->yloc = s->ylocs[(int)ceil(idx_real)];
			s->zloc = s->zlocs[(int)ceil(idx_real)];
		}

		for (j = 0; j < s->numsubscribed; j++)
		{
			dstnode = (State *)s->subscribed_sensors[j]->parent;
			
			reading = physics_propagation(s, dstnode->xloc, dstnode->yloc, dstnode->zloc);

			if (i == 0)
			{
				s->subscribed_sensors[j]->reading = reading;
			}
			else
			{
				s->subscribed_sensors[j]->reading += reading;
			}
		}
	}
}

double
physics_propagation(Signalsrc *s, double dst_xloc, double dst_yloc, double dst_zloc)
{
	double	x, q, tmpexp, signal;

	
	x = pow(dst_xloc - s->xloc, 2) +
		pow(dst_yloc - s->yloc, 2) +
		pow(dst_zloc - s->zloc, 2);
	x = pow(x, 1.0/2.0);

	q = s->sample;

	if (x == 0)
	{
		return q;
	}

	tmpexp = 	s->const_coeff_F * pow(x, s->const_pow_q) +
			s->const_coeff_G * pow(x, s->const_pow_r) +
			s->const_coeff_H * pow(x, s->const_pow_s) +
			s->const_coeff_I * pow(x, s->const_pow_t);

	signal =	q*(	s->const_coeff_A * pow(x, s->const_pow_m) +
			s->const_coeff_B * pow(x, s->const_pow_n) + 
			s->const_coeff_C * pow(x, s->const_pow_o) + 
			s->const_coeff_D * pow(x, s->const_pow_p) + 
			s->const_coeff_E * pow(s->const_base_K, tmpexp));

	return signal;	
}



void
physics_newsigsrc(int type, char* descr, double tau, double propspeed,
	double A, double B, double C, double D, double E, double F,
	double G, double H, double I, double K, double powm, double pown,
	double powo, double powp, double powq, double powr, double pows, double powt,
	char * trajectoryfile, int trajectoryrate, double fixedx,
	double fixedy, double fixedz, int looptrajectory, char *samplesfile,
	int samplerate, double fixedsampleval, int loopsamples)
{
	enum		{MAX_LINELEN = 1024};
	char		c, buf[MAX_LINELEN], *ep = &c;
	Signalsrc	*s;
	double		val;
	int		tmpfd, linesread = 0;


	if (SIM_NUM_SIGSRCS >= MAX_SIGNAL_SRCS)
	{
		merror("Signal source creation limit reached.");
		return;
	}

	s = &SIM_SIGSRCS[SIM_NUM_SIGSRCS++];
	s->ID = SIM_NUM_SIGSRCS;
	s->numsubscribed = 0;
	s->type = type;

	if (strlen(descr) > 0)
	{
		s->description = mcalloc(strlen(descr)+1, sizeof(char),
			"s->description in shasm.y");			
		if (s->description == NULL)
		{
			merror("Could not allocate memory for s->description.");
			return;
		}
		strcpy(s->description, descr);
	}

	s->valid = 1;
	s->updates_tau = tau;
	s->propagation_speed = propspeed;
	s->const_coeff_A = A;
	s->const_coeff_B = B;
	s->const_coeff_C = C;
	s->const_coeff_D = D;
	s->const_coeff_E = E;
	s->const_coeff_F = F;
	s->const_coeff_G = G;
	s->const_coeff_H = H;
	s->const_coeff_I = I;

	s->const_base_K = K;

	s->const_pow_m = powm;
	s->const_pow_n = pown;
	s->const_pow_o = powo;
	s->const_pow_p = powp;
	s->const_pow_q = powq;
	s->const_pow_r = powr;
	s->const_pow_s = pows;
	s->const_pow_t = powt;

	if (strlen(trajectoryfile) > 0)
	{
		s->trajectory_file = mcalloc(strlen(trajectoryfile)+1, sizeof(char),
				"s->trajectory_file in shasm.y");
		if (s->trajectory_file == NULL)
		{
			merror("Could not allocate memory for s->trajectory_file.");
			return;
		}
		strcpy(s->trajectory_file, trajectoryfile);
	}

	tmpfd = mopen(s->trajectory_file, M_OREAD);
	linesread = 0;
	while ((tmpfd  >= 0) && (mfgets(buf, MAX_LINELEN, tmpfd) != NULL))
	{
		if ((strlen(buf) > 0) && buf[strlen(buf)-1] == '\n')
		{
			buf[strlen(buf)-1] = '\0';
		}

		val = strtod(buf, &ep);
		if ((linesread == 0) && (*ep != '\0'))
		{
			merror("Invalid \"# of records\" field (\"%s\") in trajectory file.", buf);
			continue;
		}

		if (linesread == 0)
		{
			s->xlocs = mcalloc((int)val, sizeof(double),
				"s->xlocs in shasm.y");
			if (s->xlocs == NULL)
			{
				merror("Could not allocate memory for s->xlocs.");
				return;
			}

			s->ylocs = mcalloc((int)val, sizeof(double),
					"s->ylocs in shasm.y");
			if (s->ylocs == NULL)
			{
				merror("Could not allocate memory for s->ylocs.");
				return;
			}

			s->zlocs = mcalloc((int)val, sizeof(double),
					"s->zlocs in shasm.y");
			if (s->zlocs == NULL)
			{
				merror("Could not allocate memory for s->zlocs.");
				return;
			}

			mprint(NULL, siminfo,
				"[%d] records in trajectory file\n", (int)val);
		}
		else
		{
			int	i = 0;
			char 	*p;
						

			for ((p = strtok(buf, " \n\t")); p; (p = strtok(NULL, " \n\t")), i++)
			{
				switch (i)
				{
					case 0:
					{
						s->xlocs[s->nlocations] = strtod(p, &ep);
						if (*ep != '\0')
						{
							merror("Invalid xloc in trajectory file.");
						}

						break;
					}

					case 1:
					{
						s->ylocs[s->nlocations] = strtod(p, &ep);
						if (*ep != '\0')
						{
							merror("Invalid yloc in trajectory file.");
						}

						break;
					}

					case 2:
					{
						s->zlocs[s->nlocations] = strtod(p, &ep);
						if (*ep != '\0')
						{
							merror("Invalid zloc in trajectory file.");
						}

						break;
					}

					default:
					{
						merror("Extra field in trajectory file.");
					}

				}
           		}
			s->nlocations++;
		}
		linesread++;
	}
	
	if (tmpfd >= 0)
	{
		mclose(tmpfd);
	}
			
	s->trajectory_rate = trajectoryrate;
	s->xloc = fixedx;
	s->yloc = fixedy;
	s->zloc = fixedz;
	s->looptrajectory = looptrajectory;


	if (strlen(samplesfile) > 0)
	{
		s->samples_file = mcalloc(strlen(samplesfile)+1, sizeof(char),
				"s->description in shasm.y");
		if (s->samples_file == NULL)
		{
			merror("Could not allocate memory for s->samples_file.");
			return;
		}			
		strcpy(s->samples_file, samplesfile);
	}

	s->sample_rate = samplerate;
	linesread = 0;
	if ((tmpfd = mopen(s->samples_file, M_OREAD)) < 0)
	{
		s->nsamples = 1;
		s->samples = mcalloc(1, sizeof(double), "s->samples in shasm.y");
		if (s->samples == NULL)
		{
			merror("Could not allocate memory for s->samples.");
			return;
		}
		s->samples[0] = fixedsampleval;
	}
	else while (mfgets(buf, MAX_LINELEN, tmpfd) != NULL)
	{
		if ((strlen(buf) > 0) && buf[strlen(buf)-1] == '\n')
		{
			buf[strlen(buf)-1] = '\0';
		}

		val = strtod(buf, &ep);
		if (*ep != '\0')
		{
			merror("Invalid data in signal samples file.");
			continue;
		}

		if (linesread == 0)
		{
			s->samples = mcalloc((int)val, sizeof(double),
				"s->samples in shasm.y");
			if (s->samples == NULL)
			{
				merror("Could not allocate memory for s->samples.");
				return;
			}
		}
		else
		{
			s->samples[s->nsamples] = val;
			s->nsamples++;
		}
		linesread++;
	}				
	
	if (tmpfd >= 0)
	{
		mclose(tmpfd);
	}

	s->loopsamples = loopsamples;
			

	return;
}


void
physics_sensorsdbg(void)
{
	int 		i, j;
	double		r, sigpercent;
	State		*dstnode;
	Signalsrc	*s;


	for (i = 0; i < SIM_NUM_SIGSRCS; i++)
	{
		s = &SIM_SIGSRCS[i];

		mprint(NULL, siminfo, "\n");
		mprint(NULL, siminfo, "Signal Source \"%s\"\n",
			s->description);
		mprint(NULL, siminfo, "\ttype = [%d]\n",
			s->type);
		mprint(NULL, siminfo, "\tnumsubscribed = [%d]\n",
			s->numsubscribed);
		mprint(NULL, siminfo, "\tnsamples = [%d]\n",
			s->nsamples);

		mprint(NULL, siminfo, "\tupdates_tau = [%E]\n",
			s->updates_tau);
		mprint(NULL, siminfo, "\tpropagation_speed = [%E]\n",
			s->propagation_speed);
		mprint(NULL, siminfo, "\tconst_coeff_A = [%E]\n",
			s->const_coeff_A);
		mprint(NULL, siminfo, "\tconst_coeff_B = [%E]\n",
			s->const_coeff_B);
		mprint(NULL, siminfo, "\tconst_coeff_C = [%E]\n",
			s->const_coeff_C);
		mprint(NULL, siminfo, "\tconst_coeff_D = [%E]\n",
			s->const_coeff_D);
		mprint(NULL, siminfo, "\tconst_coeff_E = [%E]\n",
			s->const_coeff_E);
		mprint(NULL, siminfo, "\tconst_coeff_F = [%E]\n",
			s->const_coeff_F);
		mprint(NULL, siminfo, "\tconst_coeff_G = [%E]\n",
			s->const_coeff_G);
		mprint(NULL, siminfo, "\tconst_coeff_H = [%E]\n",
			s->const_coeff_H);
		mprint(NULL, siminfo, "\tconst_coeff_I = [%E]\n",
			s->const_coeff_I);
		mprint(NULL, siminfo, "\tconst_base_K = [%E]\n",
			s->const_base_K);

		mprint(NULL, siminfo, "\tconst_pow_m = [%E]\n",
			s->const_pow_m);
		mprint(NULL, siminfo, "\tconst_pow_n = [%E]\n",
			s->const_pow_n);
		mprint(NULL, siminfo, "\tconst_pow_o = [%E]\n",
			s->const_pow_o);
		mprint(NULL, siminfo, "\tconst_pow_p = [%E]\n",
			s->const_pow_p);
		mprint(NULL, siminfo, "\tconst_pow_q = [%E]\n",
			s->const_pow_q);
		mprint(NULL, siminfo, "\tconst_pow_r = [%E]\n",
			s->const_pow_r);
		mprint(NULL, siminfo, "\tconst_pow_s = [%E]\n",
			s->const_pow_s);
		mprint(NULL, siminfo, "\tconst_pow_t = [%E]\n",
			s->const_pow_t);

		mprint(NULL, siminfo, "\ttrajectory input file = [%s]\n",
			s->trajectory_file);
		mprint(NULL, siminfo, "\tnlocations = [%d]\n",
			s->nlocations);
		mprint(NULL, siminfo, "\ttrajectory_rate = [%d]\n",
			s->trajectory_rate);
		mprint(NULL, siminfo, "\txloc = [%E]\n",
			s->xloc);
		mprint(NULL, siminfo, "\tyloc = [%E]\n",
			s->yloc);
		mprint(NULL, siminfo, "\tzloc = [%E]\n",
			s->zloc);
		mprint(NULL, siminfo, "\tlooptrajectory = [%d]\n",
			s->looptrajectory);

		mprint(NULL, siminfo, "\tsamples_file = [%s]\n",
			s->samples_file);
		mprint(NULL, siminfo, "\tsamples[0] = [%E]\n",
			s->samples[0]);
		mprint(NULL, siminfo, "\tsample_rate = [%d]\n",
			s->sample_rate);
		mprint(NULL, siminfo, "\tloopsamples = [%d]\n",
			s->loopsamples);

		for (j = 0; j < SIM_SIGSRCS[i].numsubscribed; j++)
		{
			dstnode = (State *)s->subscribed_sensors[j]->parent;

			mprint(NULL, siminfo, "\t\tnode %d, sensor %d, ",
				dstnode->NODE_ID,
				s->subscribed_sensors[j]->which);

			r = pow(dstnode->xloc - s->xloc, 2) +
				pow(dstnode->yloc - s->yloc, 2) +
				pow(dstnode->zloc - s->zloc, 2);
			r = pow(r, 1.0/2.0);

			sigpercent = physics_propagation(s,
				dstnode->xloc, dstnode->yloc, dstnode->zloc) / s->sample;
			sigpercent *= 100;
	
			mprint(NULL, siminfo,
				"distance %.2f units, signal strength %.2f%%.\n",
				r, sigpercent);
		}
		mprint(NULL, siminfo, "\n\n");
	}
			
	return;
}


void
physics_sigsubscr(State *S, int whichsensor, int whichsignal)
{
	if ((whichsensor >= MAX_NODE_SENSORS) || (whichsignal >= MAX_SIGNAL_SRCS))
	{
		merror("Sensor or Signal index out of range.");
		return;
	}

	S->sensors[whichsensor].which = whichsensor;
	S->sensors[whichsensor].parent = S;
	SIM_SIGSRCS[whichsignal].subscribed_sensors[SIM_SIGSRCS[whichsignal].numsubscribed++] =
		&S->sensors[whichsensor];
			
	return;
}

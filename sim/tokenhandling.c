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

#include <string.h>
#include "sf.h"
#include "mextern.h"

void
munchinput(Engine *E, char *buf)
{	
	int 	eaten = 0;


	if (strlen(buf) > 0)
	{
              	while (eaten < strlen(buf))
               	{
			char *tptr;
			Datum *t1;

			t1 = (Datum *) mmalloc(E, sizeof(Datum),
					"Datum *t1 in munchinput(), main.c");

			if (t1 == NULL)
			{
				mprint(E, NULL, siminfo, "Could not allocate memory for Datum *t1");
				mexit(E, "See above messages.", -1);
			}

			t1->next = NULL;
			t1->prev = NULL;
			t1->value = 0;

			t1->data = (char*) mmalloc(E, (strlen(buf)+1)*sizeof(char), 
						"char *t1->data in munchinput(), main.c");
			if (t1->data == NULL)
			{
				mfree(E, t1, "char *t1 in main.c");
				mprint(E, NULL, siminfo, "Could not allocate memory for char *t1->data");
				mexit(E, "See above messages.", -1);
			}
			tptr = t1->data;

			/* 	Throw away all chars till we see a non-sepchar 	*/
			while (isasmsep(buf[eaten]) && (eaten < strlen(buf)))
			{
				eaten++;
			}

			/*							*/
			/*   I refer to tokens such as '(' etc. as "sticky":	*/
			/*   they may be "stuck" onto another token, and are	*/
			/*   NOT separators : we have to allocate a list entry 	*/
			/*   for them. 	So, get one sticky char:		*/
			/*							*/
			if (issticky(buf[eaten]))
			{
				*tptr++ = buf[eaten++];
			}
			else while(!isasmsep(buf[eaten]) &&
					!issticky(buf[eaten]) &&
					(eaten < strlen(buf))
				)
			{
				/*						*/
				/*	Get all non sepchars into t1->data	*/
				/*	If we see a quoted string, "*" (_not_	*/
				/*	'*'), gobble it including any sepchars,	*/
				/*	till end of input (we get line @a time)	*/
				/*	or matching quote.			*/
				/*						*/
				if (buf[eaten] == '"')
				{
					/*		Discard opening quote:		*/
					eaten++;

					/*	Must make not to go past newline:	*/
					while ((buf[eaten] != '"') &&
						(buf[eaten] != '\n') &&
						(eaten < strlen(buf)))
					{
						*tptr++ = buf[eaten++];
					}
					
					/*	Discard trailing quote if its there:	*/
					if (eaten < strlen(buf) && (buf[eaten] == '"'))
					{
						eaten++;
					}
				}
				else
				{
					*tptr++ = buf[eaten++];
				}
			}

			*tptr = '\0';

			/*	If we actually ate any food, put it in our stomach	*/
			if (tptr != t1->data)
			{
				if ((E->istream.head == NULL)  || (E->istream.tail == NULL))
				{
					E->istream.tail = E->istream.head = E->istream.masthead = t1;

					/*							*/
					/*    NOTE tail and head now point to the lone datum 	*/
					/*    and they _both_ have null pre- and next-.		*/
					/*							*/
					E->istream.masthead->prev = NULL;
					E->istream.masthead->next = NULL;

					E->istream.head->prev = NULL;
					E->istream.head->next = NULL;

					E->istream.tail->prev = NULL;
					E->istream.tail->next = NULL;
				}
				else
				{
					/*							*/
					/*  Add new datum to _tail_ of list. MUST keep it FIFO 	*/
					/*  for the asm to be parsed correctly.			*/
					/*							*/
					t1->next = E->istream.tail;
					E->istream.tail->prev = t1;
					E->istream.tail = t1;
				}
			}
			else
			{
				mfree(E, t1->data, "t1->data in munchinput");
				mfree(E, t1, "t1 in munchinput");
			}
			/* careful : DO NOT free t1 ! */
		}
	}


	return;
}

void
clearistream(Engine *E)
{
	Datum	*p, *q;

	p = E->istream.head;
	while (p != NULL)
	{
		if (SF_DEBUG)
		{
			mprint(E, NULL, siminfo,
				"Discarding \"%s\" from istream...\n", p->data);
		}

		q = p;
		mfree(E, p->data, "p->data in clearistream");
		p = p->prev;
		mfree(E, q, "q in clearistream");
	}
	E->istream.tail = E->istream.head = E->istream.masthead = NULL;

	return;
}

void
scan_labels_and_globalvars(Engine *E)
{
	ulong	tmp_pc;
	Datum	*tmpistream = E->istream.tail;


	while (tmpistream != NULL)
	{
		/* 	If it is new a label, add it to labellist 		*/
		if (tmpistream->data[strlen(tmpistream->data)-1] == ':')
		{
			Datum*	tmplabel = (Datum *) mmalloc(E, sizeof(Datum), 
					"Datum *tmplabel in parser-driver.c, ~line 174");
			
			if (tmplabel == NULL)
			{
				mprint(E, NULL, siminfo, 
					"Could not allocate memory for Datum *tmplabel, main.c");
				mexit(E, "See above messages.", -1);
			}

			tmplabel->next = NULL;
			tmplabel->prev = NULL;
			tmplabel->data = (char*)mmalloc(E, strlen(tmpistream->data)*sizeof(char),
					"char *tmplabel->data in scan_labels...(), main.c");
			if (tmplabel->data == NULL)
			{
				mfree(E, tmplabel, "tmplabel in scan_labels_and_globalvars");
				mprint(E, NULL, siminfo,
					"Could not allocate memory for char *tmplabel->data, main.c");
				mexit(E, "See above messages.", -1);
			}
			tmplabel->value = E->cp->PC;

			
			strncpy(tmplabel->data, tmpistream->data, strlen(tmpistream->data) - 1);
			tmplabel->data[strlen(tmpistream->data)-1] = '\0';

			if ((E->labellist.head == NULL)  || (E->labellist.tail == NULL))
			{
				//mprint(E, NULL, siminfo, "New label list\n");
				E->labellist.tail = E->labellist.head = tmplabel;

				E->labellist.head->prev = NULL;
				E->labellist.head->next = NULL;

				E->labellist.tail->prev = NULL;
				E->labellist.tail->next = NULL;
			}
			else
			{
				//mprint(E, NULL, siminfo, "Adding new item to label list");
				/*							*/
				/*  		Add new datum to _tail_ of list.	*/
				/*							*/
				tmplabel->next = E->labellist.tail;
				E->labellist.tail->prev = tmplabel;
				E->labellist.tail = tmplabel;
			}
		}

		/*	If it is a global definition (".comm"), add it to labellist	*/
		if (!strcmp(tmpistream->data, ".comm"))
		{
			Datum* tmplabel = (Datum *) mmalloc(E, sizeof(Datum),
						"Datum *tmplabel in scan_labels...(), main.c");
			if (tmplabel == NULL)
			{
				mprint(E, NULL, siminfo,
					"Could not allocate memory for Datum *tmplabel, main.c");
				mexit(E, "See above messages.", -1);
			}

			/*	Token we just passed is global var name		*/
			tmpistream = tmpistream->prev;
			if (tmpistream == NULL)
			{
				mprint(E, NULL, siminfo,
					"Badly formed input stream: \".comm\" without a var name");
				mexit(E, "See above messages.", -1);
			}

			//mprint(E, NULL, siminfo, "Found a global var definition, var name = [%s]\n",
			//	tmpistream->data);

			tmplabel->next = NULL;
			tmplabel->prev = NULL;
			tmplabel->data = (char*)mmalloc(E, strlen(tmpistream->data)*sizeof(char),
					"char *tmplabel->data in parser-driver.c, ~line 250");
			tmplabel->value = E->cp->PC;


			if (tmplabel->data == NULL)
			{
				mfree(E, tmplabel, "tmplabel in scan_labels_and_globalvars");
				mprint(E, NULL, siminfo,
					"Could not allocate memory for char* tmplabel->data, main.c");
				mexit(E, "See above messages.", -1);
			}
			
			strncpy(tmplabel->data, tmpistream->data, strlen(tmpistream->data));

			/*	We went one step back. Step forward again	*/
			tmpistream = tmpistream->next;

			if ((E->labellist.head == NULL)  || (E->labellist.tail == NULL))
			{
				//mprint(E, NULL, siminfo, "New label list\n");
				E->labellist.tail = E->labellist.head = tmplabel;

				E->labellist.head->prev = NULL;
				E->labellist.head->next = NULL;

				E->labellist.tail->prev = NULL;
				E->labellist.tail->next = NULL;
			}
			else
			{
				//mprint(E, NULL, siminfo, "Adding new item to label list");
				/*							*/
				/*  		Add new datum to _tail_ of list.	*/
				/*							*/
				tmplabel->next = E->labellist.tail;
				E->labellist.tail->prev = tmplabel;
				E->labellist.tail = tmplabel;
			}
		}
		tmpistream = tmpistream->next;
	}

	//mprint(E, NULL, siminfo, "Done scan_labels_and_globalvars()...\n");
	//mprint(E, NULL, siminfo, "Calling yyparse(), with SCANNING flag set\n\n\n");

	E->scanning = 1;
	tmp_pc = E->cp->PC;
	if (E->cp->machinetype == MACHINE_SUPERH)
	{
		sf_superh_parse();
	}
	else if (E->cp->machinetype == MACHINE_RISCV)
	{
		sf_riscv_parse();
	}
	E->cp->PC = tmp_pc;
	E->scanning = 0;
	//mprint(E, NULL, siminfo, "Done assigning true disp's to labels.\n\n");
	
	/*	We screwed up istream.head, so reset it :	*/
	E->istream.head = E->istream.masthead;


	return;
}

void
streamchk(Engine *E)
{
	Datum *tmp = E->istream.head;

	mprint(E, NULL, siminfo, "Current input stream is: ");
	while (tmp != NULL)
	{
		mprint(E, NULL, siminfo, "[%s]", tmp->data);
		tmp = tmp->prev;
	}
	mprint(E, NULL, siminfo, "\n");

	return;
}

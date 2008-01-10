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
#include <string.h>
#include "sf.h"
#include "mextern.h"

void
munchinput(char *buf)
{	
	int 	eaten = 0;


	if (strlen(buf) > 0)
	{
              	while (eaten < strlen(buf))
               	{
			char *tptr;
			Datum *t1;

			t1 = (Datum *) mmalloc(sizeof(Datum),
					"Datum *t1 in munchinput(), main.c");

			if (t1 == NULL)
			{
				mprint(NULL, siminfo, "Could not allocate memory for Datum *t1");
				mexit("See above messages.", -1);
			}

			t1->next = NULL;
			t1->prev = NULL;
			t1->value = 0;

			t1->data = (char*) mmalloc((strlen(buf)+1)*sizeof(char), 
						"char *t1->data in munchinput(), main.c");
			if (t1->data == NULL)
			{
				mfree(t1, "char *t1 in main.c");
				mprint(NULL, siminfo, "Could not allocate memory for char *t1->data");
				mexit("See above messages.", -1);
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
				if ((istream.head == NULL)  || (istream.tail == NULL))
				{
					istream.tail = istream.head = istream.masthead = t1;

					/*							*/
					/*    NOTE tail and head now point to the lone datum 	*/
					/*    and they _both_ have null pre- and next-.		*/
					/*							*/
					istream.masthead->prev = NULL;
					istream.masthead->next = NULL;

					istream.head->prev = NULL;
					istream.head->next = NULL;

					istream.tail->prev = NULL;
					istream.tail->next = NULL;
				}
				else
				{
					/*							*/
					/*  Add new datum to _tail_ of list. MUST keep it FIFO 	*/
					/*  for the asm to be parsed correctly.			*/
					/*							*/
					t1->next = istream.tail;
					istream.tail->prev = t1;
					istream.tail = t1;
				}
			}
			else
			{
				mfree(t1->data, "t1->data in munchinput");
				mfree(t1, "t1 in munchinput");
			}
			/* careful : DO NOT free t1 ! */
		}
	}


	return;
}

void
clearistream(void)
{
	Datum	*p, *q;

	p = istream.head;
	while (p != NULL)
	{
		if (SF_DEBUG)
		{
			mprint(NULL, siminfo,
				"Discarding \"%s\" from istream...\n", p->data);
		}

		q = p;
		mfree(p->data, "p->data in clearistream");
		p = p->prev;
		mfree(q, "q in clearistream");
	}
	istream.tail = istream.head = istream.masthead = NULL;

	return;
}

void
scan_labels_and_globalvars(void)
{
	ulong	tmp_pc;
	Datum	*tmpistream = istream.tail;


	while (tmpistream != NULL)
	{
		/* 	If it is new a label, add it to labellist 		*/
		if (tmpistream->data[strlen(tmpistream->data)-1] == ':')
		{
			Datum*	tmplabel = (Datum *) mmalloc(sizeof(Datum), 
					"Datum *tmplabel in parser-driver.c, ~line 174");
			
			if (tmplabel == NULL)
			{
				mprint(NULL, siminfo, 
					"Could not allocate memory for Datum *tmplabel, main.c");
				mexit("See above messages.", -1);
			}

			tmplabel->next = NULL;
			tmplabel->prev = NULL;
			tmplabel->data = (char*)mmalloc(strlen(tmpistream->data)*sizeof(char),
					"char *tmplabel->data in scan_labels...(), main.c");
			if (tmplabel->data == NULL)
			{
				mfree(tmplabel, "tmplabel in scan_labels_and_globalvars");
				mprint(NULL, siminfo,
					"Could not allocate memory for char *tmplabel->data, main.c");
				mexit("See above messages.", -1);
			}
			tmplabel->value = CUR_STATE->PC;

			
			strncpy(tmplabel->data, tmpistream->data, strlen(tmpistream->data) - 1);
			tmplabel->data[strlen(tmpistream->data)-1] = '\0';

			if ((labellist.head == NULL)  || (labellist.tail == NULL))
			{
				//mprint(NULL, siminfo, "New label list\n");
				labellist.tail = labellist.head = tmplabel;

				labellist.head->prev = NULL;
				labellist.head->next = NULL;

				labellist.tail->prev = NULL;
				labellist.tail->next = NULL;
			}
			else
			{
				//mprint(NULL, siminfo, "Adding new item to label list");
				/*							*/
				/*  		Add new datum to _tail_ of list.	*/
				/*							*/
				tmplabel->next = labellist.tail;
				labellist.tail->prev = tmplabel;
				labellist.tail = tmplabel;
			}
		}

		/*	If it is a global definition (".comm"), add it to labellist	*/
		if (!strcmp(tmpistream->data, ".comm"))
		{
			Datum* tmplabel = (Datum *) mmalloc(sizeof(Datum),
						"Datum *tmplabel in scan_labels...(), main.c");
			if (tmplabel == NULL)
			{
				mprint(NULL, siminfo,
					"Could not allocate memory for Datum *tmplabel, main.c");
				mexit("See above messages.", -1);
			}

			/*	Token we just passed is global var name		*/
			tmpistream = tmpistream->prev;
			if (tmpistream == NULL)
			{
				mprint(NULL, siminfo,
					"Badly formed input stream: \".comm\" without a var name");
				mexit("See above messages.", -1);
			}

			//mprint(NULL, siminfo, "Found a global var definition, var name = [%s]\n",
			//	tmpistream->data);

			tmplabel->next = NULL;
			tmplabel->prev = NULL;
			tmplabel->data = (char*)mmalloc(strlen(tmpistream->data)*sizeof(char),
					"char *tmplabel->data in parser-driver.c, ~line 250");
			tmplabel->value = CUR_STATE->PC;


			if (tmplabel->data == NULL)
			{
				mfree(tmplabel, "tmplabel in scan_labels_and_globalvars");
				mprint(NULL, siminfo,
					"Could not allocate memory for char* tmplabel->data, main.c");
				mexit("See above messages.", -1);
			}
			
			strncpy(tmplabel->data, tmpistream->data, strlen(tmpistream->data));

			/*	We went one step back. Step forward again	*/
			tmpistream = tmpistream->next;

			if ((labellist.head == NULL)  || (labellist.tail == NULL))
			{
				//mprint(NULL, siminfo, "New label list\n");
				labellist.tail = labellist.head = tmplabel;

				labellist.head->prev = NULL;
				labellist.head->next = NULL;

				labellist.tail->prev = NULL;
				labellist.tail->next = NULL;
			}
			else
			{
				//mprint(NULL, siminfo, "Adding new item to label list");
				/*							*/
				/*  		Add new datum to _tail_ of list.	*/
				/*							*/
				tmplabel->next = labellist.tail;
				labellist.tail->prev = tmplabel;
				labellist.tail = tmplabel;
			}
		}
		tmpistream = tmpistream->next;
	}

	//mprint(NULL, siminfo, "Done scan_labels_and_globalvars()...\n");
	//mprint(NULL, siminfo, "Calling yyparse(), with SCANNING flag set\n\n\n");

	SCANNING = 1;
	tmp_pc = CUR_STATE->PC;
	yyparse();
	CUR_STATE->PC = tmp_pc;
	SCANNING = 0;
	//mprint(NULL, siminfo, "Done assigning true disp's to labels.\n\n");
	
	/*	We screwed up istream.head, so reset it :	*/
	istream.head = istream.masthead;


	return;
}

void
streamchk(void)
{
	Datum *tmp = istream.head;

	mprint(NULL, siminfo, "Current input stream is: ");
	while (tmp != NULL)
	{
		mprint(NULL, siminfo, "[%s]", tmp->data);
		tmp = tmp->prev;
	}
	mprint(NULL, siminfo, "\n");

	return;
}

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
#define	SEPCHARS " \t\n\r"

/*	Note: Newline is no longer an 'asmsep': changed grammar (for comments)	*/
#define isasmsep(c) 	((c == ' ')||(c == '\t')||\
		    	(c == '\r') ? 1 : 0)

/*	Note: Newline is now a 'sticky': changed grammar (for comments)		*/
#define issticky(c) 	((c == '#')||(c == '(')||\
			(c == ')')||(c == '@')||\
			(c == ',')||(c == '-')||\
			(c == '+')||(c == '\n')||\
			(c == '!') ? 1 : 0)

enum
{
	MAX_BUFLEN = 1024,
};

typedef struct
{
	char 	*data;
	long	value;
	void	*next;
	void	*prev;
} Datum;

typedef struct
{
	Datum	*masthead;
	Datum 	*head;
	Datum 	*tail;
} Input;

typedef struct
{
	Datum 	*head;
	Datum 	*tail;
} Labels;

void	input(char *);
void	load(char *);
void	scan_labels_and_globalvars(void);
void	streamchk(void);

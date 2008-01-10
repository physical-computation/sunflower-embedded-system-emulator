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
typedef struct
{
	unsigned	code:16;
} instr_0;

typedef struct
{
	unsigned 	code_lo:8;
	unsigned	dst:4;
	unsigned	code_hi:4;
} instr_n;

typedef struct
{
	unsigned 	code_lo:8;
	unsigned	src:4;
	unsigned	code_hi:4;
} instr_m;

typedef struct
{
	unsigned 	code_lo:4;
	unsigned	reg:3;
	unsigned   	fill:1;
	unsigned	src:4;
	unsigned	code_hi:4;
} instr_mbank;

typedef struct
{
	unsigned 	code_lo:4;
	unsigned	reg:3;
	unsigned   	fill:1;
	unsigned	dst:4;
	unsigned	code_hi:4;
} instr_nbank;

typedef struct
{
	unsigned 	code_lo:4;
	unsigned	src:4;
	unsigned	dst:4;
	unsigned	code_hi:4;
} instr_nm;

typedef struct
{
	unsigned	disp:4;
	unsigned	src:4;
	unsigned	code:8;
} instr_md;

typedef struct
{
	unsigned	disp:4;
	unsigned	src:4;
	unsigned	dst:4;
	unsigned 	code:4;
} instr_nmd;

typedef struct
{
	unsigned	disp:8;
	unsigned	code:8;
} instr_d8;

typedef struct
{
	unsigned	disp:12;
	unsigned	code:4;
} instr_d12;

typedef struct
{
	unsigned	disp:4;
	unsigned	dst:4;
	unsigned 	code:8;
} instr_nd4;

typedef struct
{
	unsigned	disp:8;
	unsigned	dst:4;
	unsigned	code:4;
} instr_nd8;

typedef struct
{
	unsigned	imm:8;
	unsigned	code:8;
} instr_i;

typedef struct
{
	unsigned	imm:8;
	unsigned	dst:4;
	unsigned	code:4;
} instr_ni;

typedef struct
{
	unsigned	nibble_1:4;
	unsigned	nibble_2:4;
	unsigned	nibble_3:4;
	unsigned	nibble_4:4;
} decode_instr;

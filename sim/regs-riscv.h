/*
	Copyright (c) 2017-2018, Zhengyang Gu (author)
 
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

enum
{
	RISCV_X0	= 0,
	RISCV_ZERO	= 0,

	RISCV_X1	= 1,
	RISCV_RA	= 1,

	RISCV_X2	= 2,
	RISCV_SP	= 2,

	RISCV_X3	= 3,
	RISCV_GP	= 3,

	RISCV_X4	= 4,
	RISCV_TP	= 4,

	RISCV_X5	= 5,
	RISCV_T0	= 5,

	RISCV_X6	= 6,
	RISCV_T1	= 6,

	RISCV_X7	= 7,
	RISCV_T2	= 7,

	RISCV_X8	= 8,
	RISCV_S0	= 8,
	RISCV_FP	= 8,

	RISCV_X9	= 9,
	RISCV_S1	= 9,

	RISCV_X10	= 10,
	RISCV_A0	= 10,

	RISCV_X11	= 11,
	RISCV_A1	= 11,

	RISCV_X12	= 12,
	RISCV_A2	= 12,

	RISCV_X13	= 13,
	RISCV_A3	= 13,

	RISCV_X14	= 14,
	RISCV_A4	= 14,

	RISCV_X15	= 15,
	RISCV_A5	= 15,

	RISCV_X16	= 16,
	RISCV_A6	= 16,

	RISCV_X17	= 17,
	RISCV_A7	= 17,

	RISCV_X18	= 18,
	RISCV_S2	= 18,

	RISCV_X19	= 19,
	RISCV_S3	= 19,

	RISCV_X20	= 20,
	RISCV_S4	= 20,

	RISCV_X21	= 21,
	RISCV_S5	= 21,

	RISCV_X22	= 22,
	RISCV_S6	= 22,

	RISCV_X23	= 23,
	RISCV_S7	= 23,

	RISCV_X24	= 24,
	RISCV_S8	= 24,

	RISCV_X25	= 25,
	RISCV_S9	= 25,

	RISCV_X26	= 26,
	RISCV_S10	= 26,

	RISCV_X27	= 27,
	RISCV_S11	= 27,

	RISCV_X28	= 28,
	RISCV_T3	= 28,

	RISCV_X29	= 29,
	RISCV_T4	= 29,

	RISCV_X30	= 30,
	RISCV_T5	= 30,

	RISCV_X31	= 31,
	RISCV_T6	= 31,

	/*Number of general purpose registers (x0 is not considered as general purpose)*/
	RISCV_GPR   = 31,

	RISCV_XMAX, 
};


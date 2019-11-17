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

#include "inst_uncertain.h"

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


/* RISC-V floating point registers */
enum
{
	RV32FD_f0   = 0,
	RV32FD_ft0  = 0,
	
	RV32FD_f1   = 1,
	RV32FD_ft1  = 1,
	
	RV32FD_f2   = 2,
	RV32FD_ft2  = 2,
	
	RV32FD_f3   = 3,
	RV32FD_ft3  = 3,
	
	RV32FD_f4   = 4,
	RV32FD_ft4  = 4,
	
	RV32FD_f5   = 5,
	RV32FD_ft5  = 5,
	
	RV32FD_f6   = 6,
	RV32FD_ft6  = 6,
	
	RV32FD_f7   = 7,
	RV32FD_ft7  = 7,
	
	RV32FD_f8   = 8,
	RV32FD_fs0  = 8,
	
	RV32FD_f9   = 9,
	RV32FD_fs1  = 9,
	
	RV32FD_f10  = 10,
	RV32FD_fa0  = 10,
	
	RV32FD_f11  = 11,
	RV32FD_fa1  = 11,
	
	RV32FD_f12  = 12,
	RV32FD_fa2  = 12,
	
	RV32FD_f13  = 13,
	RV32FD_fa3  = 13,
	
	RV32FD_f14  = 14,
	RV32FD_fa4  = 14,
	
	RV32FD_f15  = 15,
	RV32FD_fa5  = 15,
	
	RV32FD_f16  = 16,
	RV32FD_fa6  = 16,
	
	RV32FD_f17  = 17,
	RV32FD_fa7  = 17,
	
	RV32FD_f18  = 18,
	RV32FD_fs2  = 18,
	
	RV32FD_f19  = 19,
	RV32FD_fs3  = 19,
	
	RV32FD_f20  = 20,
	RV32FD_fs4  = 20,
	
	RV32FD_f21  = 21,
	RV32FD_fs5  = 21,
	
	RV32FD_f22  = 22,
	RV32FD_fs6  = 22,
	
	RV32FD_f23  = 23,
	RV32FD_fs7  = 23,
	
	RV32FD_f24  = 24,
	RV32FD_fs8  = 24,
	
	RV32FD_f25  = 25,
	RV32FD_fs9  = 25,
	
	RV32FD_f26  = 26,
	RV32FD_fs10 = 26,
	
	RV32FD_f27  = 27,
	RV32FD_fs11 = 27,
	
	RV32FD_f28  = 28,
	RV32FD_ft8  = 28,
	
	RV32FD_f29  = 29,
	RV32FD_ft9  = 29,
	
	RV32FD_f30  = 30,
	RV32FD_ft10 = 30,
	
	RV32FD_f31  = 31,
	RV32FD_ft11 = 31,
	
	/*Number of floating point registers*/
	RF32FD_fMAX = 32
};

typedef union
{
	uint32_t bit_value;
	float float_value;
} rv32f_rep;

typedef union
{
	uint64_t bit64_value;
	double double_value;
} rv32d_rep;



//if (INCLUDE_HISTOGRAM_IMPLEMENTATION == true){ // TODO conditional compilation via SF_ flag

/*
 * Histograms (Idea: Alexa's final project thesis, Initial implementation: Jan (jh2109))
 */


// TODO consider whether this should be part of the typedef, i.e. be dynamically allocated
//      that would be more flexible, but arguably less aligned with how a processor operating
//      on fixed-precision variables would work
// TODO C doesn't allow the next line, as it only accepts #defines/hardcoding for array size declarations
//      Would need to rewrite with e.g. malloc() instead to avoid define (suggestions welcome)
//const unsigned int kNBINS = 256;
//#define kNBINS 256
#define kNBINS 8 // TODO changed for debugging
typedef int32_t HistogramBinDatatype;

// TODO Is this the best place to define this datatype?
typedef struct
{
	// TODO I chose the more familiar term "bins" rather than Alexa's "weights".
	// The term "weights" does not appear in her report, so I am not sure of the motivation
	// to use weights instead ('weights' are more common in vector terminology?)
	HistogramBinDatatype bins[kNBINS];
} Histogram;
// TODO This is screaming "template me", but I am only familiar with C++ templates (e.g. histogram<uint32_t>)
//      For now this is fixed at compile-time via HistogramBinDatatype


// Add two distributions, considering overflow
void Histogram_AddDist(Histogram *hist1, Histogram *hist2, Histogram *histDest);

// Multiply each bin with a scalar
void Histogram_ScalarMultiply(Histogram *hist, HistogramBinDatatype scalar);

// Subtract two distributions, considering overflow
void Histogram_SubDist(Histogram *hist1, Histogram *hist2, Histogram *histDest);

// Add two distograms in the simple fashion of adding corresponding bins together
void Histogram_CombDist(Histogram *hist1, Histogram *hist2, Histogram *histDest);

// Returns the lower bound of the distribution, i.e. the bin index of the lowermost non-zero bin.
// Returns -1 if all bins are empty.
int Histogram_LowerBound(Histogram *hist);

// Returns the upper bound of the distribution, i.e. the bin index of the uppermost non-zero bin.
// Returns -1 if all bins are empty.
int Histogram_UpperBound(Histogram *hist);

// Load distribution
void Histogram_LDDist(Histogram *histogram, HistogramBinDatatype bins[kNBINS]);

// Load random histogram
void Histogram_LDRandom(Histogram *histogram);

// Return mean of given histogram
double Histogram_Mean(Histogram *histogram);

// Pretty-print histogram distribution
void Histogram_PrettyPrint(Engine *E, State *S, Histogram *histogram);

// Return the mean frequency of a histogram, i.e. the average bin value (not weighted by index)
double Histogram_MeanFrequency(Histogram *histogram);

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
#include <stdlib.h>
#include "sf.h"
#include <stdint.h>
#include <string.h>

tuck
uint32_t reg_read_riscv(Engine *E, State *S, uint8_t n)
{
	uint32_t data;

	/*
	 *	TODO The following looks like an assert(register_accessed < total_number_of_registers) to me?
	 */
	if (n <= RISCV_GPR)
	{
		data = S->riscv->R[n];
	}

	switch(n) //For CSR access?
	{
		default:
		{
		}
	}

	return data;
}

tuck
void reg_set_riscv(Engine *E, State *S, uint8_t n, uint32_t data)
{
	/*
	 *	TODO The following looks like an assert(register_accessed < total_number_of_registers) to me,
	 *	with an additional check for X0. X0 is a special register, not general purpose:
	 *	'Register x0 is hardwired to the constant 0.' ISA spec p9
	 */
	if (n <= RISCV_GPR && n != RISCV_X0)
	{
		S->riscv->R[n] = data;
	}

	switch(n) //For CSR access?
	{
		case RISCV_X0:
		{
		}
		default:
		{
		}
	}

	return;
}

/* RV32F floating point register access */
tuck
uint64_t freg_read_riscv(Engine *E, State *S, uint8_t n)
{
	uint64_t data;

	// TODO: probably an assert, see above
	if (n < RF32FD_fMAX)
	{
		data = S->riscv->fR[n];
	}

	return data;
}

tuck
void freg_set_riscv(Engine *E, State *S, uint8_t n, uint64_t data)
{
	// TODO: probably an assert, see above
	if (n < RF32FD_fMAX)
	{
		S->riscv->fR[n] = data;
	}

	return;
}



void
Histogram_AddDist(Histogram *hist1, Histogram *hist2, Histogram *histDest){
	/*
	 * Add two distributions, considering overflow
	 */

	// TODO meaning of abbreviation "wid"? Width?
	// TODO choice of data type explained by? go through entire code.
	//      Alexa's implementation was based around uint16_t bins
	uint16_t overflow_wid = 0;
	uint32_t overflow_hi = 0;

	// Zero-out destination histogram
	// TODO Alexa wrote "just in case" -- why?
	//      Presumably this is to say it is our responsibility to initialise
	for (int k = 0; k < kNBINS; k++){
		histDest->bins[k] = 0;
	}

	// Iterate, adding with overflow
	for (int j = 0; j < kNBINS; j++){
		for (int i = 0; i < kNBINS; i++){
			overflow_wid = i+j;
			/*printf("overflow_wid = %u\n", overflow_wid);*/

			if (overflow_wid < kNBINS){
				overflow_hi = histDest->bins[i+j] + (uint32_t)((uint32_t)hist1->bins[i] * hist2->bins[j]);
				/*printf("histdestbinsij = %u\n", histDest->bins[i+j]);*/
				/*printf("hist1i = %u\n", hist1->bins[i]);*/
				/*printf("hist2j = %u\n", hist2->bins[j]);*/
				/*printf("overflow_hi = %u\n", overflow_hi);*/

				if (overflow_hi < 65536){
					/*printf("overflow_hi<65536\n");*/
					histDest->bins[i+j] += hist1->bins[i] * hist2->bins[j];
				}
				else{
					// Bin overflow error
					// TODO implement (also missing from original implementation -- how to handle?)
					/*printf("UNIMPLEMENTED bin overflow error\n");*/
				}
			}
			else{
				// Value overflow error
				// TODO implement (also missing from original implementation -- how to handle?)
					/*printf("UNIMPLEMENTED value overflow error\n");*/
			}
		}
	}

	// TODO Idea: normalise to same mean frequency?
	/*double meanFreq = Histogram_MeanFrequency(histDest);*/
	/*for (int i = 0; i < kNBINS; i++){*/
		/*histDest->bins[i] /= (meanFreq);*/
	/*}*/

	// TODO Idea: normalise to same full scale (say, 255?)
	/*for (int i = 0; i < kNBINS; i++){*/
		/*histDest->bins[i] /= ;*/
	/*}*/
	
	
	return;
}

void Histogram_ScalarMultiply(Histogram *hist, HistogramBinDatatype scalar){
	/*
	 * Multiply each bin with a scalar
	 */
	
	for (int k = 0; k < kNBINS; k++){
		hist->bins[k] = scalar * hist->bins[k];
	}

	return;
}

void Histogram_SubDist(Histogram *hist1, Histogram *hist2, Histogram *histDest){
	/*
	 * Subtract two distributions, considering overflow
	 */

	// We want to reuse the code for AddDist. To do this, we transform in-place as follows
	
	// Negate one of the histograms (pick hist2)
	Histogram_ScalarMultiply(hist2, -1);

	// Add together to give the result
	Histogram_AddDist(hist1, hist2, histDest);

	// Undo the change to hist2
	Histogram_ScalarMultiply(hist2, -1);

	return;

	/* Caveat: Is the above equivalent to the following code in all cases?
	 *
	 UInt uncertain_SubDist(UInt Urs1, UInt Urs2, UInt Ud){ //Subtracts distributions

		uint32_t overflow_hi = 0;


		int k = 0;
		int j = 0;
		int i = 0;

		// Empty Ud (just in case)
		for(k = 0; k < 256; k++){
			Ud.weights[i] = 0;
		}


		for (j = 0; j < 256; j++){
			for(i=0; i<256; i++){
				if(i >= j){

					overflow_hi = Ud.weights[i - j] + (uint32_t)((uint32_t)Urs1.weights[i] * Urs2.weights[j]); //Nasty casting here

					if(overflow_hi < 65536){
						Ud.weights[i - j] += Urs1.weights[i] * Urs2.weights[j];

					}else{
						//Bin overflow error
					}
				}else{
					//Value underflow error
				}
			}
		}

			return Ud;
		}
	*/


}

void Histogram_CombDist(Histogram *hist1, Histogram *hist2, Histogram *histDest){
	/*
	 * Add two distograms in the simple fashion of adding corresponding bins together
	 */
	
	for (int k = 0; k < kNBINS; k++){
		histDest->bins[k] = hist1->bins[k] * hist2->bins[k];
	}

	return;
}


int Histogram_LowerBound(Histogram *hist){
	/*
	 * Returns the lower bound of the distribution, i.e. the bin index of the lowermost non-zero bin.
	 *
	 * Returns -1 if all bins are empty.
	 */
	
	for (int k = 0; k < kNBINS; k++){
		if (hist->bins[k] != 0){
			return k;
		}
	}

	return -1;
}

int Histogram_UpperBound(Histogram *hist){
	/*
	 * Returns the upper bound of the distribution, i.e. the bin index of the uppermost non-zero bin.
	 *
	 * Returns -1 if all bins are empty.
	 */
	
	for (int k = kNBINS-1; k >= 0; k--){
		if (hist->bins[k] != 0){
			return k;
		}
	}

	return -1;
}

void Histogram_DistLShift(Histogram *hist1, uint8_t Rs2, Histogram *histDest){
	/*
	 * DistLShift shifts the heights of the bins left by rs2, effectively subtracting rs2 from each bin.
	 * This reduces the mean of the distribution by rs2.
	 */
    int k = 0;
    int i = 0;

    for(k = 0; k < 256; k++){
		histDest->bins[k] = 0;
	}

	for(i = 0; i < 256; i++){
        if(i >= Rs2){
            histDest->bins[i-Rs2] = hist1->bins[i];
        }
	}

	return;
}

void Histogram_DistRShift(Histogram *hist1, uint8_t Rs2, Histogram *histDest){
	/*
	 * DistRShift shifts the heights of the bins right by rs2, effectively adding rs2 to each bin.
	 * This increases the mean of the distribution by rs2.
	 */
    int k = 0;
    int i = 0;
    uint16_t overflow_wid = 0;

    for(k = 0; k < 256; k++){
		histDest->bins[k] = 0;
	}

	for(i = 0; i < 256; i++){
        overflow_wid = (int16_t) (i + Rs2);
        if(overflow_wid < 256){
            histDest->bins[i+Rs2] = hist1->bins[i];
        }
	}

	return;
}


uint8_t Histogram_ExpectedValue(Histogram *hist, uint8_t Rd){
	/*
	 * Exp returns the expected value of the histogram. This provides an estimate of the 
	 * variable and allows the histogram to be converted into a point value.
	 */
	int sum = 0;
	int n = 0;
	int mean = 0;
	int i = 0;

	for(i = 0; i<256; i++){
			sum += ((int)hist->bins[i] * i);
			n += (int)hist->bins[i];
	}

	mean = sum / n;
	Rd = (uint8_t) mean;

	return Rd;
}


uint32_t Histogram_DistLess(Histogram *hist, uint32_t Rs2, uint32_t Rd){
	/*
	 * DistLess returns the probability Pr(X < Rs2). 
	 * X is a discrete random variable distributed according to the relative frequencies of hist1. 
	 * The probability is returned as an unsigned integer between 0 and 100 representing a percentage. 
	 * It is expected that this instruction will often be followed by one of the branch instructions 
	 * in the base instruction set.
	 */
	int i = 0;
	int num = 0;
	int denom = 0;
	uint32_t outcome = 0;


	for(i = 0; i < 256; i++){
			if(i < Rs2){
					num = num + hist->bins[i]; //If it is less then
			}
			denom = denom + hist->bins[i];
	}

	if(denom!=0){
			outcome = (num * 100) / denom; //Note this is integer division. Note that the times by 100 is to make it a percent rather than decimal which cannot be stored easily
			Rd = outcome;
			return Rd;
	}else{
			Rd = -1;
			return Rd;
	}
}

uint32_t Histogram_DistGrt(Histogram *hist, uint32_t Rs2, uint32_t Rd){
	/*
	 * DistLess returns the probability Pr(X >= Rs2). 
	 * X is a discrete random variable distributed according to the relative frequencies of hist1. 
	 * The probability is returned as an unsigned integer between 0 and 100 representing a percentage. 
	 * It is expected that this instruction will often be followed by one of the branch instructions 
	 * in the base instruction set.
	 */
	int i = 0;
	int num = 0;
	int denom = 0;
	uint32_t outcome = 0;


	for(i = 0; i < 256; i++){
			if(i >= Rs2){
					num = num + hist->bins[i]; //If it is less then
			}
			denom = denom + hist->bins[i];
	}

	if(denom!=0){
			outcome = (num * 100) / denom; //Note this is integer division. Note that the times by 100 is to make it a percent rather than decimal which cannot be stored easily
			Rd = outcome;
			return Rd;
	}else{
			Rd = -1;
			return Rd;
	}
}





void Histogram_LDDist(Histogram *histogram, HistogramBinDatatype *bins){
	/*
	 * Load a kNBINS-sized array of HistogramBinDatatype into the Histogram class
	 */
	memcpy(histogram->bins, bins, sizeof(HistogramBinDatatype)*kNBINS);

	return;
}


void Histogram_LDRandom(Histogram *histogram){
	/*
	 * Initialise *histogram with random values in each bin
	 */

	// Create array
	HistogramBinDatatype array[kNBINS] = {};
	for (int i = 0; i < kNBINS; i++){
		array[i] = (rand()/(double)RAND_MAX) * 255; // TODO picked some reasonable max value allowing by-eye debugging, increase to data type maximum later:
		/*array[i] = (rand()/(double)RAND_MAX) * ((HistogramBinDatatype)~(HistogramBinDatatype)0);*/
		// The final expression finds the maximum value this datatype can take
	}

	// Load into histogram
	Histogram_LDDist(histogram, array);

	return;
}

double Histogram_MeanFrequency(Histogram *histogram){
	/*
	 * Return the mean frequency of a histogram, i.e. the average bin value (not weighted by index)
	 */

	double sum = 0; // TODO could be performance-optimised if histogram max sum is known from kNBINS*sizeof(HistogramBinDatatype). Playing it safe here

	for (int i = 0; i < kNBINS; i++){
		sum += histogram->bins[i];
	}

	return sum / (double)kNBINS;
}


void Histogram_PrettyPrint(Engine *E, State *S, Histogram *histogram){
	/*
	 * Pretty-print ("ASCII-graph") a normalised histogram representation, like so:
	 *
	 * +-----> bin value
	 * |
	 * | #
	 * | ##
	 * | ###
	 * | ##
	 * | #
	 * |
	 * V bin index
	 *
	 */

	double normalised[kNBINS] = {};
	double meanFreq = Histogram_MeanFrequency(histogram);

	/*HistogramBinDatatype FULLSCALE = (HistogramBinDatatype)~(HistogramBinDatatype)0;*/
	// This expression finds the maximum value this datatype can take
	
	// Alternatively, auto-scaling could be done by the following:
	double FULLSCALE = 3 * meanFreq;

	const int FULLSCALE_NUMBER_OF_CHARS = 40;

	for (int i = 0; i < kNBINS; i++){
		normalised[i] = histogram->bins[i] / FULLSCALE;
	}

	mprint(E, S, nodeinfo, "Histogram mean frequency (mean bin occupation): %.3f\n", meanFreq);
	mprint(E, S, nodeinfo, "bin | value | graphical representation (scaled rel. to mean freq)\n");
	mprint(E, S, nodeinfo, "----+-------+----------------------------------------------------\n");

	for (int i = 0; i < kNBINS; i++){
		mprint(E, S, nodeinfo, "%03u | %-5u | ", i, histogram->bins[i]);
		/*mprint(E, S, nodeinfo, "%f\n", (normalised[i]));*/
		/*mprint(E, S, nodeinfo, "%f\n", (normalised[i]*FULLSCALE_NUMBER_OF_CHARS));*/
		for (int j = 0; j < (int)(normalised[i]*FULLSCALE_NUMBER_OF_CHARS); j++){
			mprint(E, S, nodeinfo, "#");
		}
		mprint(E, S, nodeinfo, "\n");
	}

	return;
}

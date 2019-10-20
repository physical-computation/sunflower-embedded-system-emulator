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
#include "sf.h"
#include <stdint.h>

tuck
uint32_t reg_read_riscv(Engine *E, State *S, uint8_t n)
{
	uint32_t data;

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

	if (n < RF32FD_fMAX)
	{
		data = S->riscv->fR[n];
	}

	return data;
}

tuck
void freg_set_riscv(Engine *E, State *S, uint8_t n, uint64_t data)
{
	if (n < RF32FD_fMAX)
	{
		S->riscv->fR[n] = data;
	}

	return;
}

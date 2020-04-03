/*
	Copyright (c) 2018-2019, Harry Sarson (author)

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
	kUncertainRegisterSize		= 32,
};

enum
{
	COVARIANCES_REG_REG		= (((kUncertainRegisterSize - 1) * kUncertainRegisterSize) / 2)
};

typedef struct
{
	float *		variances;
	float *		covariances;
} UncertainMemory;

typedef struct
{
	float		variances[kUncertainRegisterSize];
	float		covariances_reg_reg[COVARIANCES_REG_REG];
	float *		covariances_reg_mem;
} UncertainRegisters;

typedef enum
{
	UncertainOperationNone,
	UncertainOperationLoadStore,
	UncertainOperationUpdate,
} UncertainOperationType;

typedef struct
{
	uint32_t	op_fp_pc;
	uint32_t	insn_part1;
	int		valid;
} UncertainUpdateInfo;

typedef struct
{
	UncertainUpdateInfo	last_op;
	UncertainMemory		memory;
	UncertainRegisters	registers;
	int			memory_size;
} UncertainState;

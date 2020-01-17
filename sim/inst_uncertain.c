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

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include "sf.h"

enum
{
	PRINT_DIGITS				= 5,
	PRINT_JUST_REGISTERS			= 1,

	/*
	 *	unicode symbols may require 4 bytes
	 */
	PRINT_DIGIT_BUFFER_SIZE			= (PRINT_DIGITS * 4 + 1),
};

#define EPSILON					(1e-5f)



static size_t
covariances_in_mem (size_t memory_size)
{
	return (memory_size - 1) * memory_size / 2;
}

static size_t
covariances_in_reg_mem (size_t register_size, size_t memory_size)
{
	return register_size * memory_size;
}



/*
 *	Access to uncertain registers
 */



static int
get_offset_reg_reg(int row, int col)
{
	assert(row >= 0);
	assert(col >= 1);
	assert(row < col);
	assert(col < kUncertainRegisterSize);
	assert(row < kUncertainRegisterSize - 1);

	return (col - 1 + row * (2 * kUncertainRegisterSize - 3 - row) / 2);
}

static int
get_offset_reg_mem(int memory_size, int row, int col)
{
	assert(row >= 0);
	assert(col >= 0);
	assert(col < memory_size);
	assert(row < kUncertainRegisterSize);

	return row *memory_size + col;
}

static float
get_uncertain_variance_reg(UncertainState *state, int i)
{
	assert(i >= 0);
	assert(i < kUncertainRegisterSize);

	return state->registers.variances[i];
}

static void
set_uncertain_variance_reg(UncertainState *state, int i, float value)
{
	assert(i >= 0);
	assert(i < kUncertainRegisterSize);

	if (value < 0 && value > -EPSILON)
	{
		value = 0;
	}
	assert(value >= 0 || isnan(value));
	state->registers.variances[i] = value;
}

static float
get_uncertain_covariance_reg_reg(UncertainState *state, int row, int col)
{
	int offset = get_offset_reg_reg(row, col);
	return state->registers.covariances_reg_reg[offset];
}

static void
set_uncertain_covariance_reg_reg(UncertainState *state, int row, int col, float value)
{
	int	offset = get_offset_reg_reg(row, col);
	state->registers.covariances_reg_reg[offset] = value;
}

static float
get_uncertain_covariance_reg_mem(UncertainState *state, int row, int col)
{
	int	offset = get_offset_reg_mem(state->memory_size, row, col);
	return state->registers.covariances_reg_mem[offset];
}

static void
set_uncertain_covariance_reg_mem(UncertainState *state, int row, int col, float value)
{
	int	offset = get_offset_reg_mem(state->memory_size, row, col);
	state->registers.covariances_reg_mem[offset] = value;
}



/*
 *	Access to uncertain memory
 */



static int
get_offset_mem_mem(int row, int col)
{
	assert(row >= 0);
	assert(col >= 1);
	assert(row < col);

	return (col - 1) * col / 2 + row;
}

static float
get_uncertain_variance_mem(UncertainState *state, int i)
{
	assert(i >= 0);
	assert(i < state->memory_size);

	return state->memory.variances[i];
}

static void
set_uncertain_variance_mem(UncertainState *state, int i, float value)
{
	assert(i >= 0);
	assert(i < state->memory_size);

	if (value < 0 && value > -EPSILON)
	{
		value = 0;
	}

	assert(value >= 0 || isnan(value));
	state->memory.variances[i] = value;
}

static float
get_uncertain_covariance_mem_mem(UncertainState *state, int row, int col)
{
	assert(row < state->memory_size - 1);
	assert(col < state->memory_size);
	int offset = get_offset_mem_mem(row, col);

	return state->memory.covariances[offset];
}

static void
set_uncertain_covariance_mem_mem(UncertainState *state, int row, int col, float value)
{
	assert(row < state->memory_size - 1);
	assert(col < state->memory_size);
	int offset = get_offset_mem_mem(row, col);
	state->memory.covariances[offset] = value;
}



/*
 *	Uncertain operations
 */



void
uncertain_inst_lr(UncertainState *uncertain_state, int ud, int location)
{
	int	i;
	float	to_set;

	assert(ud < kUncertainRegisterSize);
	assert(location < uncertain_state->memory_size);

	/*
	 *	Load variance from memory and store in registers
	 */
	to_set = get_uncertain_variance_mem(uncertain_state, location);
	set_uncertain_variance_reg(uncertain_state, ud, to_set);
	set_uncertain_covariance_reg_mem(uncertain_state, ud, location, to_set);

	/*
	 *	Load covariances from registers and store in registers
	 */
	for (i = 0; i < ud; ++i)
	{
		to_set = get_uncertain_covariance_reg_mem(uncertain_state, i, location);
		set_uncertain_covariance_reg_reg(uncertain_state, i, ud, to_set);
	}
	for (i = ud + 1; i < kUncertainRegisterSize; ++i)
	{
		to_set = get_uncertain_covariance_reg_mem(uncertain_state, i, location);
		set_uncertain_covariance_reg_reg(uncertain_state, ud, i, to_set);
	}

	/*
	 *	Load covariances from memory and store in registers
	 */
	for (i = 0; i < location; ++i)
	{
		to_set = get_uncertain_covariance_mem_mem(uncertain_state, i, location);
		set_uncertain_covariance_reg_mem(uncertain_state, ud, i, to_set);
	}
	for (i = location + 1; i < uncertain_state->memory_size; ++i)
	{
		to_set = get_uncertain_covariance_mem_mem(uncertain_state, location, i);
		set_uncertain_covariance_reg_mem(uncertain_state, ud, i, to_set);
	}
}

void
uncertain_inst_sr(UncertainState *uncertain_state, int us1, int location)
{
	int	i;
	float	to_set;

	assert(us1 >= 0);
	assert(location >= 0);
	assert(us1 < kUncertainRegisterSize);
	assert(location < uncertain_state->memory_size);

	/*
	 *	Store variance
	 */
	to_set = get_uncertain_variance_reg(uncertain_state, us1);
	set_uncertain_variance_mem(uncertain_state, location, to_set);
	set_uncertain_covariance_reg_mem(uncertain_state, us1, location, to_set);

	/*
	 *	Store covariances in registers
	 */
	for (i = 0; i < us1; ++i)
	{
		to_set = get_uncertain_covariance_reg_reg(uncertain_state, i, us1);
		set_uncertain_covariance_reg_mem(uncertain_state, i, location, to_set);
	}
	for (i = us1 + 1; i < kUncertainRegisterSize; ++i)
	{
		to_set = get_uncertain_covariance_reg_reg(uncertain_state, us1, i);
		set_uncertain_covariance_reg_mem(uncertain_state, i, location, to_set);
	}

	/*
	 *	Store covariances in memory
	 */
	for (i = 0; i < location; ++i)
	{
		to_set = get_uncertain_covariance_reg_mem(uncertain_state, us1, i);
		set_uncertain_covariance_mem_mem(uncertain_state, i, location, to_set);
	}
	for (i = location + 1; i < uncertain_state->memory_size; ++i)
	{
		to_set = get_uncertain_covariance_reg_mem(uncertain_state, us1, i);
		set_uncertain_covariance_mem_mem(uncertain_state, location, i, to_set);
	}
}

void
uncertain_inst_mv(UncertainState *uncertain_state, int ud, int us1)
{
	uncertain_inst_up1(uncertain_state, ud, us1, 1.0f);
}

void
uncertain_inst_up1(UncertainState *uncertain_state, int ud, int us1, float g1)
{
	int	i;

	assert(ud < kUncertainRegisterSize);
	assert(us1 < kUncertainRegisterSize);

	{
		float new_var =
			g1 * g1 * get_uncertain_variance_reg(uncertain_state, us1);
		set_uncertain_variance_reg(uncertain_state, ud, new_var);
	}

	for (i = 0; i < kUncertainRegisterSize; ++i)
	{
		if (i != ud)
		{
			float sigma;
			if (i < us1)
			{
				sigma = get_uncertain_covariance_reg_reg(uncertain_state, i, us1);
			}
			else if (i == us1)
			{
				sigma = get_uncertain_variance_reg(uncertain_state, us1);
			}
			else
			{
				sigma = get_uncertain_covariance_reg_reg(uncertain_state, us1, i);
			}

			float new_covar = g1 * sigma;

			if (i < ud)
			{
				set_uncertain_covariance_reg_reg(uncertain_state, i, ud, new_covar);
			}
			else
			{
				set_uncertain_covariance_reg_reg(uncertain_state, ud, i, new_covar);
			}
		}
	}
	for (i = 0; i < uncertain_state->memory_size; ++i)
	{
		float new_covar =
			g1 * get_uncertain_covariance_reg_mem(uncertain_state, us1, i);
		set_uncertain_covariance_reg_mem(uncertain_state, ud, i, new_covar);
	}
}

void
uncertain_inst_up2(UncertainState *uncertain_state, int ud, int us1, int us2, float g1, float g2)
{
	int	i;

	assert(ud < kUncertainRegisterSize);
	assert(us1 < kUncertainRegisterSize);
	assert(us2 < kUncertainRegisterSize);

	{
		float new_var;
		if (us1 < us2)
		{
			new_var =
				g1 * g1 * get_uncertain_variance_reg(uncertain_state, us1) +
				2 * g1 * g2 * get_uncertain_covariance_reg_reg(uncertain_state, us1, us2) +
				g2 * g2 * get_uncertain_variance_reg(uncertain_state, us2);
		}
		else if (us1 > us2)
		{
			new_var =
				g1 * g1 * get_uncertain_variance_reg(uncertain_state, us1) +
				2 * g1 * g2 * get_uncertain_covariance_reg_reg(uncertain_state, us2, us1) +
				g2 * g2 * get_uncertain_variance_reg(uncertain_state, us2);
		}
		else
		{
			new_var =
				(g1 + g2) * (g1 + g2) * get_uncertain_variance_reg(uncertain_state, us1);
		}
		set_uncertain_variance_reg(uncertain_state, ud, new_var);
	}

	for (i = 0; i < kUncertainRegisterSize; ++i)
	{
		if (i != ud)
		{
			float sigma1 = nanf("");
			if (i < us1)
			{
				sigma1 = get_uncertain_covariance_reg_reg(uncertain_state, i, us1);
			}
			else if (i == us1)
			{
				sigma1 = get_uncertain_variance_reg(uncertain_state, i);
			}
			else
			{
				sigma1 = get_uncertain_covariance_reg_reg(uncertain_state, us1, i);
			}

			float sigma2 = nanf("");
			if (i < us2)
			{
				sigma2 = get_uncertain_covariance_reg_reg(uncertain_state, i, us2);
			}
			else if (i == us2)
			{
				sigma2 = get_uncertain_variance_reg(uncertain_state, i);
			}
			else
			{
				sigma2 = get_uncertain_covariance_reg_reg(uncertain_state, us2, i);
			}

			float new_covar = g1 * sigma1 + g2 * sigma2;

			if (i < ud)
			{
				set_uncertain_covariance_reg_reg(uncertain_state, i, ud, new_covar);
			}
			else
			{
				set_uncertain_covariance_reg_reg(uncertain_state, ud, i, new_covar);
			}
		}
	}
	for (i = 0; i < uncertain_state->memory_size; ++i)
	{
		float new_covar =
			g1 * get_uncertain_covariance_reg_mem(uncertain_state, us1, i) +
			g2 * get_uncertain_covariance_reg_mem(uncertain_state, us2, i);
		set_uncertain_covariance_reg_mem(uncertain_state, ud, i, new_covar);
	}
}

void
uncertain_inst_sv(UncertainState *uncertain_state, int ud, float variance)
{
	int	i;

	assert(ud < kUncertainRegisterSize);
	assert(variance >= 0 || isnan(variance));

	set_uncertain_variance_reg(uncertain_state, ud, variance);

	for (i = 0; i < ud; ++i)
	{
		set_uncertain_covariance_reg_reg(uncertain_state, i, ud, 0);
	}

	for (i = ud + 1; i < kUncertainRegisterSize; ++i)
	{
		set_uncertain_covariance_reg_reg(uncertain_state, ud, i, 0);
	}

	for (i = 0; i < uncertain_state->memory_size; ++i)
	{
		set_uncertain_covariance_reg_mem(uncertain_state, ud, i, 0);
	}
}

float
uncertain_inst_gv(UncertainState *uncertain_state, int us1)
{
	assert(us1 < kUncertainRegisterSize);
	return get_uncertain_variance_reg(uncertain_state, us1);
}

float
uncertain_inst_gcov(UncertainState *uncertain_state, int us1, int us2)
{
	assert(us1 < kUncertainRegisterSize);
	assert(us2 < kUncertainRegisterSize);

	if (us1 == us2)
	{
		return get_uncertain_variance_reg(uncertain_state, us1);
	}
	else
	{
		return get_uncertain_covariance_reg_reg(uncertain_state, us1, us2);
	}
}

void
uncertain_sizemem(Engine *E, State *S, int size)
{
	void *	tmp;


	if (!SF_UNCERTAIN_UPE)
	{
		return;
	}

	/*
	 *	This used to be
	 *
	 *		size = (size - 0x4000) / 4
	 *
	 *	That is no longer correct as the S->MEMBASE is now adaptible
	 *	and memory also no longer always starts at offset 0x4000 from
	 *	MEMBASE. In any case,
	 */
	size /= 4;

/*
	if (size > (1<< 16))
	{
		mexit(E, "Cannot compute required number of covariances without overflow\n", 1);
	}
*/

	size_t variance_bytes = size * sizeof(float);
	size_t covariance_bytes = covariances_in_mem(size) * sizeof(float);
	size_t register_bytes = covariances_in_reg_mem(kUncertainRegisterSize, size) * sizeof(float);
	size_t required_bytes = variance_bytes + covariance_bytes + register_bytes;

	if (S->riscv->uncertain->memory_size == 0)
	{
		assert(S->riscv->uncertain->memory.variances == NULL);
		assert(S->riscv->uncertain->memory.covariances == NULL);
		assert(S->riscv->uncertain->registers.covariances_reg_mem == NULL);

		tmp = mcalloc(E, 1, required_bytes, "S->riscv->uncertain_memory");
		if (tmp == NULL)
		{
			mexit(E, "Could not allocate mem for uncertain memory\n", -1);
		}
		S->riscv->uncertain->memory.variances = tmp;
		S->riscv->uncertain->memory.covariances = tmp + variance_bytes;
		S->riscv->uncertain->registers.covariances_reg_mem = tmp + variance_bytes + covariance_bytes;
		S->riscv->uncertain->memory_size = size;
		return;
	}

	assert(S->riscv->uncertain->memory.variances != NULL);
	assert(S->riscv->uncertain->memory.covariances != NULL);
	assert(S->riscv->uncertain->registers.covariances_reg_mem != NULL);

	tmp = mrealloc(E, S->riscv->uncertain->memory.variances, required_bytes, "S->riscv->uncertain_memory");
	if (tmp == NULL)
	{
		mexit(E, "SIZEMEM failed: could not allocate uncertain memory\n", -1);
	}
	else
	{
		S->riscv->uncertain->memory.variances = tmp;
		S->riscv->uncertain->memory.covariances = tmp + variance_bytes;
		S->riscv->uncertain->registers.covariances_reg_mem = tmp + variance_bytes + covariance_bytes;
		S->riscv->uncertain->memory_size = size;
		mprint(E, S, nodeinfo, "Set uncertain memory size to %d values\n", size);
	}

	return;
}

static int
print_register_row(UncertainState *uncertain_state, FILE *stream, int row)
{
	int	i, result;
	int	characters_written = 0;
	char	buffer[PRINT_DIGIT_BUFFER_SIZE] = {'\0'};

	result = snprintf(buffer, PRINT_DIGITS + 1, " u%-*d", PRINT_DIGITS, row);
	if (result < 0)
	{
		return result;
	}

	result = fprintf(stream, "%s│", buffer);
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	for (i = 0; i < kUncertainRegisterSize; ++i)
	{
		if (i < row)
		{
			memset(buffer, ' ', PRINT_DIGITS);
		}
		else if (i == row)
		{
			float var = get_uncertain_variance_reg(uncertain_state, row);
			result = snprintf(buffer, PRINT_DIGITS + 1, "%#-*.*f",
							PRINT_DIGITS, PRINT_DIGITS, var);
			if (result < 0)
			{
				return result;
			}
		}
		else
		{
			float covar = get_uncertain_covariance_reg_reg(uncertain_state, row, i);
			if (covar == 0)
			{
				result = snprintf(buffer, PRINT_DIGITS + 1, "%-*d", PRINT_DIGITS, 0);
			}
			else
			{
				result = snprintf(buffer, PRINT_DIGITS + 1, "%#-*.*f",
								PRINT_DIGITS, PRINT_DIGITS, covar);
			}

			if (result < 0)
			{
				return result;
			}
		}
		const char *padding;
		if (i == 0)
		{
			padding = "";
		}
		else
		{
			padding = " ";
		}

		result = fprintf(stream, "%s%s", padding, buffer);

		if (result < 0)
		{
			return result;
		}
		else
		{
			characters_written += result;
		}
	}

	if (!PRINT_JUST_REGISTERS)
	{
		for (i = 0; i < uncertain_state->memory_size; ++i)
		{
			float covar = get_uncertain_covariance_reg_mem(uncertain_state, row, i);
			result = snprintf(buffer, PRINT_DIGITS + 1, "%-*.*g",
							PRINT_DIGITS, PRINT_DIGITS - 2 - (int)ceil(fabs(log10(covar))), covar);
			if (result < 0)
			{
				return result;
			}
			const char *padding;
			if (i == 0)
			{
				padding = "│";
			}
			else
			{
				padding = " ";
			}

			result = fprintf(stream, "%s%s", padding, buffer);

			if (result < 0)
			{
				return result;
			}
			else
			{
				characters_written += result;
			}
		}
	}

	result = fprintf(stream, "│\n");
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	return characters_written;
}

static int
print_memory_row(UncertainState *uncertain_state, FILE *stream, int row, const char *stack_indicator)
{
	int	i, result;
	int	characters_written = 0;
	char	buffer[PRINT_DIGIT_BUFFER_SIZE] = {'\0'};

	result = fprintf(stream, " %-*.*s│", PRINT_DIGITS - 1, PRINT_DIGITS - 1, stack_indicator);
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	for (i = 0; i < kUncertainRegisterSize; ++i)
	{
		memset(buffer, ' ', PRINT_DIGITS);
		const char *padding;
		if (i == 0)
		{
			padding = "";
		}
		else
		{
			padding = " ";
		}

		result = fprintf(stream, "%s%s", padding, buffer);

		if (result < 0)
		{
			return result;
		}
		else
		{
			characters_written += result;
		}
	}

	result = fprintf(stream, "│");
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	for (i = 0; i < uncertain_state->memory_size; ++i)
	{
		if (i < row)
		{
			memset(buffer, ' ', PRINT_DIGITS);
		}
		else if (i == row)
		{
			float var = get_uncertain_variance_mem(uncertain_state, row);
			result = snprintf(buffer, PRINT_DIGITS + 1, "%-*.*g",
							PRINT_DIGITS, PRINT_DIGITS - 2 - (int)ceil(fabs(log10(var))), var);
			if (result < 0)
			{
				return result;
			}
		}
		else
		{
			float covar = get_uncertain_covariance_mem_mem(uncertain_state, row, i);
			result = snprintf(buffer, PRINT_DIGITS + 1, "%-*.*g",
							PRINT_DIGITS, PRINT_DIGITS - 2 - (int)ceil(fabs(log10(covar))), covar);
			if (result < 0)
			{
				return result;
			}
		}
		const char *padding;
		if (i == 0)
		{
			padding = "";
		}
		else
		{
			padding = " ";
		}

		result = fprintf(stream, "%s%s", padding, buffer);
		if (result < 0)
		{
			return result;
		}
		else
		{
			characters_written += result;
		}
	}

	result = fprintf(stream, "│\n");
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	return characters_written;
}

static int
print_extreme_row(UncertainState *uncertain_state, FILE *stream, const char *label, const char *open, const char *close)
{
	int	i, len, result;
	int	characters_written = 0;
	char	buffer[PRINT_DIGIT_BUFFER_SIZE] = {'\0'};

	result = snprintf(buffer, PRINT_DIGITS + 1, "%-*s", PRINT_DIGITS, label);
	if (result < 0)
	{
		return result;
	}

	result = fprintf(stream, "%s%s", buffer, open);
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	for (	i = 0, len = (PRINT_JUST_REGISTERS ? kUncertainRegisterSize : kUncertainRegisterSize + uncertain_state->memory_size);
		i < len;
		++i)
	{
		const char *	padding;

		memset(buffer, ' ', PRINT_DIGITS);

		if (i == 0)
		{
			padding = "";
		}
		else
		{
			padding = " ";
		}

		result = fprintf(stream, "%s%s", padding, buffer);
		if (result < 0)
		{
			return result;
		}
		else
		{
			characters_written += result;
		}
	}

	result = fprintf(stream, "%s\n", close);
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	return characters_written;
}

static int
print_middle_row(UncertainState *uncertain_state, FILE *stream)
{
	int	i, j, result;
	int	characters_written = 0;
	char	buffer[PRINT_DIGIT_BUFFER_SIZE] = {'\0'};

	memset(buffer, ' ', PRINT_DIGITS);

	result = fprintf(stream, "%s├", buffer);
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	for (i = 0; i < kUncertainRegisterSize + uncertain_state->memory_size; ++i)
	{
		const char *padding;
		if (i == 0)
		{
			padding = "";
		}
		else if (i == kUncertainRegisterSize)
		{
			padding = "┼"; // " └";
		}
		else
		{
			padding = "─";
		}

		result = fprintf(stream, "%s", padding);
		if (result < 0)
		{
			return result;
		}
		else
		{
			characters_written += result;
		}
		for (j = 0; j < PRINT_DIGITS; ++j)
		{
			result = fprintf(stream, "─");
			if (result < 0)
			{
				return result;
			}
			else
			{
				characters_written += result;
			}
		}
	}

	result = fprintf(stream, "┤\n");
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	return characters_written;
}

int
uncertain_print_system(UncertainState *uncertain_state, FILE *stream)
{
	int	i, result;
	int	characters_written = 0;

	result = print_extreme_row(uncertain_state, stream, "REG", "┌", "┐");
	if (result < 0)
	{
		return result;
	}
	else
	{
		characters_written += result;
	}

	for (i = 0; i < kUncertainRegisterSize; ++i)
	{
		result = print_register_row(uncertain_state, stream, i);
		if (result < 0)
		{
			return result;
		}
		else
		{
			characters_written += result;
		}
	}

	if (PRINT_JUST_REGISTERS)
	{
		result = print_extreme_row(uncertain_state, stream, "", "└", "┘");
		if (result < 0)
		{
			return result;
		}
		else
		{
			characters_written += result;
		}
	}
	else
	{
		result = print_middle_row(uncertain_state, stream);

		/*
		 *	TODO: read stack_point from CSR
		 */
		// int stack_pointer = uncertain_inst_addisp(0);

		for (i = 0; i < uncertain_state->memory_size; ++i)
		{
			const char *label = "";
			// if (i == stack_pointer)
			//	 label = "^";
			// else if (i > stack_pointer)
			//	 label = "|";

			result = print_memory_row(uncertain_state, stream, i, label);
			if (result < 0)
			{
				return result;
			}
			else
			{
				characters_written += result;
			}
		}

		result = print_extreme_row(uncertain_state, stream, "stack", "└", "┘");
		if (result < 0)
		{
			return result;
		}
		else
		{
			characters_written += result;
		}
	}

	return characters_written;
}

#include "inst_uncertain.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define PRINT_DIGITS 5
#define PRINT_JUST_REGISTERS 1

// unicode symbols may require 4 bytes
#define PRINT_DIGIT_BUFFER_SIZE (PRINT_DIGITS * 4 + 1)

// Access to uncertain registers

static int get_offset_reg_reg(int row, int col)
{
    assert(row >= 0);
    assert(col >= 1);
    assert(row < col);
    assert(col < UNCERTAIN_REGISTER_SIZE);
    assert(row < UNCERTAIN_REGISTER_SIZE - 1);
    return (col - 1 + row * (2 * UNCERTAIN_REGISTER_SIZE - 3 - row) / 2);
}

static int get_offset_reg_mem(int row, int col)
{
    assert(row >= 0);
    assert(col >= 0);
    assert(col < UNCERTAIN_MEMORY_SIZE);
    assert(row < UNCERTAIN_REGISTER_SIZE);
    return row * UNCERTAIN_MEMORY_SIZE + col;
}

static float get_uncertain_variance_reg(UncertainRegisters *uncertain_registers, int i)
{
    assert(i >= 0);
    assert(i < UNCERTAIN_REGISTER_SIZE);
    return uncertain_registers->variances[i];
}

static void set_uncertain_variance_reg(UncertainRegisters *uncertain_registers, int i, float value)
{
    assert(i >= 0);
    assert(i < UNCERTAIN_REGISTER_SIZE);
    // printf("%g\n", value);
    assert(value >= 0 || isnan(value));
    uncertain_registers->variances[i] = value;
}

static float get_uncertain_covariance_reg_reg(UncertainRegisters *uncertain_registers, int row, int col)
{
    int offset = get_offset_reg_reg(row, col);
    return uncertain_registers->covariances_reg_reg[offset];
}

static void set_uncertain_covariance_reg_reg(UncertainRegisters *uncertain_registers, int row, int col, float value)
{
    int offset = get_offset_reg_reg(row, col);
    uncertain_registers->covariances_reg_reg[offset] = value;
}

static float get_uncertain_covariance_reg_mem(UncertainRegisters *uncertain_registers, int row, int col)
{
    int offset = get_offset_reg_mem(row, col);
    return uncertain_registers->covariances_reg_mem[offset];
}

static void set_uncertain_covariance_reg_mem(UncertainRegisters *uncertain_registers, int row, int col, float value)
{
    int offset = get_offset_reg_mem(row, col);
    uncertain_registers->covariances_reg_mem[offset] = value;
}

// Access to uncertain memory

static int get_offset_mem_mem(int row, int col)
{
    assert(row >= 0);
    assert(col >= 1);
    assert(row < col);
    assert(row < UNCERTAIN_MEMORY_SIZE - 1);
    assert(col < UNCERTAIN_MEMORY_SIZE);

    return (col - 1 + row * (2 * UNCERTAIN_MEMORY_SIZE - 3 - row) / 2);
}

static float get_uncertain_variance_mem(UncertainMemory *uncertain_memory, int i)
{
    assert(i >= 0);
    assert(i < UNCERTAIN_MEMORY_SIZE);
    return uncertain_memory->variances[i];
}

static void set_uncertain_variance_mem(UncertainMemory *uncertain_memory, int i, float value)
{
    assert(i >= 0);
    assert(i < UNCERTAIN_MEMORY_SIZE);
    assert(value >= 0);
    uncertain_memory->variances[i] = value;
}

static float get_uncertain_covariance_mem_mem(UncertainMemory *uncertain_memory, int row, int col)
{
    int offset = get_offset_mem_mem(row, col);
    return uncertain_memory->covariances[offset];
}

static void set_uncertain_covariance_mem_mem(UncertainMemory *uncertain_memory, int row, int col, float value)
{
    int offset = get_offset_mem_mem(row, col);
    uncertain_memory->covariances[offset] = value;
}

// Uncertain operations

void uncertain_inst_lr(UncertainState *uncertain_state, int ud, int location)
{
    int i;
    float to_set;
    assert(ud < UNCERTAIN_REGISTER_SIZE);
    assert(location < UNCERTAIN_MEMORY_SIZE);

    // Load variance from memory and store in registers
    to_set = get_uncertain_variance_mem(&uncertain_state->memory, location);
    set_uncertain_variance_reg(&uncertain_state->registers, ud, to_set);
    set_uncertain_covariance_reg_mem(&uncertain_state->registers, ud, location, to_set);

    // Load covariances from registers and store in registers
    for (i = 0; i < ud; ++i)
    {
        to_set = get_uncertain_covariance_reg_mem(&uncertain_state->registers, i, location);
        set_uncertain_covariance_reg_reg(&uncertain_state->registers, i, ud, to_set);
    }
    for (i = ud + 1; i < UNCERTAIN_REGISTER_SIZE; ++i)
    {
        to_set = get_uncertain_covariance_reg_mem(&uncertain_state->registers, i, location);
        set_uncertain_covariance_reg_reg(&uncertain_state->registers, ud, i, to_set);
    }

    // Load covariances from memory and store in registers
    for (i = 0; i < location; ++i)
    {
        to_set = get_uncertain_covariance_mem_mem(&uncertain_state->memory, i, location);
        set_uncertain_covariance_reg_mem(&uncertain_state->registers, ud, i, to_set);
    }
    for (i = location + 1; i < UNCERTAIN_MEMORY_SIZE; ++i)
    {
        to_set = get_uncertain_covariance_mem_mem(&uncertain_state->memory, location, i);
        set_uncertain_covariance_reg_mem(&uncertain_state->registers, ud, i, to_set);
    }
}

void uncertain_inst_sr(UncertainState *uncertain_state, int us1, int location)
{
    int i;
    float to_set;
    assert(us1 >= 0);
    assert(location >= 0);
    assert(us1 < UNCERTAIN_REGISTER_SIZE);
    assert(location < UNCERTAIN_MEMORY_SIZE);

    // Store variance
    to_set = get_uncertain_variance_reg(&uncertain_state->registers, us1);
    set_uncertain_variance_mem(&uncertain_state->memory, location, to_set);
    set_uncertain_covariance_reg_mem(&uncertain_state->registers, us1, location, to_set);

    // Store covariances in registers
    for (i = 0; i < us1; ++i)
    {
        to_set = get_uncertain_covariance_reg_reg(&uncertain_state->registers, i, us1);
        set_uncertain_covariance_reg_mem(&uncertain_state->registers, i, location, to_set);
    }
    for (i = us1 + 1; i < UNCERTAIN_REGISTER_SIZE; ++i)
    {
        to_set = get_uncertain_covariance_reg_reg(&uncertain_state->registers, us1, i);
        set_uncertain_covariance_reg_mem(&uncertain_state->registers, i, location, to_set);
    }

    // Store covariances in memory
    for (i = 0; i < location; ++i)
    {
        to_set = get_uncertain_covariance_reg_mem(&uncertain_state->registers, us1, i);
        set_uncertain_covariance_mem_mem(&uncertain_state->memory, i, location, to_set);
    }
    for (i = location + 1; i < UNCERTAIN_MEMORY_SIZE; ++i)
    {
        to_set = get_uncertain_covariance_reg_mem(&uncertain_state->registers, us1, i);
        set_uncertain_covariance_mem_mem(&uncertain_state->memory, location, i, to_set);
    }
}

void uncertain_inst_mv(UncertainState *uncertain_state, int ud, int us1)
{
    uncertain_inst_up1(uncertain_state, ud, us1, 1.0f);
}

void uncertain_inst_up1(UncertainState *uncertain_state, int ud, int us1, float g1)
{
    int i;
    assert(ud < UNCERTAIN_REGISTER_SIZE);
    assert(us1 < UNCERTAIN_REGISTER_SIZE);

    {
        float new_var =
            g1 * g1 * get_uncertain_variance_reg(&uncertain_state->registers, us1);
        set_uncertain_variance_reg(&uncertain_state->registers, ud, new_var);
    }

    for (i = 0; i < UNCERTAIN_REGISTER_SIZE; ++i)
    {
        if (i != ud)
        {
            float sigma;
            if (i < us1)
            {
                sigma = get_uncertain_covariance_reg_reg(&uncertain_state->registers, i, us1);
            }
            else if (i == us1)
            {
                sigma = get_uncertain_variance_reg(&uncertain_state->registers, us1);
            }
            else
            {
                sigma = get_uncertain_covariance_reg_reg(&uncertain_state->registers, us1, i);
            }

            float new_covar = g1 * sigma;

            if (i < ud)
            {
                set_uncertain_covariance_reg_reg(&uncertain_state->registers, i, ud, new_covar);
            }
            else
            {
                set_uncertain_covariance_reg_reg(&uncertain_state->registers, ud, i, new_covar);
            }
        }
    }
    for (i = 0; i < UNCERTAIN_MEMORY_SIZE; ++i)
    {
        float new_covar =
            g1 * get_uncertain_covariance_reg_mem(&uncertain_state->registers, us1, i);
        set_uncertain_covariance_reg_mem(&uncertain_state->registers, ud, i, new_covar);
    }
}

void uncertain_inst_up2(UncertainState *uncertain_state, int ud, int us1, int us2, float g1, float g2)
{
    int i;
    assert(ud < UNCERTAIN_REGISTER_SIZE);
    assert(us1 < UNCERTAIN_REGISTER_SIZE);
    assert(us2 < UNCERTAIN_REGISTER_SIZE);

    {
        float new_var;
        if (us1 < us2)
        {
            new_var =
                g1 * g1 * get_uncertain_variance_reg(&uncertain_state->registers, us1) +
                2 * g1 * g2 * get_uncertain_covariance_reg_reg(&uncertain_state->registers, us1, us2) +
                g2 * g2 * get_uncertain_variance_reg(&uncertain_state->registers, us2);
        }
        else if (us1 > us2)
        {
            new_var =
                g1 * g1 * get_uncertain_variance_reg(&uncertain_state->registers, us1) +
                2 * g1 * g2 * get_uncertain_covariance_reg_reg(&uncertain_state->registers, us2, us1) +
                g2 * g2 * get_uncertain_variance_reg(&uncertain_state->registers, us2);
        }
        else
        {
            new_var =
                (g1 + g2) * (g1 + g2) * get_uncertain_variance_reg(&uncertain_state->registers, us1);
        }
        set_uncertain_variance_reg(&uncertain_state->registers, ud, new_var);
    }

    for (i = 0; i < UNCERTAIN_REGISTER_SIZE; ++i)
    {
        if (i != ud)
        {
            float sigma1 = nanf("");
            if (i < us1)
            {
                sigma1 = get_uncertain_covariance_reg_reg(&uncertain_state->registers, i, us1);
            }
            else if (i == us1)
            {
                sigma1 = get_uncertain_variance_reg(&uncertain_state->registers, i);
            }
            else
            {
                sigma1 = get_uncertain_covariance_reg_reg(&uncertain_state->registers, us1, i);
            }

            float sigma2 = nanf("");
            if (i < us2)
            {
                sigma2 = get_uncertain_covariance_reg_reg(&uncertain_state->registers, i, us2);
            }
            else if (i == us2)
            {
                sigma2 = get_uncertain_variance_reg(&uncertain_state->registers, i);
            }
            else
            {
                sigma2 = get_uncertain_covariance_reg_reg(&uncertain_state->registers, us2, i);
            }

            float new_covar = g1 * sigma1 + g2 * sigma2;

            if (i < ud)
            {
                set_uncertain_covariance_reg_reg(&uncertain_state->registers, i, ud, new_covar);
            }
            else
            {
                set_uncertain_covariance_reg_reg(&uncertain_state->registers, ud, i, new_covar);
            }
        }
    }
    for (i = 0; i < UNCERTAIN_MEMORY_SIZE; ++i)
    {
        float new_covar =
            g1 * get_uncertain_covariance_reg_mem(&uncertain_state->registers, us1, i) +
            g2 * get_uncertain_covariance_reg_mem(&uncertain_state->registers, us2, i);
        set_uncertain_covariance_reg_mem(&uncertain_state->registers, ud, i, new_covar);
    }
}

void uncertain_inst_sv(UncertainState *uncertain_state, int ud, float variance)
{
    int i;
    assert(ud < UNCERTAIN_REGISTER_SIZE);
    assert(variance >= 0 || isnan(variance));

    set_uncertain_variance_reg(&uncertain_state->registers, ud, variance);

    for (i = 0; i < ud; ++i)
    {
        set_uncertain_covariance_reg_reg(&uncertain_state->registers, i, ud, 0);
    }

    for (i = ud + 1; i < UNCERTAIN_REGISTER_SIZE; ++i)
    {
        set_uncertain_covariance_reg_reg(&uncertain_state->registers, ud, i, 0);
    }

    for (i = 0; i < UNCERTAIN_MEMORY_SIZE; ++i)
    {
        set_uncertain_covariance_reg_mem(&uncertain_state->registers, ud, i, 0);
    }
}

float uncertain_inst_gv(UncertainState *uncertain_state, int us1)
{
    assert(us1 < UNCERTAIN_REGISTER_SIZE);
    return get_uncertain_variance_reg(&uncertain_state->registers, us1);
}

static int print_register_row(UncertainState *uncertain_state, FILE *stream, int row)
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    int i, result;
    int characters_written = 0;
    char buffer[PRINT_DIGIT_BUFFER_SIZE] = {'\0'};

    result = snprintf(buffer, PRINT_DIGITS + 1, " u%-*d",
                      PRINT_DIGITS, row);

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

    for (i = 0; i < UNCERTAIN_REGISTER_SIZE; ++i)
    {
        if (i < row)
        {
            memset(buffer, ' ', PRINT_DIGITS);
        }
        else if (i == row)
        {
            float var = get_uncertain_variance_reg(&uncertain_state->registers, row);
            result = snprintf(buffer, PRINT_DIGITS + 1, "%#-*.*f",
                              PRINT_DIGITS, PRINT_DIGITS, var);
            if (result < 0)
            {
                return result;
            }
        }
        else
        {
            float covar = get_uncertain_covariance_reg_reg(&uncertain_state->registers, row, i);
            if (covar == 0) {
                result = snprintf(buffer, PRINT_DIGITS + 1, "%-*d", PRINT_DIGITS, 0);
            } else {
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
            padding = "";
        else
            padding = " ";
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
        for (i = 0; i < UNCERTAIN_MEMORY_SIZE; ++i)
        {
            float covar = get_uncertain_covariance_reg_mem(&uncertain_state->registers, row, i);
            result = snprintf(buffer, PRINT_DIGITS + 1, "%-*.*g",
                            PRINT_DIGITS, PRINT_DIGITS - 2 - (int)ceil(fabs(log10(covar))), covar);
            if (result < 0)
            {
                return result;
            }
            const char *padding;
            if (i == 0)
                padding = "│";
            else
                padding = " ";
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
#pragma GCC diagnostic pop
}

static int print_memory_row(UncertainState *uncertain_state, FILE *stream, int row, const char *stack_indicator)
{
    int i, result;
    int characters_written = 0;
    char buffer[PRINT_DIGIT_BUFFER_SIZE] = {'\0'};

    result = fprintf(stream, " %-*.*s│",
                     PRINT_DIGITS - 1, PRINT_DIGITS - 1, stack_indicator);
    if (result < 0)
    {
        return result;
    }
    else
    {
        characters_written += result;
    }

    for (i = 0; i < UNCERTAIN_REGISTER_SIZE; ++i)
    {
        memset(buffer, ' ', PRINT_DIGITS);
        const char *padding;
        if (i == 0)
            padding = "";
        else
            padding = " ";
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

    for (i = 0; i < UNCERTAIN_MEMORY_SIZE; ++i)
    {
        if (i < row)
        {
            memset(buffer, ' ', PRINT_DIGITS);
        }
        else if (i == row)
        {
            float var = get_uncertain_variance_mem(&uncertain_state->memory, row);
            result = snprintf(buffer, PRINT_DIGITS + 1, "%-*.*g",
                            PRINT_DIGITS, PRINT_DIGITS - 2 - (int)ceil(fabs(log10(var))), var);
            if (result < 0)
            {
                return result;
            }
        }
        else
        {
            float covar = get_uncertain_covariance_mem_mem(&uncertain_state->memory, row, i);
            result = snprintf(buffer, PRINT_DIGITS + 1, "%-*.*g",
                            PRINT_DIGITS, PRINT_DIGITS - 2 - (int)ceil(fabs(log10(covar))), covar);
            if (result < 0)
            {
                return result;
            }
        }
        const char *padding;
        if (i == 0)
            padding = "";
        else
            padding = "  ";
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

static int print_extreme_row(UncertainState *uncertain_state, FILE *stream, const char *label, const char *open, const char *close)
{
    int i, len, result;
    int characters_written = 0;
    char buffer[PRINT_DIGIT_BUFFER_SIZE] = {'\0'};

    result = snprintf(buffer, PRINT_DIGITS + 1, "%-*s",
                      PRINT_DIGITS, label);
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

    for (i = 0, len = PRINT_JUST_REGISTERS ? UNCERTAIN_REGISTER_SIZE : UNCERTAIN_REGISTER_SIZE + UNCERTAIN_MEMORY_SIZE;
         i < len;
         ++i)
    {
        memset(buffer, ' ', PRINT_DIGITS);
        const char *padding;
        if (i == 0)
            padding = "";
        else
            padding = " ";
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

static int print_middle_row(UncertainState *uncertain_state, FILE *stream)
{
    int i, j, result;
    int characters_written = 0;
    char buffer[PRINT_DIGIT_BUFFER_SIZE] = {'\0'};

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

    for (i = 0; i < UNCERTAIN_REGISTER_SIZE + UNCERTAIN_MEMORY_SIZE; ++i)
    {
        const char *padding;
        if (i == 0)
            padding = "";
        else if (i == UNCERTAIN_REGISTER_SIZE)
            padding = "┼"; // " └";
        else
            padding = "─";

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

int uncertain_print_system(UncertainState *uncertain_state, FILE *stream)
{
    int i, result;
    int characters_written = 0;

    result = print_extreme_row(uncertain_state, stream, "REG", "┌", "┐");
    if (result < 0)
    {
        return result;
    }
    else
    {
        characters_written += result;
    }

    for (i = 0; i < UNCERTAIN_REGISTER_SIZE; ++i)
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

        // TODO: read stack_point from CSR
        // int stack_pointer = uncertain_inst_addisp(0);

        for (i = 0; i < UNCERTAIN_MEMORY_SIZE; ++i)
        {
            const char *label = "";
            // if (i == stack_pointer)
            //     label = "^";
            // else if (i > stack_pointer)
            //     label = "|";

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

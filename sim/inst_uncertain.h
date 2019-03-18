#ifndef INSTR_UNCERTAIN_H
#define INSTR_UNCERTAIN_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define UNCERTAIN_REGISTER_SIZE 32

#define COVARIANCES_REG_REG (((UNCERTAIN_REGISTER_SIZE - 1) * UNCERTAIN_REGISTER_SIZE) / 2)

struct Engine;
typedef struct Engine Engine;
struct State;
typedef struct State State;

typedef struct
{
    float *variances;
    float *covariances;
} UncertainMemory;

typedef struct
{
    float variances[UNCERTAIN_REGISTER_SIZE];
    float covariances_reg_reg[COVARIANCES_REG_REG];
    float *covariances_reg_mem;
} UncertainRegisters;

typedef enum {
    UncertainOperationNone,
    UncertainOperationLoadStore,
    UncertainOperationUpdate,
} UncertainOperationType;

typedef struct
{
    uint32_t op_fp_pc;
    uint32_t insn_part1;
    int valid;
} UncertainUpdateInfo;

typedef struct  {
    UncertainUpdateInfo last_op;
    UncertainMemory memory;
    UncertainRegisters registers;
    int memory_size;
} UncertainState;

int uncertain_print_system(UncertainState * state, FILE *stream);
void uncertain_sizemen(Engine *E, State *S, int size);

void uncertain_inst_lr(UncertainState * state, int ud, int location);
void uncertain_inst_sr(UncertainState * state, int us1, int location);

void uncertain_inst_mv(UncertainState * state, int ud, int us1);

void uncertain_inst_up1(UncertainState * state,
                        int ud, int us1,
                        float g1);

void uncertain_inst_up2(UncertainState * state,
                        int ud, int us1, int us2,
                        float g1, float g2);

float uncertain_inst_gv(UncertainState * state, int us1);
float uncertain_inst_gcov(UncertainState * state, int us1, int us2);
void uncertain_inst_sv(UncertainState * state, int ud, float variance);

#endif

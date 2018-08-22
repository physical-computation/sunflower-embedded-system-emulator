#include "instr-riscv.h"
#include "sf.h"

void riscvdecode(Engine *E, uint32_t instr, RiscvPipestage *stage)
{
    volatile instr_riscv_decode *tmp;


	stage->instr = instr;


	/*								*/
	/*	Fill fptr with (void *)nop by default so we dont have	*/
	/*	to check for valid fptrs ever, when using decode cache	*/
	/*	and, e.g., instr is '0'.				*/
	/*								*/
	stage->format = INSTR_N;
    stage->fptr = (void *)superH_nop;

	tmp = (void *)&instr;

    switch(tmp->opcode)
    {
        case 0b0110111:
        {
            stage->fptr = (void *) riscv_lui;
            stage->format = INSTR_U;
            stage->op = RISCV_OP_LUI;

            break;
        }
        case 0b0010111:
        {
            stage->fptr = (void *) riscv_auipc;
            stage->format = INSTR_U;
            stage->op = RISCV_OP_AUIPC;

            break;
        }
        case 0b1101111:
        {
            stage->fptr = (void *) riscv_jal;
            stage->format = INSTR_J;
            stage->op = RISCV_OP_JAL;

            break;
        }
        case 0b1100111:
        {
            stage->fptr = (void *) riscv_jalr;
            stage->format = INSTR_I;
            stage->op = RISCV_OP_JALR;

            break;
        }
        case 0b1100011:
        {
            switch(tmp->funct3)
            {
                case 0b000:
                {
                    stage->fptr = (void *) riscv_beq;
                    stage->format = INSTR_B;
                    stage->op = RISCV_OP_BEQ;

                    break;
                }
                case 0b001:
                {
                    stage->fptr = (void *) riscv_bne;
                    stage->format = INSTR_B;
                    stage->op = RISCV_OP_BNE;

                    break;
                }
                case 0b100:
                {
                    stage->fptr = (void *) riscv_blt;
                    stage->format = INSTR_B;
                    stage->op = RISCV_OP_BLT;

                    break;
                }
                case 0b101:
                {
                    stage->fptr = (void *) riscv_bge;
                    stage->format = INSTR_B;
                    stage->op = RISCV_OP_BGE;

                    break;
                }
                case 0b110:
                {
                    stage->fptr = (void *) riscv_bltu;
                    stage->format = INSTR_B;
                    stage->op = RISCV_OP_BLTU;

                    break;
                }
                case 0b111:
                {
                    stage->fptr = (void *) riscv_bgeu;
                    stage->format = INSTR_B;
                    stage->op = RISCV_OP_BGEU;

                    break;
                }
                default:
                {
                    
                }
            }

            break;
        }
        case 0b0000011:
        {
            switch(tmp->funct3)
            {
                case 0b000:
                {
                    stage->fptr = (void *) riscv_lb;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_LB;

                    break;
                }
                case 0b001:
                {
                    stage->fptr = (void *) riscv_lh;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_LH;

                    break;
                }
                case 0b010:
                {
                    stage->fptr = (void *) riscv_lw;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_LW;

                    break;
                }
                case 0b100:
                {
                    stage->fptr = (void *) riscv_lbu;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_LBU;

                    break;
                }
                case 0b101:
                {
                    stage->fptr = (void *) riscv_lhu;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_LHU;

                    break;
                }
                default:
                {
                    
                }
            }

            break;
        }
        case 0b0100011:
        {
            switch(tmp->funct3)
            {
                case 0b000:
                {
                    stage->fptr = (void *) riscv_sb;
                    stage->format = INSTR_S;
                    stage->op = RISCV_OP_SB;

                    break;
                }
                case 0b001:
                {
                    stage->fptr = (void *) riscv_sh;
                    stage->format = INSTR_S;
                    stage->op = RISCV_OP_SH;

                    break;
                }
                case 0b010:
                {
                    stage->fptr = (void *) riscv_sw;
                    stage->format = INSTR_S;
                    stage->op = RISCV_OP_SW;

                    break;
                }
                default:
                {
                    
                }
            }

            break;
        }
        case 0b0010011:
        {
            switch(tmp->funct3)
            {
                case 0b000:
                {
                    stage->fptr = (void *) riscv_addi;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_ADDI;

                    break;
                }
                case 0b010:
                {
                    stage->fptr = (void *) riscv_slti;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_SLTI;

                    break;
                }
                case 0b011:
                {
                    stage->fptr = (void *) riscv_sltiu;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_SLTIU;

                    break;
                }
                case 0b100:
                {
                    stage->fptr = (void *) riscv_xori;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_XORI;

                    break;
                }
                case 0b110:
                {
                    stage->fptr = (void *) riscv_ori;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_ORI;

                    break;
                }
                case 0b111:
                {
                    stage->fptr = (void *) riscv_andi;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_ANDI;

                    break;
                }
                case 0b001:
                {
                    stage->fptr = (void *) riscv_slli;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_SLLI;

                    break;
                }
                case 0b101:
                {
                    switch(tmp->funct7)
                    {
                        case 0b0000000:
                        {
                            stage->fptr = (void *) riscv_srli;
                            stage->format = INSTR_R;
                            stage->op = RISCV_OP_SRLI;

                            break;
                        }
                        case 0b0100000:
                        {
                            stage->fptr = (void *) riscv_srai;
                            stage->format = INSTR_R;
                            stage->op = RISCV_OP_SRAI;

                            break;
                        }
                        default:
                        {
                            
                        }
                    }

                    break;
                }
                default:
                {
                    
                }
            }

            break;
        }
        case 0b0110011:
        {
            switch(tmp->funct3)
            {
                case 0b000:
                {
                    switch(tmp->funct7)
                    {
                        case 0b0000000:
                        {
                            stage->fptr = (void *) riscv_add;
                            stage->format = INSTR_R;
                            stage->op = RISCV_OP_ADD;

                            break;
                        }
                        case 0b0100000:
                        {
                            stage->fptr = (void *) riscv_sub;
                            stage->format = INSTR_R;
                            stage->op = RISCV_OP_SUB;

                            break;
                        }
                        default:
                        {
                            
                        }
                    }

                    break;
                }
                case 0b001:
                {
                    stage->fptr = (void *) riscv_sll;
                    stage->format = INSTR_R;
                    stage->op = RISCV_OP_SLL;

                    break;
                }
                case 0b010:
                {
                    stage->fptr = (void *) riscv_slt;
                    stage->format = INSTR_R;
                    stage->op = RISCV_OP_SLT;

                    break;
                }
                case 0b011:
                {
                    stage->fptr = (void *) riscv_sltu;
                    stage->format = INSTR_R;
                    stage->op = RISCV_OP_SLTU;

                    break;
                }
                case 0b100:
                {
                    stage->fptr = (void *) riscv_xor;
                    stage->format = INSTR_R;
                    stage->op = RISCV_OP_XOR;

                    break;
                }
                case 0b101:
                {
                    switch(tmp->funct7)
                    {
                        case 0b0000000:
                        {
                            stage->fptr = (void *) riscv_srl;
                            stage->format = INSTR_R;
                            stage->op = RISCV_OP_SRL;

                            break;
                        }
                        case 0b0100000:
                        {
                            stage->fptr = (void *) riscv_sra;
                            stage->format = INSTR_R;
                            stage->op = RISCV_OP_SRA;

                            break;
                        }
                        default:
                        {
                            
                        }
                    }

                    break;
                }
                case 0b110:
                {
                    stage->fptr = (void *) riscv_or;
                    stage->format = INSTR_R;
                    stage->op = RISCV_OP_OR;

                    break;
                }
                case 0b111:
                {
                    stage->fptr = (void *) riscv_and;
                    stage->format = INSTR_R;
                    stage->op = RISCV_OP_AND;

                    break;
                }

                default:
                {
                    
                }
            }

            break;
        }
        case 0b0001111:
        {
            switch (tmp->funct3)
            {
                case 0b000:
                {
                    stage->fptr = (void *) riscv_fence;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_FENCE;

                    break;
                }
                case 0b001:
                {
                    stage->fptr = (void *) riscv_fence_i;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_FENCE_I;

                    break;
                }
                default:
                {
                    
                }
            }

            break;
        }
        case 0b1110011:
        {
            switch (tmp->funct3)
            {
                case 0b000:
                {
                    if (tmp->b20)
                    {
                        stage->fptr = (void *) riscv_ecall;
                        stage->format = INSTR_I;
                        stage->op = RISCV_OP_ECALL;
                    }
                    else
                    {
                        stage->fptr = (void *) riscv_ebreak;
                        stage->format = INSTR_I;
                        stage->op = RISCV_OP_EBREAK;
                    }

                    break;
                }
                case 0b001:
                {
                    stage->fptr = (void *) riscv_csrrw;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_CSRRW;

                    break;
                }
                case 0b010:
                {
                    stage->fptr = (void *) riscv_csrrs;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_CSRRS;

                    break;
                }
                case 0b011:
                {
                    stage->fptr = (void *) riscv_csrrc;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_CSRRC;

                    break;
                }
                case 0b101:
                {
                    stage->fptr = (void *) riscv_csrrwi;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_CSRRWI;

                    break;
                }
                case 0b110:
                {
                    stage->fptr = (void *) riscv_csrrsi;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_CSRRSI;

                    break;
                }
                case 0b111:
                {
                    stage->fptr = (void *) riscv_csrrci;
                    stage->format = INSTR_I;
                    stage->op = RISCV_OP_CSRRCI;

                    break;
                }
                default:
                {
                    
                }
            }

            break;
        }
        default:
        {
            
        }
    }
}
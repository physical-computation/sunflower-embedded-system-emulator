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
	stage->format = INSTR_N;	/*	RISCV has no NOP instruction.				*/
    stage->fptr = (void *)superH_nop;	/*	ADDI x0,x0,x0 requires arguments so no can do...	*/
					/*	superH_nop will suffice for instrutcion simulation,	*/
					/*	but not sure for power analysis.			*/

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
                    break;
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
                    break;
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
                    break;
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
                            stage->format = INSTR_I;
                            stage->op = RISCV_OP_SRLI;

                            break;
                        }
                        case 0b0100000:
                        {
                            stage->fptr = (void *) riscv_srai;
                            stage->format = INSTR_I;
                            stage->op = RISCV_OP_SRAI;

                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }

                    break;
                }
                default:
                {
                    break;
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
                            break;
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
                            break;
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
                    break;
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
                    break;
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
                        stage->fptr = (void *) riscv_ebreak;
                        stage->format = INSTR_I;
                        stage->op = RISCV_OP_EBREAK;
                    }
                    else
                    {
                        stage->fptr = (void *) riscv_ecall;
                        stage->format = INSTR_I;
                        stage->op = RISCV_OP_ECALL;
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
                    break;
                }
            }

            break;
        }


        /*
        *RV32F implementation
        */
        case 0b0000111: //FLW, FLD
        {
        	switch (tmp->funct3)
          {
          	case 0b010: //FLW
          	{
							stage->fptr = (void *) rv32f_flw;
							stage->format = INSTR_I;
							stage->op = RV32F_OP_FLW;

          		break;
          	}

          	case 0b011: //FLD
          	{
          		stage->fptr = (void *) rv32d_fld;
							stage->format = INSTR_I;
							stage->op = RV32D_OP_FLD;

          		break;
          	}

          	default:
          	{
          		break;
          	}
        	}

        	break;
        }

        case 0b0100111: //FSW, FSD
        {
        	switch (tmp->funct3)
          {
          	case 0b010: //FSW
          	{
          		stage->fptr = (void *) rv32f_fsw;
						  stage->format = INSTR_S;
						  stage->op = RV32F_OP_FSW;

          		break;
          	}

          	case 0b011: //FSD
          	{
          		stage->fptr = (void *) rv32d_fsd;
							stage->format = INSTR_S;
							stage->op = RV32D_OP_FSD;

          		break;
          	}

          	default:
          	{
          		break;
          	}
        	}

        	break;
        }

        case 0b1000011: //FMADD.S, FMADD.D
        {
        	if ((tmp->funct7)&1) //FMADD.D
        	{
						stage->fptr = (void *) rv32d_fmadd_d;
						stage->format = INSTR_R4;
						stage->op = RV32D_OP_FMADDD;
        	}
        	else //FMADD.S
        	{
        		stage->fptr = (void *) rv32f_fmadd_s;
				    stage->format = INSTR_R4;
				    stage->op = RV32F_OP_FMADDS;
        	}

        	break;
        }

        case 0b1000111: //FMSUB.S, FMSUB.D
        {
        	if ((tmp->funct7)&1) //FMSUB.D
        	{
						stage->fptr = (void *) rv32d_fmsub_d;
						stage->format = INSTR_R4;
						stage->op = RV32D_OP_FMSUBD;
        	}
        	else //FMSUB.S
        	{
        		stage->fptr = (void *) rv32f_fmsub_s;
				    stage->format = INSTR_R4;
				    stage->op = RV32F_OP_FMSUBS;
        	}

        	break;
        }

        case 0b1001011: //FNMSUB.S, FNMSUB.D
        {
        	if ((tmp->funct7)&1) //FNMSUB.D
        	{
						stage->fptr = (void *) rv32d_fnmsub_d;
						stage->format = INSTR_R4;
						stage->op = RV32D_OP_FNMSUBD;
        	}
        	else //FNMSUB.S
        	{
        		stage->fptr = (void *) rv32f_fnmsub_s;
				    stage->format = INSTR_R4;
				    stage->op = RV32F_OP_FNMSUBS;
        	}

        	break;
        }

        case 0b1001111: //FNMADD.S, //FNMADD.D
        {
        	if ((tmp->funct7)&1) //FNMADD.D
        	{
						stage->fptr = (void *) rv32d_fnmadd_d;
						stage->format = INSTR_R4;
						stage->op = RV32D_OP_FNMADDD;
        	}
        	else //FNMADD.S
        	{
        		stage->fptr = (void *) rv32f_fnmadd_s;
				    stage->format = INSTR_R4;
				    stage->op = RV32F_OP_FNMADDS;
        	}

        	break;
        }

        case 0b1010011:
        {
        	switch(tmp->funct7)
          {
          	case 0b0000000: //FADD.S
          	{
          		stage->fptr = (void *) rv32f_fadd_s;
						  stage->format = INSTR_R;
						  stage->op = RV32F_OP_FADDS;

          		break;
          	}

          	case 0b0000100: //FSUB.S
          	{
          		stage->fptr = (void *) rv32f_fsub_s;
						  stage->format = INSTR_R;
						  stage->op = RV32F_OP_FSUBS;

          		break;
          	}

          	case 0b0001000: //FMUL.S
          	{
          		stage->fptr = (void *) rv32f_fmul_s;
						  stage->format = INSTR_R;
						  stage->op = RV32F_OP_FMULS;

          		break;
          	}

          	case 0b0001100: //FDIV.S
          	{
          		stage->fptr = (void *) rv32f_fdiv_s;
						  stage->format = INSTR_R;
						  stage->op = RV32F_OP_FDIVS;

          		break;
          	}

          	case 0b0101100: //FSQRT.S
          	{
          		stage->fptr = (void *) rv32f_fsqrt_s;
						  stage->format = INSTR_R;
						  stage->op = RV32F_OP_FSQRTS;

          		break;
          	}

          	case 0b0010000: //FSGNJ*
          	{
          		switch (tmp->funct3)
            	{
            		case 0b000: //FSGNJ.S
            		{
            			stage->fptr = (void *) rv32f_fsgnj_s;
									stage->format = INSTR_R;
									stage->op = RV32F_OP_FSGNJS;

            			break;
            		}

            		case 0b001: //FSGNJN.S
            		{
            			stage->fptr = (void *) rv32f_fsgnjn_s;
									stage->format = INSTR_R;
									stage->op = RV32F_OP_FSGNJNS;

            			break;
            		}

            		case 0b010: //FSGNJX.S
            		{
            			stage->fptr = (void *) rv32f_fsgnjx_s;
									stage->format = INSTR_R;
									stage->op = RV32F_OP_FSGNJXS;

            			break;
            		}

            		default:
            		{
            			break;
            		}
            	}

          		break;
          	}

          	case 0b0010100: //FMIN.S, FMAX.S
          	{
          		switch (tmp->funct3)
            	{
            		case 0b000://FMIN.S
            		{
            			stage->fptr = (void *) rv32f_fmin_s;
            			stage->format = INSTR_R;
            			stage->op = RV32F_OP_FMINS;

            			break;
            		}

            		case 0b001://FMAX.S
            		{
            			stage->fptr = (void *) rv32f_fmax_s;
            			stage->format = INSTR_R;
            			stage->op = RV32F_OP_FMAXS;

            			break;
            		}

            		default:
            		{
            			break;
            		}
            	}

          		break;
          	}

          	case 0b1100000: //FCVT.W*
          	{
          		if (tmp->b20) //FCVT.WU.S
            	{
            		stage->fptr = (void *) rv32f_fcvt_wu_s;
            		stage->format = INSTR_R;
								stage->op = RV32F_OP_FCVTWUS;
            	}
            	else //FCVT.W.S
            	{
            		stage->fptr = (void *) rv32f_fcvt_w_s;
            		stage->format = INSTR_R;
								stage->op = RV32F_OP_FCVTWS;
            	}

          		break;
          	}

          	case 0b1110000: //FMV.X.W, FCLASS.S
          	{
          		switch (tmp->funct3)
            	{
            		case 0b000: //FMV.X.W
            		{
            			stage->fptr = (void *) rv32f_fmv_x_w;
            			stage->format = INSTR_R;
            			stage->op = RV32F_OP_FMVXW;

            			break;
            		}

            		case 0b001: //FCLASS.S
            		{
            			stage->fptr = (void *) rv32f_fclass_s;
            			stage->format = INSTR_R;
            			stage->op = RV32F_OP_FCLASSS;

            			break;
            		}

            		default:
            		{
            			break;
            		}
            	}

          		break;
          	}

          	case 0b1010000: //FEQ.S, FLT.S, FLE.S
          	{
          		switch (tmp->funct3)
            	{
            		case 0b010: //FEQ.S
            		{
            			stage->fptr = (void *) rv32f_feq_s;
            			stage->format = INSTR_R;
            			stage->op = RV32F_OP_FEQS;

            			break;
            		}

            		case 0b001: //FLT.S
            		{
            			stage->fptr = (void *) rv32f_flt_s;
            			stage->format = INSTR_R;
            			stage->op = RV32F_OP_FLTS;

            			break;
            		}

            		case 0b000: //FLE.S
            		{
            			stage->fptr = (void *) rv32f_fle_s;
            			stage->format = INSTR_R;
            			stage->op = RV32F_OP_FLES;

            			break;
            		}

            		default:
            		{
            			break;
            		}
            	}

          		break;
          	}

          	case 0b1101000: //FCVT.S*
          	{
          		if (tmp->b20) //FCVT.S.WU
            	{
            		stage->fptr = (void *) rv32f_fcvt_s_wu;
            		stage->format = INSTR_R;
								stage->op = RV32F_OP_FCVTSWU;
            	}
            	else //FCVT.S.W
            	{
            		stage->fptr = (void *) rv32f_fcvt_s_w;
            		stage->format = INSTR_R;
								stage->op = RV32F_OP_FCVTSW;
            	}

          		break;
          	}

          	case 0b1111000: //FMV.W.X
          	{
          		stage->fptr = (void *) rv32f_fmv_w_x;
						  stage->format = INSTR_R;
						  stage->op = RV32F_OP_FMVWX;

          		break;
          	}


          	/* Rest of the RV32D instructions */
          	case 0b0000001: //FADD.D
          	{
							stage->fptr = (void *) rv32d_fadd_d;
							stage->format = INSTR_R;
							stage->op = RV32D_OP_FADDD;

          		break;
          	}

          	case 0b0000101: //FSUB.D
          	{
          		stage->fptr = (void *) rv32d_fsub_d;
							stage->format = INSTR_R;
							stage->op = RV32D_OP_FSUBD;

          		break;
          	}

          	case 0b0001001: //FMUL.D
          	{
          		stage->fptr = (void *) rv32d_fmul_d;
							stage->format = INSTR_R;
							stage->op = RV32D_OP_FMULD;

          		break;
          	}

          	case 0b0001101: //FDIV.D
          	{
          		stage->fptr = (void *) rv32d_fdiv_d;
							stage->format = INSTR_R;
							stage->op = RV32D_OP_FDIVD;

          		break;
          	}

          	case 0b0101101: //FSQRT.D
          	{
          		stage->fptr = (void *) rv32d_fsqrt_d;
							stage->format = INSTR_R;
							stage->op = RV32D_OP_FSQRTD;

          		break;
          	}

          	case 0b0010001: //FSGNJ*.D
          	{
          		switch(tmp->funct3)
          		{
          			case 0b000: //FSGNJ.D
          			{
          				stage->fptr = (void *) rv32d_fsgnj_d;
									stage->format = INSTR_R;
									stage->op = RV32D_OP_FSGNJD;

          				break;
          			}

          			case 0b001: //FSGNJN.D
          			{
          				stage->fptr = (void *) rv32d_fsgnjn_d;
									stage->format = INSTR_R;
									stage->op = RV32D_OP_FSGNJND;

          				break;
          			}

          			case 0b010: //FSGNJX.D
          			{
          				stage->fptr = (void *) rv32d_fsgnjx_d;
									stage->format = INSTR_R;
									stage->op = RV32D_OP_FSGNJXD;

          				break;
          			}

          			default:
          			{
          				break;
          			}
          		}

          		break;
          	}

          	case 0b0010101: //FMIN.D, FMAX.D
          	{
          		switch(tmp->funct3)
          		{
          			case 0b000: //FMIN.D
          			{
          				stage->fptr = (void *) rv32d_fmin_d;
									stage->format = INSTR_R;
									stage->op = RV32D_OP_FMIND;

          				break;
          			}

          			case 0b001: //FMAX.D
          			{
          				stage->fptr = (void *) rv32d_fmax_d;
									stage->format = INSTR_R;
									stage->op = RV32D_OP_FMAXD;

          				break;
          			}

          			default:
          			{
          				break;
          			}
          		}

          		break;
          	}

          	case 0b0100000: //FCVT.S.D
          	{
          		stage->fptr = (void *) rv32d_fcvt_s_d;
							stage->format = INSTR_R;
							stage->op = RV32D_OP_FCVTSD;

          		break;
          	}

          	case 0b0100001: //FCVT.D.S
          	{
          		stage->fptr = (void *) rv32d_fcvt_d_s;
							stage->format = INSTR_R;
							stage->op = RV32D_OP_FCVTDS;

          		break;
          	}

          	case 0b1010001: //FEQ.D, FLT.D, FLE.D
          	{
          		switch(tmp->funct3)
          		{
          			case 0b010: //FEQ.D
          			{
          				stage->fptr = (void *) rv32d_feq_d;
									stage->format = INSTR_R;
									stage->op = RV32D_OP_FEQD;

          				break;
          			}

          			case 0b001: //FLT.D
          			{
          				stage->fptr = (void *) rv32d_flt_d;
									stage->format = INSTR_R;
									stage->op = RV32D_OP_FLTD;

          				break;
          			}

          			case 0b000: //FLE.D
          			{
          				stage->fptr = (void *) rv32d_fle_d;
									stage->format = INSTR_R;
									stage->op = RV32D_OP_FLED;

          				break;
          			}

          			default:
          			{
          				break;
          			}
          		}

          		break;
          	}

          	case 0b1110001: //FCLASS.D
          	{
          		stage->fptr = (void *) rv32d_fclass_d;
							stage->format = INSTR_R;
							stage->op = RV32D_OP_FCLASSD;

          		break;
          	}

          	case 0b1100001: //FCVT.W.D, FCVT.WU.D
          	{
          		if(tmp->b20) //FCVT.WU.D
          		{
          			stage->fptr = (void *) rv32d_fcvt_wu_d;
								stage->format = INSTR_R;
								stage->op = RV32D_OP_FCVTWUD;
          		}
          		else //FCVT.W.D
          		{
          			stage->fptr = (void *) rv32d_fcvt_w_d;
								stage->format = INSTR_R;
								stage->op = RV32D_OP_FCVTWD;
          		}

          		break;
          	}

          	case 0b1101001: //FCVT.D.W, FCVT.D.WU
          	{
          		if(tmp->b20) //FCVT.D.WU
          		{
          			stage->fptr = (void *) rv32f_fcvt_d_wu;
								stage->format = INSTR_R;
								stage->op = RV32D_OP_FCVTDWU;
          		}
          		else //FCVT.D.W
          		{
          			stage->fptr = (void *) rv32d_fcvt_d_w;
								stage->format = INSTR_R;
								stage->op = RV32D_OP_FCVTDW;
          		}

          		break;
          	}

          	default:
          	{
          		break;
          	}
          }

        	break;
        }

        default:
        {
        	break;
        }
    }
}

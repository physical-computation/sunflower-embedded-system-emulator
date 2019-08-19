/*
	Copyright (c) 1999-2008, Phillip Stanley-Marbell (author)
 
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

#include <stdio.h>
#include <stdlib.h>
#include "instr-hitachi-sh.h"
#include "endian-hitachi-sh.h"
#include "sf.h"
extern int	superH_instr_latencies[][5];

void
superHdecode(Engine *E, ushort instr, SuperHPipestage *stage)
{
	/*								*/
	/*	Must declare volatile to guarantee cookie cutting 	*/
	/*	(won't work w/o it, e.g., on SunOS 5.8 w/ gcc 2.95.3)	*/
	/*	Should stop using bitfields anyway :{P			*/
	/*								*/
	volatile decode_instr 	*tmp;


	stage->instr = instr;


	/*								*/
	/*	Fill fptr with (void *)nop by default so we dont have	*/
	/*	to check for valid fptrs ever, when using decode cache	*/
	/*	and, e.g., instr is '0'.				*/
	/*								*/
	stage->format = INSTR_0;
	stage->cycles = 1;
	stage->fptr = (void *)superH_nop;


	tmp = (void *)&instr;

	switch (tmp->nibble_4)
	{
		case B0000:
		{
			switch (tmp->nibble_1)
			{
				case B0010:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcsr;
							stage->op = SUPERH_OP_STCSR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCSR]);
		
							break;						
						}
						case B0001:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcgbr;
							stage->op = SUPERH_OP_STCGBR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCGBR]);

							break;						
						}

						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcvbr;
							stage->op = SUPERH_OP_STCVBR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCVBR]);

							break;						
						}

						case B0011:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcssr;
							stage->op = SUPERH_OP_STCSSR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCSSR]);

							break;						
						}

						case B0100:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcspc;
							stage->op = SUPERH_OP_STCSPC;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCSPC]);

							break;						
						}

						case B1000:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcr_bank;
							stage->op = SUPERH_OP_STCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCRBANK]);

							break;						
						}

						case B1001:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcr_bank;
							stage->op = SUPERH_OP_STCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCRBANK]);

							break;						
						}

						case B1010:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcr_bank;
							stage->op = SUPERH_OP_STCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCRBANK]);

							break;						
						}

						case B1011:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcr_bank;
							stage->op = SUPERH_OP_STCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCRBANK]);

							break;						
						}

						case B1100:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcr_bank;
							stage->op = SUPERH_OP_STCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCRBANK]);

							break;						
						}

						case B1101:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcr_bank;
							stage->op = SUPERH_OP_STCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCRBANK]);

							break;						
						}

						case B1110:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcr_bank;
							stage->op = SUPERH_OP_STCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCRBANK]);

							break;						
						}

						case B1111:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcr_bank;
							stage->op = SUPERH_OP_STCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCRBANK]);

							break;						
						}
					}
					break;
				}
				case B0011:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 2;
							stage->fptr = (void *)superH_bsrf;
							stage->op = SUPERH_OP_BSRF;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_BSRF]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 2;
							stage->fptr = (void *)superH_braf;
							stage->op = SUPERH_OP_BRAF;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_BRAF]);

							break;
						}

						case B1000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_pref;
							stage->op = SUPERH_OP_PREF;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_PREF]);

							break;
						}
					}

					break;
				}
				
				case B0100:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movbs0;
					stage->op = SUPERH_OP_MOVBS0;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBS0]);

					break;
				}
				
				case B0101:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movws0;
					stage->op = SUPERH_OP_MOVWS0;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWS0]);

					break;
				}

				case B0110:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movls0;
					stage->op = SUPERH_OP_MOVLS0;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLS0]);

					break;
				}

				case B0111:
				{
					stage->format = INSTR_NM;
					stage->cycles = 5;
					stage->fptr = (void *)superH_mull;
					stage->op = SUPERH_OP_MULL;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MULL]);

					break;
				}
				case B1000:
				{
					switch (tmp->nibble_2)
					{
						case B0101:
						{
							if (tmp->nibble_3 == B0000)
							{
								stage->format = INSTR_0;
								stage->cycles = 1;
								stage->fptr = (void *)superH_sets;
								stage->op = SUPERH_OP_SETS;
								stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SETS]);
							}

							break;
						}

						case B0001:
						{
							if (tmp->nibble_3 == B0000)
							{
								stage->format = INSTR_0;
								stage->cycles = 1;
								stage->fptr = (void *)superH_sett;
								stage->op = SUPERH_OP_SETT;
								stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SETT]);
							}

							break;
						}
					}

					break;
				}

				case B1001:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							if (tmp->nibble_3 == B0000)
							{
								stage->format = INSTR_0;
								stage->cycles = 1;
								stage->fptr = (void *)superH_nop;
								stage->op = SUPERH_OP_NOP;
								stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_NOP]);
							}

							break;
						}

						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_movt;
							stage->op = SUPERH_OP_MOVT;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVT]);

							break;
						}
					}
					break;
				}

				case B1010:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stsmach;
							stage->op = SUPERH_OP_STSMACH;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STSMACH]);

							break;
						}
						case B0001:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stsmacl;
							stage->op = SUPERH_OP_STSMACL;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STSMACL]);

							break;
						}
						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stspr;
							stage->op = SUPERH_OP_STSPR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STSPR]);

							break;
						}

					}
					break;
				}

		
				case B1011:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							if (tmp->nibble_3 == B0000)
							{
								stage->format = INSTR_0;
								stage->cycles = 2;
								stage->fptr = (void *)superH_rts;
								stage->op = SUPERH_OP_RTS;
								stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_RTS]);
							}

							break;
						}

						case B0001:
						{
							if (tmp->nibble_3 == B0000)
							{
								stage->format = INSTR_0;
								stage->cycles = 4;
								stage->fptr = (void *)superH_sleep;
								stage->op = SUPERH_OP_SLEEP;
								stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SLEEP]);
							}

							break;
						}

						case B0010:
						{
							if (tmp->nibble_3 == B0000)
							{
								stage->format = INSTR_0;
								stage->cycles = 4;
								stage->fptr = (void *)superH_rte;
								stage->op = SUPERH_OP_RTE;
								stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_RTE]);
							}

							break;
						}
					}
					break;
				}
				case B1100:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movbl0;
					stage->op = SUPERH_OP_MOVBL0;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBL0]);

					break;
				}

				case B1101:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movwl0;
					stage->op = SUPERH_OP_MOVWL0;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWL0]);

					break;
				}

				case B1110:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movll0;
					stage->op = SUPERH_OP_MOVLL0;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLL0]);

					break;
				}

				case B1111:
				{
					stage->format = INSTR_NM;
					stage->cycles = 5;
					stage->fptr = (void *)superH_macl;
					stage->op = SUPERH_OP_MACL;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MACL]);

					break;
				}
			}

			switch (tmp->nibble_3)
			{
				case B0000:
				{
					switch (tmp->nibble_1)
					{
						case B1000:
						{
							switch (tmp->nibble_2)
							{
								case B0000:
								{
									stage->format = INSTR_0;
									stage->cycles = 1;
									stage->fptr = (void *)superH_clrt;
									stage->op = SUPERH_OP_CLRT;
									stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CLRT]);

									break;
								}

								case B0010:
								{
									stage->format = INSTR_0;
									stage->cycles = 1;
									stage->fptr = (void *)superH_clrmac;
									stage->op = SUPERH_OP_CLRMAC;
									stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CLRMAC]);

									break;
								}

								case B0011:
								{
									stage->format = INSTR_0;
									stage->cycles = 1;
									stage->fptr = (void *)superH_ldtlb;
									stage->op = SUPERH_OP_LDTLB;
									stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDTLB]);

									break;
								}
								case B0100:
								{
									stage->format = INSTR_0;
									stage->cycles = 1;
									stage->fptr = (void *)superH_clrs;
									stage->op = SUPERH_OP_CLRS;
									stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CLRS]);

									break;
								}
							}

							break;
						}

						case B1001:
						{
							switch (tmp->nibble_2)
							{
								case B0001:
								{
									stage->format = INSTR_0;
									stage->cycles = 1;
									stage->fptr = (void *)superH_div0u;
									stage->op = SUPERH_OP_DIV0U;
									stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_DIV0U]);

									break;
								}
							}

							break;
						}
					}
				}
			}

			break;
		}

		case B0001:
		{
			stage->format = INSTR_NMD;
			stage->cycles = 1;
			stage->fptr = (void *)superH_movls4;
			stage->op = SUPERH_OP_MOVLS4;
			stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLS4]);

			break;
		}

		case B0010:
		{
			switch (tmp->nibble_1)
			{
				case B0000:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movbs;
					stage->op = SUPERH_OP_MOVBS;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBS]);

					break;
				}

				case B0001:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movws;
					stage->op = SUPERH_OP_MOVWS;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWS]);

					break;
				}

				case B0010:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movls;
					stage->op = SUPERH_OP_MOVLS;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLS]);

					break;
				}

				case B0100:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movbm;
					stage->op = SUPERH_OP_MOVBM;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBM]);

					break;
				}

				case B0101:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movwm;
					stage->op = SUPERH_OP_MOVWM;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWM]);

					break;
				}

				case B0110:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movlm;
					stage->op = SUPERH_OP_MOVLM;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLM]);

					break;
				}

				case B0111:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_div0s;
					stage->op = SUPERH_OP_DIV0S;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_DIV0S]);

					break;
				}

				case B1000:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_tst;
					stage->op = SUPERH_OP_TST;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_TST]);

					break;
				}

				case B1001:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_and;
					stage->op = SUPERH_OP_AND;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_AND]);

					break;
				}

				case B1010:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_xor;
					stage->op = SUPERH_OP_XOR;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_XOR]);

					break;
				}

				case B1011:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_or;
					stage->op = SUPERH_OP_OR;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_OR]);

					break;
				}

				case B1100:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_cmpstr;
					stage->op = SUPERH_OP_CMPSTR;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CMPSTR]);

					break;
				}

				case B1101:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_xtrct;
					stage->op = SUPERH_OP_XTRCT;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_XTRCT]);

					break;
				}

				case B1110:
				{
					stage->format = INSTR_NM;
					stage->cycles = 3;
					stage->fptr = (void *)superH_mulu;
					stage->op = SUPERH_OP_MULU;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MULU]);

					break;
				}

				case B1111:
				{
					stage->format = INSTR_NM;
					stage->cycles = 3;
					stage->fptr = (void *)superH_muls;
					stage->op = SUPERH_OP_MULS;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MULS]);

					break;
				}
			}

			break;
		}

		case B0011:
		{
			switch (tmp->nibble_1)
			{
				case B0000:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_cmpeq;
					stage->op = SUPERH_OP_CMPEQ;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CMPEQ]);

					break;
				}

				case B0010:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_cmphs;
					stage->op = SUPERH_OP_CMPHS;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CMPHS]);

					break;
				}

				case B0011:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_cmpge;
					stage->op = SUPERH_OP_CMPGE;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CMPGE]);

					break;
				}

				case B0100:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_div1;
					stage->op = SUPERH_OP_DIV1;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_DIV1]);

					break;
				}

				case B0101:
				{
					stage->format = INSTR_NM;
					stage->cycles = 5;
					stage->fptr = (void *)superH_dmulu;
					stage->op = SUPERH_OP_DMULU;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_DMULU]);

					break;
				}

				case B0110:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_cmphi;
					stage->op = SUPERH_OP_CMPHI;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CMPHI]);

					break;
				}

				case B0111:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_cmpgt;
					stage->op = SUPERH_OP_CMPGT;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CMPGT]);

					break;
				}

				case B1000:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_sub;
					stage->op = SUPERH_OP_SUB;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SUB]);

					break;
				}
				case B1010:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_subc;
					stage->op = SUPERH_OP_SUBC;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SUBC]);

					break;
				}
				case B1011:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_subv;
					stage->op = SUPERH_OP_SUBV;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SUBV]);

					break;
				}

				case B1100:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_add;
					stage->op = SUPERH_OP_ADD;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ADD]);

					break;
				}

				case B1101:
				{
					stage->format = INSTR_NM;
					stage->cycles = 5;
					stage->fptr = (void *)superH_dmuls;
					stage->op = SUPERH_OP_DMULS;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_DMULS]);

					break;
				}

				case B1110:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_addc;
					stage->op = SUPERH_OP_ADDC;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ADDC]);

					break;
				}

				case B1111:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_addv;
					stage->op = SUPERH_OP_ADDV;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ADDV]);

					break;
				}
			}
			break;
		}

		case B0100:
		{
			switch (tmp->nibble_1)
			{
				case B0000:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shll;
							stage->op = SUPERH_OP_SHLL;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHLL]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shal;
							stage->op = SUPERH_OP_SHAL;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHAL]);

							break;
						}
					}
					break;
				}

				case B0001:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shlr;
							stage->op = SUPERH_OP_SHLR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHLR]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shar;
							stage->op = SUPERH_OP_SHAR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHAR]);

							break;
						}
					}
					break;
				}

				case B0010:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stsmmach;
							stage->op = SUPERH_OP_STSMMACH;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STSMMACH]);

							break;
						}

						case B0001:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stsmmacl;
							stage->op = SUPERH_OP_STSMMACL;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STSMMACL]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stsmpr;
							stage->op = SUPERH_OP_STSMPR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STSMPR]);

							break;
						}
					}
					break;
				}

				case B0011:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcmsr;
							stage->op = SUPERH_OP_STCMSR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMSR]);

							break;
						}

						case B0001:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcmgbr;
							stage->op = SUPERH_OP_STCMGBR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMGBR]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcmvbr;
							stage->op = SUPERH_OP_STCMVBR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMVBR]);

							break;
						}

						case B0011:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcmssr;
							stage->op = SUPERH_OP_STCMSSR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMSSR]);

							break;
						}

						case B0100:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_stcmspc;
							stage->op = SUPERH_OP_STCMSPC;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMSPC]);

							break;
						}

						case B1000:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 2;
							stage->fptr = (void *)superH_stcmr_bank;
							stage->op = SUPERH_OP_STCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMRBANK]);

							break;
						}

						case B1001:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 2;
							stage->fptr = (void *)superH_stcmr_bank;
							stage->op = SUPERH_OP_STCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMRBANK]);

							break;
						}

						case B1010:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 2;
							stage->fptr = (void *)superH_stcmr_bank;
							stage->op = SUPERH_OP_STCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMRBANK]);

							break;
						}

						case B1011:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 2;
							stage->fptr = (void *)superH_stcmr_bank;
							stage->op = SUPERH_OP_STCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMRBANK]);

							break;
						}

						case B1100:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 2;
							stage->fptr = (void *)superH_stcmr_bank;
							stage->op = SUPERH_OP_STCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMRBANK]);

							break;
						}

						case B1101:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 2;
							stage->fptr = (void *)superH_stcmr_bank;
							stage->op = SUPERH_OP_STCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMRBANK]);

							break;
						}

						case B1110:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 2;
							stage->fptr = (void *)superH_stcmr_bank;
							stage->op = SUPERH_OP_STCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMRBANK]);

							break;
						}

						case B1111:
						{
							stage->format = INSTR_NBANK;
							stage->cycles = 2;
							stage->fptr = (void *)superH_stcmr_bank;
							stage->op = SUPERH_OP_STCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_STCMRBANK]);

							break;
						}
					}
					break;
				}

				case B0100:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_rotl;
							stage->op = SUPERH_OP_ROTL;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ROTL]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_rotcl;
							stage->op = SUPERH_OP_ROTCL;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ROTCL]);

							break;
						}
					}
					break;
				}

				case B0101:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_rotr;
							stage->op = SUPERH_OP_ROTR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ROTR]);

							break;
						}
			
						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_rotcr;
							stage->op = SUPERH_OP_ROTCR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ROTCR]);

							break;
						}
					}
					break;
				}

				case B0110:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldsmmach;
							stage->op = SUPERH_OP_LDSMMACH;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDSMMACH]);

							break;
						}

						case B0001:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldsmmacl;
							stage->op = SUPERH_OP_LDSMMACL;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDSMMACL]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldsmpr;
							stage->op = SUPERH_OP_LDSMPR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDSMPR]);

							break;
						}

					}

					break;
				}

				case B0111:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_M;
							stage->cycles = 7;
							stage->fptr = (void *)superH_ldcmsr;
							stage->op = SUPERH_OP_LDCMSR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMSR]);

							break;
						}

						case B0001:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmgbr;
							stage->op = SUPERH_OP_LDCMGBR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMGBR]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmvbr;
							stage->op = SUPERH_OP_LDCMVBR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMVBR]);

							break;
						}

						case B0011:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmssr;
							stage->op = SUPERH_OP_LDCMSSR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMSSR]);

							break;
						}

						case B0100:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmspc;
							stage->op = SUPERH_OP_LDCMSPC;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMSPC]);

							break;
						}

						/* Keep It Simple Stupid - LDCMR_BANK */
						case B1000:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmr_bank;
							stage->op = SUPERH_OP_LDCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMRBANK]);

							break;
						}

						case B1001:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmr_bank;
							stage->op = SUPERH_OP_LDCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMRBANK]);

							break;
						}

						case B1010:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmr_bank;
							stage->op = SUPERH_OP_LDCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMRBANK]);

							break;
						}

						case B1011:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmr_bank;
							stage->op = SUPERH_OP_LDCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMRBANK]);

							break;
						}

						case B1100:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmr_bank;
							stage->op = SUPERH_OP_LDCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMRBANK]);

							break;
						}

						case B1101:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmr_bank;
							stage->op = SUPERH_OP_LDCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMRBANK]);

							break;
						}

						case B1110:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmr_bank;
							stage->op = SUPERH_OP_LDCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMRBANK]);

							break;
						}

						case B1111:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcmr_bank;
							stage->op = SUPERH_OP_LDCMRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCMRBANK]);

							break;
						}

					}
					break;
				}

				case B1000:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shll2;
							stage->op = SUPERH_OP_SHLL2;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHLL2]);

							break;
						}
						case B0001:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shll8;
							stage->op = SUPERH_OP_SHLL8;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHLL8]);

							break;
						}
						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shll16;
							stage->op = SUPERH_OP_SHLL16;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHLL16]);

							break;
						}
					}
					break;
				}

				case B1001:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shlr2;
							stage->op = SUPERH_OP_SHLR2;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHLR2]);

							break;
						}
						case B0001:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shlr8;
							stage->op = SUPERH_OP_SHLR8;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHLR8]);

							break;
						}
						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_shlr16;
							stage->op = SUPERH_OP_SHLR16;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHLR16]);

							break;
						}
					}
					break;
				}
				case B1010:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldsmach;
							stage->op = SUPERH_OP_LDSMACH;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDSMACH]);

							break;
						}

						case B0001:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldsmacl;
							stage->op = SUPERH_OP_LDSMACL;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDSMACL]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldspr;
							stage->op = SUPERH_OP_LDSPR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDSPR]);

							break;
						}
					}

					break;
				}

				case B1011:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 2;
							stage->fptr = (void *)superH_jsr;
							stage->op = SUPERH_OP_JSR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_JSR]);

							break;
						}

						case B0001:
						{
							stage->format = INSTR_N;
							stage->cycles = 3;
							stage->fptr = (void *)superH_tas;
							stage->op = SUPERH_OP_TAS;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_TAS]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_N;
							stage->cycles = 2;
							stage->fptr = (void *)superH_jmp;
							stage->op = SUPERH_OP_JMP;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_JMP]);

							break;
						}
					}

					break;
				}

				case B1100:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_shad;
					stage->op = SUPERH_OP_SHAD;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHAD]);

					break;
				}
				
				case B1101:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_shld;
					stage->op = SUPERH_OP_SHLD;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SHLD]);

					break;
				}
						
				case B1110:
				{
					switch (tmp->nibble_2)
					{
						case B0000:
						{
							stage->format = INSTR_M;
							stage->cycles = 5;
							stage->fptr = (void *)superH_ldcsr;
							stage->op = SUPERH_OP_LDCSR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCSR]);

							break;
						}

						case B0001:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcgbr;
							stage->op = SUPERH_OP_LDCGBR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCGBR]);

							break;
						}

						case B0010:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcvbr;
							stage->op = SUPERH_OP_LDCVBR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCVBR]);

							break;
						}

						case B0011:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcssr;
							stage->op = SUPERH_OP_LDCSSR;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCSSR]);

							break;
						}

						case B0100:
						{
							stage->format = INSTR_M;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcspc;
							stage->op = SUPERH_OP_LDCSPC;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCSPC]);

							break;
						}

						/* Keep It Simple Stupid - LDCR_BANK */
						case B1000:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcr_bank;
							stage->op = SUPERH_OP_LDCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCRBANK]);

							break;
						}

						case B1001:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcr_bank;
							stage->op = SUPERH_OP_LDCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCRBANK]);

							break;
						}

						case B1010:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcr_bank;
							stage->op = SUPERH_OP_LDCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCRBANK]);

							break;
						}

						case B1011:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcr_bank;
							stage->op = SUPERH_OP_LDCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCRBANK]);

							break;
						}

						case B1100:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcr_bank;
							stage->op = SUPERH_OP_LDCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCRBANK]);

							break;
						}

						case B1101:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcr_bank;
							stage->op = SUPERH_OP_LDCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCRBANK]);

							break;
						}

						case B1110:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcr_bank;
							stage->op = SUPERH_OP_LDCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCRBANK]);

							break;
						}

						case B1111:
						{
							stage->format = INSTR_MBANK;
							stage->cycles = 1;
							stage->fptr = (void *)superH_ldcr_bank;
							stage->op = SUPERH_OP_LDCRBANK;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_LDCRBANK]);

							break;
						}
					}

					break;
				}

				case B1111:
				{
					stage->format = INSTR_NM;
					stage->cycles = 5;
					stage->fptr = (void *)superH_macw;
					stage->op = SUPERH_OP_MACW;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MACW]);

					break;
				}
			}

			switch (tmp->nibble_2)
			{
				case B0001:
				{
					switch (tmp->nibble_1)
					{
						case B0000:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_dt;
							stage->op = SUPERH_OP_DT;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_DT]);

							break;
						}

						case B0001:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_cmppz;
							stage->op = SUPERH_OP_CMPPZ;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CMPPZ]);

							break;
						}

						case B0101:
						{
							stage->format = INSTR_N;
							stage->cycles = 1;
							stage->fptr = (void *)superH_cmppl;
							stage->op = SUPERH_OP_CMPPL;
							stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CMPPL]);

							break;
						}
					}

					break;
				}
			}
			break;
		}

		case B0101:
		{
			stage->format = INSTR_NMD;
			stage->cycles = 1;
			stage->fptr = (void *)superH_movll4;
			stage->op = SUPERH_OP_MOVLL4;
			stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLL4]);

			break;
		}

		case B0110:
		{
			switch (tmp->nibble_1)
			{
				case B0000:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movbl;
					stage->op = SUPERH_OP_MOVBL;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBL]);

					break;
				}

				case B0001:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movwl;
					stage->op = SUPERH_OP_MOVWL;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWL]);

					break;
				}

				case B0010:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movll;
					stage->op = SUPERH_OP_MOVLL;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLL]);

					break;
				}

				case B0011:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_mov;
					stage->op = SUPERH_OP_MOV;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOV]);

					break;
				}

				case B0100:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movbp;
					stage->op = SUPERH_OP_MOVBP;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBP]);

					break;
				}

				case B0101:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movwp;
					stage->op = SUPERH_OP_MOVWP;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWP]);

					break;
				}

				case B0110:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movlp;
					stage->op = SUPERH_OP_MOVLP;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLP]);

					break;
				}

				case B0111:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_not;
					stage->op = SUPERH_OP_NOT;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_NOT]);

					break;
				}

				case B1000:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_swapb;
					stage->op = SUPERH_OP_SWAPB;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SWAPB]);

					break;
				}

				case B1001:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_swapw;
					stage->op = SUPERH_OP_SWAPW;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_SWAPW]);

					break;
				}

				case B1010:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_negc;
					stage->op = SUPERH_OP_NEGC;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_NEGC]);

					break;
				}

				case B1011:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_neg;
					stage->op = SUPERH_OP_NEG;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_NEG]);

					break;
				}

				case B1100:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_extub;
					stage->op = SUPERH_OP_EXTUB;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_EXTUB]);

					break;
				}

				case B1101:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_extuw;
					stage->op = SUPERH_OP_EXTUW;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_EXTUW]);

					break;
				}

				case B1110:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_extsb;
					stage->op = SUPERH_OP_EXTSB;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_EXTSB]);

					break;
				}

				case B1111:
				{
					stage->format = INSTR_NM;
					stage->cycles = 1;
					stage->fptr = (void *)superH_extsw;
					stage->op = SUPERH_OP_EXTSW;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_EXTSW]);

					break;
				}
			}

			break;
		}

		case B0111:
		{
			stage->format = INSTR_NI;
			stage->cycles = 1;
			stage->fptr = (void *)superH_addi;
			stage->op = SUPERH_OP_ADDI;
			stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ADDI]);

			break;
		}

		case B1000:
		{
			switch (tmp->nibble_3)
			{
				case B0000:
				{
					stage->format = INSTR_ND4;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movbs4;
					stage->op = SUPERH_OP_MOVBS4;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBS4]);

					break;
				}

				case B0001:
				{
					stage->format = INSTR_ND4;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movws4;
					stage->op = SUPERH_OP_MOVWS4;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWS4]);

					break;
				}

				case B0100:
				{
					stage->format = INSTR_MD;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movbl4;
					stage->op = SUPERH_OP_MOVBL4;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBL4]);

					break;
				}

				case B0101:
				{
					stage->format = INSTR_MD;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movwl4;
					stage->op = SUPERH_OP_MOVWL4;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWL4]);

					break;
				}

				case B1000:
				{
					stage->format = INSTR_I;
					stage->cycles = 1;
					stage->fptr = (void *)superH_cmpim;
					stage->op = SUPERH_OP_CMPIM;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_CMPIM]);

					break;
				}

				case B1001:
				{
					stage->format = INSTR_D8;
					stage->cycles = 3;
					stage->fptr = (void *)superH_bt;
					stage->op = SUPERH_OP_BT;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_BT]);

					break;
				}

				case B1011:
				{
					stage->format = INSTR_D8;
					stage->cycles = 2;
					stage->fptr = (void *)superH_bf;
					stage->op = SUPERH_OP_BF;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_BF]);

					break;
				}

				case B1101:
				{
					stage->format = INSTR_D8;
					stage->cycles = 2;
					stage->fptr = (void *)superH_bts;
					stage->op = SUPERH_OP_BTS;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_BTS]);

					break;
				}

				case B1111:
				{
					stage->format = INSTR_D8;
					stage->cycles = 3;
					stage->fptr = (void *)superH_bfs;
					stage->op = SUPERH_OP_BFS;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_BFS]);

					break;
				}
			}
			break;
		}

		case B1001:
		{
			stage->format = INSTR_ND8;
			stage->cycles = 2;
			stage->fptr = (void *)superH_movwi;
			stage->op = SUPERH_OP_MOVWI;
			stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWI]);

			break;
		}

		case B1010:
		{
			stage->format = INSTR_D12;
			stage->cycles = 2;
			stage->fptr = (void *)superH_bra;
			stage->op = SUPERH_OP_BRA;
			stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_BRA]);

			break;
		}

		case B1011:
		{
			stage->format = INSTR_D12;
			stage->cycles = 2;
			stage->fptr = (void *)superH_bsr;
			stage->op = SUPERH_OP_BSR;
			stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_BSR]);

			break;
		}

		case B1100:
		{
			switch (tmp->nibble_3)
			{
				case B0000:
				{
					stage->format = INSTR_D8;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movbsg;
					stage->op = SUPERH_OP_MOVBSG;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBSG]);

					break;
				}

				case B0001:
				{
					stage->format = INSTR_D8;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movwsg;
					stage->op = SUPERH_OP_MOVWSG;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWSG]);

					break;
				}

				case B0010:
				{
					stage->format = INSTR_D8;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movlsg;
					stage->op = SUPERH_OP_MOVLSG;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLSG]);

					break;
				}

				case B0011:
				{
					stage->format = INSTR_I;
					stage->cycles = 6;
					stage->fptr = (void *)superH_trapa;
					stage->op = SUPERH_OP_TRAPA;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_TRAPA]);

					break;
				}

				case B0100:
				{
					stage->format = INSTR_D8;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movblg;
					stage->op = SUPERH_OP_MOVBLG;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVBLG]);

					break;
				}

				case B0101:
				{
					stage->format = INSTR_D8;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movwlg;
					stage->op = SUPERH_OP_MOVWLG;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVWLG]);

					break;
				}

				case B0110:
				{
					stage->format = INSTR_D8;
					stage->cycles = 1;
					stage->fptr = (void *)superH_movllg;
					stage->op = SUPERH_OP_MOVLLG;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLLG]);

					break;
				}

				case B0111:
				{
					stage->format = INSTR_D8;
					stage->cycles = 1;
					stage->fptr = (void *)superH_mova;
					stage->op = SUPERH_OP_MOVA;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVA]);

					break;
				}

				case B1000:
				{
					stage->format = INSTR_I;
					stage->cycles = 1;
					stage->fptr = (void *)superH_tsti;
					stage->op = SUPERH_OP_TSTI;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_TSTI]);

					break;
				}

				case B1001:
				{
					stage->format = INSTR_I;
					stage->cycles = 1;
					stage->fptr = (void *)superH_andi;
					stage->op = SUPERH_OP_ANDI;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ANDI]);

					break;
				}

				case B1010:
				{
					stage->format = INSTR_I;
					stage->cycles = 1;
					stage->fptr = (void *)superH_xori;
					stage->op = SUPERH_OP_XORI;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_XORI]);

					break;
				}

				case B1011:
				{
					stage->format = INSTR_I;
					stage->cycles = 1;
					stage->fptr = (void *)superH_ori;
					stage->op = SUPERH_OP_ORI;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ORI]);

					break;
				}

				case B1100:
				{
					stage->format = INSTR_I;
					stage->cycles = 3;
					stage->fptr = (void *)superH_tstm;
					stage->op = SUPERH_OP_TSTM;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_TSTM]);

					break;
				}

				case B1101:
				{
					stage->format = INSTR_I;
					stage->cycles = 3;
					stage->fptr = (void *)superH_andm;
					stage->op = SUPERH_OP_ANDM;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ANDM]);

					break;
				}

				case B1110:
				{
					stage->format = INSTR_I;
					stage->cycles = 3;
					stage->fptr = (void *)superH_xorm;
					stage->op = SUPERH_OP_XORM;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_XORM]);

					break;
				}

				case B1111:
				{
					stage->format = INSTR_I;
					stage->cycles = 3;
					stage->fptr = (void *)superH_orm;
					stage->op = SUPERH_OP_ORM;
					stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_ORM]);

					break;
				}
			}

			break;
		}

		case B1101:
		{
			stage->format = INSTR_ND8;
			stage->cycles = 1;
			stage->fptr = (void *)superH_movli;
			stage->op = SUPERH_OP_MOVLI;
			stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVLI]);

			break;
		}

		case B1110:
		{
			stage->format = INSTR_NI;
			stage->cycles = 1;
			stage->fptr = (void *)superH_movi;
			stage->op = SUPERH_OP_MOVI;
			stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_MOVI]);

			break;
		}

		case B1111:
		{
			stage->format = INSTR_I;
			stage->cycles = 1;
			stage->fptr = (void *)superH_rfg;
			stage->op = SUPERH_OP_RFG;
			stage->instr_latencies = (int *)(&superH_instr_latencies[SUPERH_OP_RFG]);

			break;
		}

		default:
		{
			mprint(E, NULL, siminfo, "\nUnknown instruction seen in superHdecode()!\n\n");
			mexit(E, "See above messages", -1);

			break;
		}
	}


	return;
}

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

void superHdecode(Engine *E, ushort instr, SuperHPipestage *stage) {
  /*								*/
  /*	Must declare volatile to guarantee cookie cutting 	*/
  /*	(won't work w/o it, e.g., on SunOS 5.8 w/ gcc 2.95.3)	*/
  /*	Should stop using bitfields anyway :{P			*/
  /*								*/
  volatile decode_instr *tmp;

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

  switch (tmp->nibble_4) {
  case B0000: {
    switch (tmp->nibble_1) {
    case B0010: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcsr;
        stage->op = OP_STCSR;

        break;
      }
      case B0001: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcgbr;
        stage->op = OP_STCGBR;

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcvbr;
        stage->op = OP_STCVBR;

        break;
      }

      case B0011: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcssr;
        stage->op = OP_STCSSR;

        break;
      }

      case B0100: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcspc;
        stage->op = OP_STCSPC;

        break;
      }

      case B1000: {
        stage->format = INSTR_NBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcr_bank;
        stage->op = OP_STCRBANK;

        break;
      }

      case B1001: {
        stage->format = INSTR_NBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcr_bank;
        stage->op = OP_STCRBANK;

        break;
      }

      case B1010: {
        stage->format = INSTR_NBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcr_bank;
        stage->op = OP_STCRBANK;

        break;
      }

      case B1011: {
        stage->format = INSTR_NBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcr_bank;
        stage->op = OP_STCRBANK;

        break;
      }

      case B1100: {
        stage->format = INSTR_NBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcr_bank;
        stage->op = OP_STCRBANK;

        break;
      }

      case B1101: {
        stage->format = INSTR_NBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcr_bank;
        stage->op = OP_STCRBANK;

        break;
      }

      case B1110: {
        stage->format = INSTR_NBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcr_bank;
        stage->op = OP_STCRBANK;

        break;
      }

      case B1111: {
        stage->format = INSTR_NBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcr_bank;
        stage->op = OP_STCRBANK;

        break;
      }
      }
      break;
    }
    case B0011: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 2;
        stage->fptr = (void *)superH_bsrf;
        stage->op = OP_BSRF;

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 2;
        stage->fptr = (void *)superH_braf;
        stage->op = OP_BRAF;

        break;
      }

      case B1000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_pref;
        stage->op = OP_PREF;

        break;
      }
      }

      break;
    }

    case B0100: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movbs0;
      stage->op = OP_MOVBS0;

      break;
    }

    case B0101: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movws0;
      stage->op = OP_MOVWS0;

      break;
    }

    case B0110: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movls0;
      stage->op = OP_MOVLS0;

      break;
    }

    case B0111: {
      stage->format = INSTR_NM;
      stage->cycles = 5;
      stage->fptr = (void *)superH_mull;
      stage->op = OP_MULL;

      break;
    }
    case B1000: {
      switch (tmp->nibble_2) {
      case B0101: {
        if (tmp->nibble_3 == B0000) {
          stage->format = INSTR_0;
          stage->cycles = 1;
          stage->fptr = (void *)superH_sets;
          stage->op = OP_SETS;
        }

        break;
      }

      case B0001: {
        if (tmp->nibble_3 == B0000) {
          stage->format = INSTR_0;
          stage->cycles = 1;
          stage->fptr = (void *)superH_sett;
          stage->op = OP_SETT;
        }

        break;
      }
      }

      break;
    }

    case B1001: {
      switch (tmp->nibble_2) {
      case B0000: {
        if (tmp->nibble_3 == B0000) {
          stage->format = INSTR_0;
          stage->cycles = 1;
          stage->fptr = (void *)superH_nop;
          stage->op = OP_NOP;
        }

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_movt;
        stage->op = OP_MOVT;

        break;
      }
      }
      break;
    }

    case B1010: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stsmach;
        stage->op = OP_STSMACH;

        break;
      }
      case B0001: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stsmacl;
        stage->op = OP_STSMACL;

        break;
      }
      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stspr;
        stage->op = OP_STSPR;

        break;
      }
      }
      break;
    }

    case B1011: {
      switch (tmp->nibble_2) {
      case B0000: {
        if (tmp->nibble_3 == B0000) {
          stage->format = INSTR_0;
          stage->cycles = 2;
          stage->fptr = (void *)superH_rts;
          stage->op = OP_RTS;
        }

        break;
      }

      case B0001: {
        if (tmp->nibble_3 == B0000) {
          stage->format = INSTR_0;
          stage->cycles = 4;
          stage->fptr = (void *)superH_sleep;
          stage->op = OP_SLEEP;
        }

        break;
      }

      case B0010: {
        if (tmp->nibble_3 == B0000) {
          stage->format = INSTR_0;
          stage->cycles = 4;
          stage->fptr = (void *)superH_rte;
          stage->op = OP_RTE;
        }

        break;
      }
      }
      break;
    }
    case B1100: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movbl0;
      stage->op = OP_MOVBL0;

      break;
    }

    case B1101: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movwl0;
      stage->op = OP_MOVWL0;

      break;
    }

    case B1110: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movll0;
      stage->op = OP_MOVLL0;

      break;
    }

    case B1111: {
      stage->format = INSTR_NM;
      stage->cycles = 5;
      stage->fptr = (void *)superH_macl;
      stage->op = OP_MACL;

      break;
    }
    }

    switch (tmp->nibble_3) {
    case B0000: {
      switch (tmp->nibble_1) {
      case B1000: {
        switch (tmp->nibble_2) {
        case B0000: {
          stage->format = INSTR_0;
          stage->cycles = 1;
          stage->fptr = (void *)superH_clrt;
          stage->op = OP_CLRT;

          break;
        }

        case B0010: {
          stage->format = INSTR_0;
          stage->cycles = 1;
          stage->fptr = (void *)superH_clrmac;
          stage->op = OP_CLRMAC;

          break;
        }

        case B0011: {
          stage->format = INSTR_0;
          stage->cycles = 1;
          stage->fptr = (void *)superH_ldtlb;
          stage->op = OP_LDTLB;

          break;
        }
        case B0100: {
          stage->format = INSTR_0;
          stage->cycles = 1;
          stage->fptr = (void *)superH_clrs;
          stage->op = OP_CLRS;

          break;
        }
        }

        break;
      }

      case B1001: {
        switch (tmp->nibble_2) {
        case B0001: {
          stage->format = INSTR_0;
          stage->cycles = 1;
          stage->fptr = (void *)superH_div0u;
          stage->op = OP_DIV0U;

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

  case B0001: {
    stage->format = INSTR_NMD;
    stage->cycles = 1;
    stage->fptr = (void *)superH_movls4;
    stage->op = OP_MOVLS4;

    break;
  }

  case B0010: {
    switch (tmp->nibble_1) {
    case B0000: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movbs;
      stage->op = OP_MOVBS;

      break;
    }

    case B0001: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movws;
      stage->op = OP_MOVWS;

      break;
    }

    case B0010: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movls;
      stage->op = OP_MOVLS;

      break;
    }

    case B0100: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movbm;
      stage->op = OP_MOVBM;

      break;
    }

    case B0101: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movwm;
      stage->op = OP_MOVWM;

      break;
    }

    case B0110: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movlm;
      stage->op = OP_MOVLM;

      break;
    }

    case B0111: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_div0s;
      stage->op = OP_DIV0S;

      break;
    }

    case B1000: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_tst;
      stage->op = OP_TST;

      break;
    }

    case B1001: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_and;
      stage->op = OP_AND;

      break;
    }

    case B1010: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_xor;
      stage->op = OP_XOR;

      break;
    }

    case B1011: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_or;
      stage->op = OP_OR;

      break;
    }

    case B1100: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_cmpstr;
      stage->op = OP_CMPSTR;

      break;
    }

    case B1101: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_xtrct;
      stage->op = OP_XTRCT;

      break;
    }

    case B1110: {
      stage->format = INSTR_NM;
      stage->cycles = 3;
      stage->fptr = (void *)superH_mulu;
      stage->op = OP_MULU;

      break;
    }

    case B1111: {
      stage->format = INSTR_NM;
      stage->cycles = 3;
      stage->fptr = (void *)superH_muls;
      stage->op = OP_MULS;

      break;
    }
    }

    break;
  }

  case B0011: {
    switch (tmp->nibble_1) {
    case B0000: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_cmpeq;
      stage->op = OP_CMPEQ;

      break;
    }

    case B0010: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_cmphs;
      stage->op = OP_CMPHS;

      break;
    }

    case B0011: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_cmpge;
      stage->op = OP_CMPGE;

      break;
    }

    case B0100: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_div1;
      stage->op = OP_DIV1;

      break;
    }

    case B0101: {
      stage->format = INSTR_NM;
      stage->cycles = 5;
      stage->fptr = (void *)superH_dmulu;
      stage->op = OP_DMULU;

      break;
    }

    case B0110: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_cmphi;
      stage->op = OP_CMPHI;

      break;
    }

    case B0111: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_cmpgt;
      stage->op = OP_CMPGT;

      break;
    }

    case B1000: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_sub;
      stage->op = OP_SUB;

      break;
    }
    case B1010: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_subc;
      stage->op = OP_SUBC;

      break;
    }
    case B1011: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_subv;
      stage->op = OP_SUBV;

      break;
    }

    case B1100: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_add;
      stage->op = OP_ADD;

      break;
    }

    case B1101: {
      stage->format = INSTR_NM;
      stage->cycles = 5;
      stage->fptr = (void *)superH_dmuls;
      stage->op = OP_DMULS;

      break;
    }

    case B1110: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_addc;
      stage->op = OP_ADDC;

      break;
    }

    case B1111: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_addv;
      stage->op = OP_ADDV;

      break;
    }
    }
    break;
  }

  case B0100: {
    switch (tmp->nibble_1) {
    case B0000: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shll;
        stage->op = OP_SHLL;

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shal;
        stage->op = OP_SHAL;

        break;
      }
      }
      break;
    }

    case B0001: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shlr;
        stage->op = OP_SHLR;

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shar;
        stage->op = OP_SHAR;

        break;
      }
      }
      break;
    }

    case B0010: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stsmmach;
        stage->op = OP_STSMMACH;

        break;
      }

      case B0001: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stsmmacl;
        stage->op = OP_STSMMACL;

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stsmpr;
        stage->op = OP_STSMPR;

        break;
      }
      }
      break;
    }

    case B0011: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcmsr;
        stage->op = OP_STCMSR;

        break;
      }

      case B0001: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcmgbr;
        stage->op = OP_STCMGBR;

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcmvbr;
        stage->op = OP_STCMVBR;

        break;
      }

      case B0011: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcmssr;
        stage->op = OP_STCMSSR;

        break;
      }

      case B0100: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_stcmspc;
        stage->op = OP_STCMSPC;

        break;
      }

      case B1000: {
        stage->format = INSTR_NBANK;
        stage->cycles = 2;
        stage->fptr = (void *)superH_stcmr_bank;
        stage->op = OP_STCMRBANK;

        break;
      }

      case B1001: {
        stage->format = INSTR_NBANK;
        stage->cycles = 2;
        stage->fptr = (void *)superH_stcmr_bank;
        stage->op = OP_STCMRBANK;

        break;
      }

      case B1010: {
        stage->format = INSTR_NBANK;
        stage->cycles = 2;
        stage->fptr = (void *)superH_stcmr_bank;
        stage->op = OP_STCMRBANK;

        break;
      }

      case B1011: {
        stage->format = INSTR_NBANK;
        stage->cycles = 2;
        stage->fptr = (void *)superH_stcmr_bank;
        stage->op = OP_STCMRBANK;

        break;
      }

      case B1100: {
        stage->format = INSTR_NBANK;
        stage->cycles = 2;
        stage->fptr = (void *)superH_stcmr_bank;
        stage->op = OP_STCMRBANK;

        break;
      }

      case B1101: {
        stage->format = INSTR_NBANK;
        stage->cycles = 2;
        stage->fptr = (void *)superH_stcmr_bank;
        stage->op = OP_STCMRBANK;

        break;
      }

      case B1110: {
        stage->format = INSTR_NBANK;
        stage->cycles = 2;
        stage->fptr = (void *)superH_stcmr_bank;
        stage->op = OP_STCMRBANK;

        break;
      }

      case B1111: {
        stage->format = INSTR_NBANK;
        stage->cycles = 2;
        stage->fptr = (void *)superH_stcmr_bank;
        stage->op = OP_STCMRBANK;

        break;
      }
      }
      break;
    }

    case B0100: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_rotl;
        stage->op = OP_ROTL;

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_rotcl;
        stage->op = OP_ROTCL;

        break;
      }
      }
      break;
    }

    case B0101: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_rotr;
        stage->op = OP_ROTR;

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_rotcr;
        stage->op = OP_ROTCR;

        break;
      }
      }
      break;
    }

    case B0110: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldsmmach;
        stage->op = OP_LDSMMACH;

        break;
      }

      case B0001: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldsmmacl;
        stage->op = OP_LDSMMACL;

        break;
      }

      case B0010: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldsmpr;
        stage->op = OP_LDSMPR;

        break;
      }
      }

      break;
    }

    case B0111: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_M;
        stage->cycles = 7;
        stage->fptr = (void *)superH_ldcmsr;
        stage->op = OP_LDCMSR;

        break;
      }

      case B0001: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmgbr;
        stage->op = OP_LDCMGBR;

        break;
      }

      case B0010: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmvbr;
        stage->op = OP_LDCMVBR;

        break;
      }

      case B0011: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmssr;
        stage->op = OP_LDCMSSR;

        break;
      }

      case B0100: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmspc;
        stage->op = OP_LDCMSPC;

        break;
      }

      /* Keep It Simple Stupid - LDCMR_BANK */
      case B1000: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmr_bank;
        stage->op = OP_LDCMRBANK;

        break;
      }

      case B1001: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmr_bank;
        stage->op = OP_LDCMRBANK;

        break;
      }

      case B1010: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmr_bank;
        stage->op = OP_LDCMRBANK;

        break;
      }

      case B1011: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmr_bank;
        stage->op = OP_LDCMRBANK;

        break;
      }

      case B1100: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmr_bank;
        stage->op = OP_LDCMRBANK;

        break;
      }

      case B1101: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmr_bank;
        stage->op = OP_LDCMRBANK;

        break;
      }

      case B1110: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmr_bank;
        stage->op = OP_LDCMRBANK;

        break;
      }

      case B1111: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcmr_bank;
        stage->op = OP_LDCMRBANK;

        break;
      }
      }
      break;
    }

    case B1000: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shll2;
        stage->op = OP_SHLL2;

        break;
      }
      case B0001: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shll8;
        stage->op = OP_SHLL8;

        break;
      }
      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shll16;
        stage->op = OP_SHLL16;

        break;
      }
      }
      break;
    }

    case B1001: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shlr2;
        stage->op = OP_SHLR2;

        break;
      }
      case B0001: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shlr8;
        stage->op = OP_SHLR8;

        break;
      }
      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_shlr16;
        stage->op = OP_SHLR16;

        break;
      }
      }
      break;
    }
    case B1010: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldsmach;
        stage->op = OP_LDSMACH;

        break;
      }

      case B0001: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldsmacl;
        stage->op = OP_LDSMACL;

        break;
      }

      case B0010: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldspr;
        stage->op = OP_LDSPR;

        break;
      }
      }

      break;
    }

    case B1011: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 2;
        stage->fptr = (void *)superH_jsr;
        stage->op = OP_JSR;

        break;
      }

      case B0001: {
        stage->format = INSTR_N;
        stage->cycles = 3;
        stage->fptr = (void *)superH_tas;
        stage->op = OP_TAS;

        break;
      }

      case B0010: {
        stage->format = INSTR_N;
        stage->cycles = 2;
        stage->fptr = (void *)superH_jmp;
        stage->op = OP_JMP;

        break;
      }
      }

      break;
    }

    case B1100: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_shad;
      stage->op = OP_SHAD;

      break;
    }

    case B1101: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_shld;
      stage->op = OP_SHLD;

      break;
    }

    case B1110: {
      switch (tmp->nibble_2) {
      case B0000: {
        stage->format = INSTR_M;
        stage->cycles = 5;
        stage->fptr = (void *)superH_ldcsr;
        stage->op = OP_LDCSR;

        break;
      }

      case B0001: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcgbr;
        stage->op = OP_LDCGBR;

        break;
      }

      case B0010: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcvbr;
        stage->op = OP_LDCVBR;

        break;
      }

      case B0011: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcssr;
        stage->op = OP_LDCSSR;

        break;
      }

      case B0100: {
        stage->format = INSTR_M;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcspc;
        stage->op = OP_LDCSPC;

        break;
      }

      /* Keep It Simple Stupid - LDCR_BANK */
      case B1000: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcr_bank;
        stage->op = OP_LDCRBANK;

        break;
      }

      case B1001: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcr_bank;
        stage->op = OP_LDCRBANK;

        break;
      }

      case B1010: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcr_bank;
        stage->op = OP_LDCRBANK;

        break;
      }

      case B1011: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcr_bank;
        stage->op = OP_LDCRBANK;

        break;
      }

      case B1100: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcr_bank;
        stage->op = OP_LDCRBANK;

        break;
      }

      case B1101: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcr_bank;
        stage->op = OP_LDCRBANK;

        break;
      }

      case B1110: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcr_bank;
        stage->op = OP_LDCRBANK;

        break;
      }

      case B1111: {
        stage->format = INSTR_MBANK;
        stage->cycles = 1;
        stage->fptr = (void *)superH_ldcr_bank;
        stage->op = OP_LDCRBANK;

        break;
      }
      }

      break;
    }

    case B1111: {
      stage->format = INSTR_NM;
      stage->cycles = 5;
      stage->fptr = (void *)superH_macw;
      stage->op = OP_MACW;

      break;
    }
    }

    switch (tmp->nibble_2) {
    case B0001: {
      switch (tmp->nibble_1) {
      case B0000: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_dt;
        stage->op = OP_DT;

        break;
      }

      case B0001: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_cmppz;
        stage->op = OP_CMPPZ;

        break;
      }

      case B0101: {
        stage->format = INSTR_N;
        stage->cycles = 1;
        stage->fptr = (void *)superH_cmppl;
        stage->op = OP_CMPPL;

        break;
      }
      }

      break;
    }
    }
    break;
  }

  case B0101: {
    stage->format = INSTR_NMD;
    stage->cycles = 1;
    stage->fptr = (void *)superH_movll4;
    stage->op = OP_MOVLL4;

    break;
  }

  case B0110: {
    switch (tmp->nibble_1) {
    case B0000: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movbl;
      stage->op = OP_MOVBL;

      break;
    }

    case B0001: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movwl;
      stage->op = OP_MOVWL;

      break;
    }

    case B0010: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movll;
      stage->op = OP_MOVLL;

      break;
    }

    case B0011: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_mov;
      stage->op = OP_MOV;

      break;
    }

    case B0100: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movbp;
      stage->op = OP_MOVBP;

      break;
    }

    case B0101: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movwp;
      stage->op = OP_MOVWP;

      break;
    }

    case B0110: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movlp;
      stage->op = OP_MOVLP;

      break;
    }

    case B0111: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_not;
      stage->op = OP_NOT;

      break;
    }

    case B1000: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_swapb;
      stage->op = OP_SWAPB;

      break;
    }

    case B1001: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_swapw;
      stage->op = OP_SWAPW;

      break;
    }

    case B1010: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_negc;
      stage->op = OP_NEGC;

      break;
    }

    case B1011: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_neg;
      stage->op = OP_NEG;

      break;
    }

    case B1100: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_extub;
      stage->op = OP_EXTUB;

      break;
    }

    case B1101: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_extuw;
      stage->op = OP_EXTUW;

      break;
    }

    case B1110: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_extsb;
      stage->op = OP_EXTSB;

      break;
    }

    case B1111: {
      stage->format = INSTR_NM;
      stage->cycles = 1;
      stage->fptr = (void *)superH_extsw;
      stage->op = OP_EXTSW;

      break;
    }
    }

    break;
  }

  case B0111: {
    stage->format = INSTR_NI;
    stage->cycles = 1;
    stage->fptr = (void *)superH_addi;
    stage->op = OP_ADDI;

    break;
  }

  case B1000: {
    switch (tmp->nibble_3) {
    case B0000: {
      stage->format = INSTR_ND4;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movbs4;
      stage->op = OP_MOVBS4;

      break;
    }

    case B0001: {
      stage->format = INSTR_ND4;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movws4;
      stage->op = OP_MOVWS4;

      break;
    }

    case B0100: {
      stage->format = INSTR_MD;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movbl4;
      stage->op = OP_MOVBL4;

      break;
    }

    case B0101: {
      stage->format = INSTR_MD;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movwl4;
      stage->op = OP_MOVWL4;

      break;
    }

    case B1000: {
      stage->format = INSTR_I;
      stage->cycles = 1;
      stage->fptr = (void *)superH_cmpim;
      stage->op = OP_CMPIM;

      break;
    }

    case B1001: {
      stage->format = INSTR_D8;
      stage->cycles = 3;
      stage->fptr = (void *)superH_bt;
      stage->op = OP_BT;

      break;
    }

    case B1011: {
      stage->format = INSTR_D8;
      stage->cycles = 2;
      stage->fptr = (void *)superH_bf;
      stage->op = OP_BF;

      break;
    }

    case B1101: {
      stage->format = INSTR_D8;
      stage->cycles = 2;
      stage->fptr = (void *)superH_bts;
      stage->op = OP_BTS;

      break;
    }

    case B1111: {
      stage->format = INSTR_D8;
      stage->cycles = 3;
      stage->fptr = (void *)superH_bfs;
      stage->op = OP_BFS;

      break;
    }
    }
    break;
  }

  case B1001: {
    stage->format = INSTR_ND8;
    stage->cycles = 2;
    stage->fptr = (void *)superH_movwi;
    stage->op = OP_MOVWI;

    break;
  }

  case B1010: {
    stage->format = INSTR_D12;
    stage->cycles = 2;
    stage->fptr = (void *)superH_bra;
    stage->op = OP_BRA;

    break;
  }

  case B1011: {
    stage->format = INSTR_D12;
    stage->cycles = 2;
    stage->fptr = (void *)superH_bsr;
    stage->op = OP_BSR;

    break;
  }

  case B1100: {
    switch (tmp->nibble_3) {
    case B0000: {
      stage->format = INSTR_D8;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movbsg;
      stage->op = OP_MOVBSG;

      break;
    }

    case B0001: {
      stage->format = INSTR_D8;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movwsg;
      stage->op = OP_MOVWSG;

      break;
    }

    case B0010: {
      stage->format = INSTR_D8;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movlsg;
      stage->op = OP_MOVLSG;

      break;
    }

    case B0011: {
      stage->format = INSTR_I;
      stage->cycles = 6;
      stage->fptr = (void *)superH_trapa;
      stage->op = OP_TRAPA;

      break;
    }

    case B0100: {
      stage->format = INSTR_D8;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movblg;
      stage->op = OP_MOVBLG;

      break;
    }

    case B0101: {
      stage->format = INSTR_D8;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movwlg;
      stage->op = OP_MOVWLG;

      break;
    }

    case B0110: {
      stage->format = INSTR_D8;
      stage->cycles = 1;
      stage->fptr = (void *)superH_movllg;
      stage->op = OP_MOVLLG;

      break;
    }

    case B0111: {
      stage->format = INSTR_D8;
      stage->cycles = 1;
      stage->fptr = (void *)superH_mova;
      stage->op = OP_MOVA;

      break;
    }

    case B1000: {
      stage->format = INSTR_I;
      stage->cycles = 1;
      stage->fptr = (void *)superH_tsti;
      stage->op = OP_TSTI;

      break;
    }

    case B1001: {
      stage->format = INSTR_I;
      stage->cycles = 1;
      stage->fptr = (void *)superH_andi;
      stage->op = OP_ANDI;

      break;
    }

    case B1010: {
      stage->format = INSTR_I;
      stage->cycles = 1;
      stage->fptr = (void *)superH_xori;
      stage->op = OP_XORI;

      break;
    }

    case B1011: {
      stage->format = INSTR_I;
      stage->cycles = 1;
      stage->fptr = (void *)superH_ori;
      stage->op = OP_ORI;

      break;
    }

    case B1100: {
      stage->format = INSTR_I;
      stage->cycles = 3;
      stage->fptr = (void *)superH_tstm;
      stage->op = OP_TSTM;

      break;
    }

    case B1101: {
      stage->format = INSTR_I;
      stage->cycles = 3;
      stage->fptr = (void *)superH_andm;
      stage->op = OP_ANDM;

      break;
    }

    case B1110: {
      stage->format = INSTR_I;
      stage->cycles = 3;
      stage->fptr = (void *)superH_xorm;
      stage->op = OP_XORM;

      break;
    }

    case B1111: {
      stage->format = INSTR_I;
      stage->cycles = 3;
      stage->fptr = (void *)superH_orm;
      stage->op = OP_ORM;

      break;
    }
    }

    break;
  }

  case B1101: {
    stage->format = INSTR_ND8;
    stage->cycles = 1;
    stage->fptr = (void *)superH_movli;
    stage->op = OP_MOVLI;

    break;
  }

  case B1110: {
    stage->format = INSTR_NI;
    stage->cycles = 1;
    stage->fptr = (void *)superH_movi;
    stage->op = OP_MOVI;

    break;
  }

  case B1111: {
    stage->format = INSTR_I;
    stage->cycles = 1;
    stage->fptr = (void *)superH_rfg;
    stage->op = OP_RFG;

    break;
  }

  default: {
    mprint(E, NULL, siminfo,
           "\nUnknown instruction seen in superHdecode()!\n\n");
    mexit(E, "See above messages", -1);

    break;
  }
  }

  return;
}

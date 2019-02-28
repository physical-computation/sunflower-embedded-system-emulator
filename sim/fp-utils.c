#include <stdio.h>
#include "sf.h"

void mfloatprint(Engine *E, State *S, uvlong num) {
  if ((num >> 32) == 0xFFFFFFFF) //NaN boxed
  {
    rv32f_rep val;
    val.bit_value = (uint32_t)num;
    mprint(E, S, nodeinfo, "  [%8f]  (Single)\n", val.float_value);
  } else {
    rv32d_rep val;
    val.bit64_value = (uint64_t)num;
    mprint(E, S, nodeinfo, "  [%8lf]  (Double)\n", val.double_value);
  }
}

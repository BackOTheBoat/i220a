#include "stall-sim.h"

#include "y86-util.h"

#include "errors.h"
#include "memalloc.h"

#include <assert.h>

enum {
  MAX_DATA_BUBBLES = 3,  /** max # of bubbles due to data hazards */
  JUMP_BUBBLES = 2,      /** # of bubbles for cond jump op */
  RET_BUBBLES = 3,       /** # of bubbles for return op */
  MAX_REG_WRITE = 2      /** max # of registers written per clock cycle */
};

struct StallSimStruct {
  //@TODO

  Y86 *y86;
};


/********************** Allocation / Deallocation **********************/

/** Create a new pipeline stall simulator for y86. */
StallSim *
new_stall_sim(Y86 *y86Sim)
{
  //@TODO

  StallSim *sim = malloc(sizeof(StallSim));
  
  //sim -> y86 = malloc(sizeof(Y86)*y86);

  return sim;
}

/** Free all resources allocated by new_pipe_sim() in stallSim. */
void
free_stall_sim(StallSim *stallSim)
{
  //@TODO
  free(stallSim);
}


/** Apply next pipeline clock to stallSim.  Return true if
 *  processor can proceed, false if pipeline is stalled.
 *
 * The pipeline will stall under the following circumstances:
 *
 * Exactly 4 clock cycles on startup to allow the pipeline to fill up.
 *
 * Exactly 2 clock cyclies after execution of a conditional jump.
 *
 * Exactly 3 clock cycles after execution of a return.
 *
 * Upto 3 clock cycles when attempting to read a register which was
 * written by any of upto 3 preceeding instructions.
 */
bool
clock_stall_sim(StallSim *stallSim)
{
  //@TODO

  Address programCounter = read_pc_y86(stallSim -> y86);
  Byte instruction = read_memory_byte_y86(stallSim -> y86, programCounter);

  Byte highNibble = get_nybble(instruction, 1);

  printf("\nPC is %d\n", programCounter);                                       
  printf("INS is: %d\n", highNibble);                                         

  switch (highNibble)
  {
    case Jxx_CODE:
      return false;
  }
  
  return true;
}

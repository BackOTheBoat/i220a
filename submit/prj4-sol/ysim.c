#include "ysim.h"
#include "errors.h"

/************************** Utility Routines ****************************/

/** Return nybble from op (pos 0: least-significant; pos 1:
 *  most-significant)
 */
static Byte get_nybble(Byte op, int pos)
{
  return (op >> (pos * 4)) & 0xF;
}

/************************** Condition Codes ****************************/

/** Conditions used in instructions */
typedef enum {
  ALWAYS_COND, LE_COND, LT_COND, EQ_COND, NE_COND, GE_COND, GT_COND
} Condition;

/** accessing condition code flags */
static inline bool get_cc_flag(Byte cc, unsigned flagBitIndex)
{
  return !!(cc & (1 << flagBitIndex));
}

static inline bool get_zf(Byte cc) { return get_cc_flag(cc, ZF_CC); }
static inline bool get_sf(Byte cc) { return get_cc_flag(cc, SF_CC); }
static inline bool get_of(Byte cc) { return get_cc_flag(cc, OF_CC); }

/** Return true iff the condition specified in the least-significant
 *  nybble of op holds in y86.  Encoding of Figure 3.15 of Bryant's
 *  CompSys3e.
 */
bool check_cc(const Y86 *y86, Byte op)
{
  bool ret = false;
  Condition condition = get_nybble(op, 0);
  Byte cc = read_cc_y86(y86);
  switch (condition) {
  case ALWAYS_COND:
    ret = true;
    break;
  case LE_COND:
    ret = (get_sf(cc) ^ get_of(cc)) | get_zf(cc);
    break;
  //@TODO add other cases -- DONE
  case LT_COND:
    ret = get_sf(cc) ^ get_of(cc);
    break;
  case EQ_COND:
    ret = get_zf(cc);
    break;
  case NE_COND:
    ret = ~get_zf(cc);
    break;
  case GE_COND:
    ret = ~(get_sf(cc) ^ get_of(cc));
    break;
  case GT_COND:
    ret = ~(get_sf(cc) ^ get_of(cc)) & get_zf(cc);
    break;
  default: {
    Address pc = read_pc_y86(y86);
    fatal("%08lx: bad condition code %d\n", pc, condition);
    break;
    }
  }
  return ret;
}

/** return true iff word has its sign bit set */
static inline bool isLt0(Word word)
{
  return (word & (1UL << (sizeof(Word)*CHAR_BIT - 1))) != 0;
}

/** Set condition codes for addition operation with operands opA, opB
 *  and result with result == opA + opB.
 */
static void set_add_arith_cc(Y86 *y86, Word opA, Word opB, Word result)
{
  //@TODO -- DONE
  Byte cc = 0;
  
  if (result == 0) //Test if result is zero
  {
    cc = cc + 1; //Set zero flag -> ZF = 1
  }
  
  if (result & 0x800000000000000 == 0x800000000000000)
  {
    cc = cc + 2; //Set sign flag -> SF = 1
  }
  
  //Checking for overflow
  if (opA > 0 && opB > 0 && result < 0)
  {   //A and B are +, RESULT is -
    cc = cc + 4;
  }
  
  if (opA < 0 && opB < 0 && result > 0)
  {   //A and B are -, RESULT is +
    cc = cc + 4;
  }
  
  write_cc_y86(y86, cc);
}

/** Set condition codes for subtraction operation with operands opA, opB
 *  and result with result == opA - opB.
 */
static void set_sub_arith_cc(Y86 *y86, Word opA, Word opB, Word result)
{
  //@TODO -- DONE
  Byte cc = 0;
  
  if (result == 0)
  {
    cc = cc + 1;
  }
  
  if (result & 0x800000000000000 == 0x800000000000000)
  {
    cc = cc + 2;
  }
  
  //Checking for overflow
  if (opA > 0 && opB < 0 && result < 0)
  {   //A is +, B is -, RESULT is NOT +
    cc = cc + 4;
  }
  
  if (opA < 0 && opB > 0 && result > 0)
  {   //A is -, B is +, RESULT is NOT -
    cc = cc + 4;
  }
  
  write_cc_y86(y86, cc);
}

static void set_logic_op_cc(Y86 *y86, Word result)
{
  //@TODO
  Byte cc = 0;

  if (result == 0)
  {
    cc = cc + 1;
  }
  
  if (result & 0x800000000000000 == 0x800000000000000)
  {
    cc = cc + 2;
  }
  
  write_cc_y86(y86, cc);
}

/**************************** Operations *******************************/

static void op1(Y86 *y86, Byte op, Register regA, Register regB)
{
  enum {ADDL_FN, SUBL_FN, ANDL_FN, XORL_FN };
  //@TODO

  Byte programCounter = read_pc_y86(y86);
  Word result;
  
  switch (op)
  {
    case ADDL_FN:
      result = read_register_y86(y86, regA) + read_register_y86(y86, regB);
      set_add_arith_cc(y86, read_register_y86(y86, regA), read_register_y86(y86, regB), result);
      write_register_y86(y86, regB, result);
      break;
    case SUBL_FN:
      //printf("OP 1: %lx OP 2: %lx\n", read_register_y86(y86, regA), read_register_y86(y86, regB));
      result = read_register_y86(y86, regA) - read_register_y86(y86, regB);
      set_sub_arith_cc(y86, read_register_y86(y86, regA), read_register_y86(y86, regB), result);
      write_register_y86(y86, regB, result);
      break;
    case ANDL_FN:
      result = read_register_y86(y86, regA) & read_register_y86(y86, regB);
      set_logic_op_cc(y86, result);
      write_register_y86(y86, regB, result);
      break;
    case XORL_FN:
      result = read_register_y86(y86, regA) ^ read_register_y86(y86, regB);
      set_logic_op_cc(y86, result);
      write_register_y86(y86, regB, result);
      break;
    default:
    {
      printf("INVALID OPCODE! Operation is: %d\n", op);
    }
  }
}

/*********************** Single Instruction Step ***********************/

typedef enum {
  HALT_CODE, NOP_CODE, CMOVxx_CODE, IRMOVQ_CODE, RMMOVQ_CODE, MRMOVQ_CODE,
  OP1_CODE, Jxx_CODE, CALL_CODE, RET_CODE,
  PUSHQ_CODE, POPQ_CODE } BaseOpCode;

/** Execute the next instruction of y86. Must change status of
 *  y86 to STATUS_HLT on halt, STATUS_ADR or STATUS_INS on
 *  bad address or instruction.
 */
void step_ysim(Y86 *y86)
{
  //@TODO

  Address programCounter = read_pc_y86(y86);
  Byte memoryByte = read_memory_byte_y86(y86, programCounter);

  if (read_status_y86(y86) != STATUS_AOK)
  {
    return;
  }
  
  Byte highNibble = get_nybble(memoryByte, 1);
  
  //printf("PC is %d\n", programCounter);
  //printf("INS is: %d\n", highNibble);

  switch (highNibble)
  {
    case HALT_CODE:
      write_status_y86(y86, STATUS_HLT);
      break;
    case NOP_CODE:
      write_pc_y86(y86, programCounter + 1);
      break;
    case CMOVxx_CODE:
      cmovq(y86);
      break;
    case IRMOVQ_CODE:
      irmovq(y86);
      break;
    case RMMOVQ_CODE:
      rmmov(y86);
      break;
    case MRMOVQ_CODE:
      mrmov(y86);
      break;
    case OP1_CODE:
      parseOp1(y86);
      break;
    case Jxx_CODE:
      break;
    case CALL_CODE:
      call(y86);
      break;
    case RET_CODE:
      ret(y86);
      break;
    case PUSHQ_CODE:
      break;
    case POPQ_CODE:
      break;
    default:
    {
      write_status_y86(y86, STATUS_INS);
      printf("BAD INS! INS is %d\n", highNibble);
      break;
    }
  }
}

void incrementPC(Y86 *y86)
{
  Address programCounter = read_pc_y86(y86);
  write_pc_y86(y86, programCounter + (1 + sizeof(Byte) + sizeof(Word)));
  
  return;
}

void push(Y86 *y86, Word value)
{
  return;
}

void pop(Y86 *y86)
{
  return;
}

void irmovq(Y86 *y86)
{
  Address programCounter = read_pc_y86(y86);
  Byte memoryByte = read_memory_byte_y86(y86, programCounter + sizeof(Byte));
  memoryByte = get_nybble(memoryByte, 0); //get lower byte
  
  Register destination = memoryByte; //Set the register to be modified
  Word value = read_memory_word_y86(y86, programCounter + 1 + sizeof(Byte)); //Get the value to place in the register

  //printf("Destination: %d, Value: %d\n");
  write_register_y86(y86, destination, value);
  incrementPC(y86);
  
  return;
}

void call(Y86 *y86)
{
  Address programCounter = read_pc_y86(y86); //Read pc
  Word callAddress = read_memory_word_y86(y86, programCounter + 1);
  Word rsp = read_register_y86(y86, REG_RSP); //Get address from RSP
  rsp -= sizeof(Word); //Decrement RSP
  
  write_register_y86(y86, REG_RSP, rsp); //Set RSP to new stack address
  write_memory_word_y86(y86, rsp, programCounter + 1 + sizeof(Word));
  
  write_pc_y86(y86, callAddress);
  
  return;
}

void ret(Y86 *y86)
{
  Word rsp = read_register_y86(y86, REG_RSP);
  Byte returnPC = read_memory_word_y86(y86, rsp);
  rsp += sizeof(Word);
  //printf("RSP: %lx\n", rsp);
  write_register_y86(y86, REG_RSP, rsp);
  
  //printf("RSP REGISTER: %lx", read_register_y86(y86, REG_RSP));
  
  write_pc_y86(y86, returnPC);
  
  return;
}

void rmmov(Y86 *y86)
{
  Address programCounter = read_pc_y86(y86); //Read pc
  Byte registerByte = read_memory_byte_y86(y86, programCounter + sizeof(Byte));
  Word displacement = read_memory_word_y86(y86, programCounter + sizeof(Byte) + sizeof(Byte)); //Get displacement
  
  Byte regA = get_nybble(registerByte, 1);
  Byte regB = get_nybble(registerByte, 0);
  
  Address effectiveAddress = read_register_y86(y86, regB) + displacement;
  
  write_memory_word_y86(y86, effectiveAddress, read_register_y86(y86, regA));
  write_pc_y86(y86, programCounter + (1 + sizeof(Byte) + sizeof(Word)));
  
  return;
}

void mrmov(Y86 *y86)
{
  Address programCounter = read_pc_y86(y86); //Read pc
  Byte registerByte = read_memory_byte_y86(y86, programCounter + sizeof(Byte)); //Read byte containing registers
  Word displacement = read_memory_word_y86(y86, programCounter + sizeof(Byte) + sizeof(Byte)); //Find the displacement
  
  Byte regA = get_nybble(registerByte, 0);
  Byte regB = get_nybble(registerByte, 1);
  
  Address effectiveAddress = read_register_y86(y86, regA) + displacement;
  //printf("Register: %d Address is: %lx\n", regA, effectiveAddress);
  Word value = read_memory_word_y86(y86, effectiveAddress);
  //printf("Value in memory is: %lx\n", value);

  write_register_y86(y86, regB, value);
  write_pc_y86(y86, programCounter + (1 + sizeof(Byte) + sizeof(Word))); //Increment PC by 10
  
  return;
}

void cmovq(Y86 *y86)
{
  Address programCounter = read_pc_y86(y86); //Read pc
  Byte registerByte = read_memory_byte_y86(y86, programCounter + sizeof(Byte)); //Read register byte

  Byte sourceRegister = get_nybble(registerByte, 1); //RegA
  Byte destination = get_nybble(registerByte, 0); //RegB

  Byte conditionCodes = read_cc_y86(y86);
  if (check_cc(y86, conditionCodes))
  {
    Word value = read_register_y86(y86, sourceRegister);
    write_register_y86(y86, destination, value);
  }
  else
  {
    destination = 0xf; //Destination = NO REGISTER <- 0xf
    write_register_y86(y86, destination, read_register_y86(y86, sourceRegister));
  }

  write_pc_y86(y86, programCounter + (2 * sizeof(Byte)));
}

void parseOp1(Y86 *y86)
{
  Address programCounter = read_pc_y86(y86);
  Byte operator = get_nybble(read_memory_byte_y86(y86, programCounter), 0);
  Byte registers = read_memory_byte_y86(y86, programCounter + sizeof(Byte));
  Byte regA = get_nybble(registers, 1);
  Byte regB = get_nybble(registers, 0);

  op1(y86, operator, regA, regB);
  write_pc_y86(y86, programCounter + (2 * sizeof(Byte)));
}

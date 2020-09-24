#include "hamming.h"
#include <assert.h>

/**
 * All bitIndex'es are numbered starting at the LSB which is given index 1
 * denotes exponentiation; note that 2**n == (1 << n)
 */

/** Return bit at bitIndex from word. */
static inline unsigned get_bit(HammingWord word, int bitIndex)
{
  assert(bitIndex > 0);
  //@TODO -- DONE

  HammingWord bit = (word >> bitIndex - 1) % 2;
  
  return bit;
}

/** Return word with bit at bitIndex in word set to bitValue. */
static inline HammingWord set_bit(HammingWord word, int bitIndex, unsigned bitValue)
{
  assert(bitIndex > 0);
  assert(bitValue == 0 || bitValue == 1);
  //@TODO -- DONE

  if (word == 0 && bitValue == 1)
    {
      word = 1;
      word = word << bitIndex;
    }
  else
  {
    word ^= (-bitValue ^ word) & (1UL << bitIndex - 1);
  }
  return word;
  
  return 0;
}

/** Given a Hamming code with nParityBits, return 2**nParityBits - 1,
 *  i.e. the max # of bits in an encoded word (# data bits + # parity
 *  bits).
 */
static inline unsigned get_n_encoded_bits(unsigned nParityBits)
{
  //@TODO -- DONE
  
  return (1 << nParityBits) - 1;
}

/** Return non-zero if bitIndex indexes a bit which will be used for a
 *  Hamming parity bit; i.e. the bit representation of bitIndex
 *  contains only a single 1.
 */
static inline int is_parity_position(int bitIndex)
{
  assert(bitIndex > 0);
  //@TODO -- DONE

  for (int i = 1; i <= bitIndex; i *= 2)
  {
    if (bitIndex == i)
    {
      return 1;
    }
  }
  
  return 0;
}

/** Return the parity over the data bits in word specified by the
 *  parity bit bitIndex.  The word contains a total of nBits bits.
 *  Equivalently, return parity over all data bits whose bit-index has
 *  a 1 in the same position as in bitIndex.
 */
static int compute_parity(HammingWord word, int bitIndex, unsigned nBits)
{
  assert(bitIndex > 0);
  //@TODO
  
  int totalBits = 1; //Number of bits used for PARITY and DATA
   
  while (totalBits <= nBits) //Find lowest power of 2 that can hold PARITY and DATA
  {
    totalBits = totalBits * 2; //Set total bits to lowest power of 2 possible with DATA and PARITY
  }

  HammingWord temp = 0; //HammingWord with DATA entered. Room for PARITY
  int nextBitLocation = totalBits - 1;

  for (int i = nBits; i >= 1; i = i - 1) //Iterating through HammingWord word by bit
  {
    int j = nextBitLocation;
    
    while (j > 1) //Iterating through HammingWord temp by bit to add DATA
    {
      if (is_parity_position(j) == 0) //If current index in temp IS NOT a parity position
      {
        unsigned bit = get_bit(word, i);
        temp = set_bit(temp, j, bit); //Set bit at current index in temp to DATA bit value
        nextBitLocation = j - 1;
        j = 1;
      }
      j = j - 1;
    }
  }

  int numOnes = 0;

  for (int i = 1; i < totalBits; i = i + 1) //Iterates through temp to compute number of ones
  {
    if (get_bit(i, bitIndex) == 1) //Determines if parity bit is responsible for current position
    {
      unsigned bit = get_bit(temp, i + 1);
      if (bit == 1) //Determines if bit at current position is a one
      numOnes = numOnes + 1; //If TRUE, adds a one to the counter
    }
  }
  if (numOnes % 2 == 1)
  {
    return 1;
  }
  else
  {
    return 0;
  }
  
  return 0;
}

/** Encode data using nParityBits Hamming code parity bits.
 *  Assumes data is within range of values which can be encoded using
 *  nParityBits.
 */
HammingWord hamming_encode(HammingWord data, unsigned nParityBits)
{
  //@TODO

  HammingWord encoded = 0;

  //copy loops from computeParity
  int totalBits = 1; //Number of bits used for DATA
   
  while (1 << totalBits < data) //Find lowest power of 2 that can hold DATA
  {
    totalBits = totalBits + 1; //Set total bits to lowest power of 2 possible with DATA and PARITY
  }

  HammingWord temp = 0; //HammingWord with DATA entered. Room for PARITY
  int nextBitLocation = totalBits - 1;

  for (int i = totalBits; i >= 1; i = i - 1) //Iterating through HammingWord word by bit
  {
    int j = nextBitLocation;
    
    while (j > 1) //Iterating through HammingWord temp by bit to add DATA
    {
      if (is_parity_position(j) == 0) //If current index in temp IS NOT a parity position
      {
        unsigned bit = get_bit(data, i);
        temp = set_bit(temp, j, bit); //Set bit at current index in temp to DATA bit value
        nextBitLocation = j - 1;
        j = 1;
      }
      j = j - 1;
    }
  }

  int nextParityLocation = 0;
  while (nParityBits > 0)
  {
    for (int i = nextParityLocation; i < totalBits; i = i + 1)
    {
      if (is_parity_position(i) == 1)
      {
	int parity = compute_parity(data, i, totalBits);
	encoded = set_bit(encoded, i, parity);

	nextParityLocation = i + 1;
	nParityBits = nParityBits - 1;
      }
    }
  }
  
  return encoded;
}

/** Decode encoded using nParityBits Hamming code parity bits.
 *  Set *hasError if an error was corrected.
 *  Assumes that data is within range of values which can be decoded
 *  using nParityBits.
 */
HammingWord hamming_decode(HammingWord encoded, unsigned nParityBits, int *hasError)
{
  //@TODO
  return 0;
}

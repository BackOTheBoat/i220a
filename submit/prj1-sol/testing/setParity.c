#include "hamming.h"
#include <stdio.h>
#include <assert.h>

static inline unsigned get_bit(HammingWord word, int bitIndex)
{
  assert(bitIndex > 0);
  //@TODO -- DONE                                                                                                                         
  HammingWord bit = (word >> bitIndex - 1) % 2;

  return bit;
}

HammingWord set_bit(HammingWord word, int bitIndex, unsigned bitValue)
{
  assert(bitIndex > 0);
  assert(bitValue == 0 || bitValue == 1);
  //@TODO -- DONE

  int newValue = 0;
  HammingWord tempWord = 0;

  word ^= (-newValue ^ word) & (1UL << bitIndex);

  return word;
}

int is_parity_position(int bitIndex)
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

int main()
{
  //arguments
  HammingWord word;
  int bitIndex;
  unsigned nBits;

  scanf("%lld", &word);
  scanf("%d", &bitIndex);
  scanf("%d", &nBits);

  //START
  int totalBits = 0; //Number of bits used for PARITY and DATA

  for (int i = 1; i <= nBits + 1; i = i * 2) //Find lowest power of 2 that can hold PARITY and DATA
  {
    printf("i: %d\n", i);
    totalBits = i; //Set total bits to lowest power of 2 possible with DATA and PARITY
  }
  printf("Total Bits: %d\n", totalBits);

  HammingWord temp = 0; //HammingWord with DATA entered. Room for PARITY

  for (int i = 1; i < nBits; i = i + 1) //Iterating through HammingWord word by bit
  {
    int j = 1;

    while (j < totalBits) //Iterating through HammingWord temp by bit to add DATA
    {
      if (is_parity_position(j) == 0) //If current index in temp IS NOT a parity position
      {
	temp = set_bit(temp, j, get_bit(word, i)); //Set bit at current index in temp to DATA bit value
      }
      j = j + 1;
    }
  }

  int numOnes = 0;

  for (int i = 1; i < totalBits; i = i + 1) //Iterates through temp to compute number of ones
  {
    if (get_bit(i, bitIndex) == 1) //Determines if parity bit is responsible for current position
    {
      if (get_bit(temp, i) == 1) //Determines if bit at current position is a one
	numOnes = numOnes + 1; //If TRUE, adds a one to the counter
    }
  }

  printf("NUM ONES: %d\n", numOnes);
  if (numOnes % 2 == 1)
  {
    printf("RETURN 1");
  }
  else
  {
    printf("RETURN 0");
  }
  
  return 0;
}

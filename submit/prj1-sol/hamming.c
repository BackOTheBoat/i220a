/**
 * All bitIndex'es are numbered starting at the LSB which is given index 1
 * denotes exponentiation; note that 2**n == (1 << n)
 */

/** Return bit at bitIndex from word. */
static inline unsigned get_bit(HammingWord word, int bitIndex)
{
  assert(bitIndex >= 0);
  //@TODO -- DONE

  //printf("Shift: %d\n", word >> bitIndex - 1);
  unsigned bit = (word >> bitIndex) % 2;
  
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
    word ^= (-bitValue ^ word) & (1UL << bitIndex);
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
  //printf("Total Bits: %d\n", totalBits);

  HammingWord temp = 0; //HammingWord with DATA entered. Room for PARITY
  int nextBitLocation = totalBits - 1;

  for (int i = nBits; i >= 1; i = i - 1) //Iterating through HammingWord word by bit
  {
    int j = nextBitLocation;
    
    while (j > 1) //Iterating through HammingWord temp by bit to add DATA
    {
      if (is_parity_position(j) == 0) //If current index in temp IS NOT a parity position
      {
        unsigned bit = get_bit(word, i - 1);
        temp = set_bit(temp, j, bit); //Set bit at current index in temp to DATA bit value
        nextBitLocation = j - 1;
        j = 1;
      }
      j = j - 1;
    }
  }
  //printf("Temp: %llu\n", temp);
  int numOnes = 0;
  int result;
  
  for (int x = 0; x >>= 1; x = x + 1)
      result++;

  for (int i = 1; i < totalBits; i = i + 1) //Iterates through temp to compute number of ones
  {
    //printf("I: %d\n", i);
    int parityBitIndex = result;
    //printf("PBI: %d\n", parityBitIndex);
    unsigned bitOfIndex = get_bit(i, parityBitIndex);
    //printf("indexBit: %d\n", bitOfIndex);
    if (bitOfIndex == 1) //Determines if parity bit is responsible for current position
    {
      unsigned bitAtPos = get_bit(temp, i);
      if (bitAtPos == 1) //Determines if bit at current position is a one
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
  
  return 1;
}

/** Encode data using nParityBits Hamming code parity bits.
 *  Assumes data is within range of values which can be encoded using
 *  nParityBits.
 */
HammingWord hamming_encode(HammingWord data, unsigned nParityBits)
{
  //@TODO
  
  int nBits = 1;
  HammingWord encoded = 1;
  
  while (1 << nBits < data) //find the number of bits in data
  {
      nBits = nBits + 1;
  }
  nBits = nBits + 1;
  //printf("NBits: %d\n", nBits);

  //copy loops from computeParity
  int totalBits = get_n_encoded_bits(nParityBits); //Number of bits used for DATA

  HammingWord temp = 0; //HammingWord with DATA entered. Room for PARITY
  int nextBitLocation = totalBits;

  for (int i = nBits; i >= 1; i = i - 1) //Iterating through HammingWord DATA by bit
  {
    //printf("Next Bit Location: %d\n", nextBitLocation);
    int j = nextBitLocation;
    
    while (j > 1) //Iterating through HammingWord temp by bit to add DATA
    {
      int isParity = is_parity_position(j);
      if (isParity == 0) //If current index in temp IS NOT a parity position
      {
        //printf("i: %d\n", i);
        unsigned bit = get_bit(data, i - 1); //get bit from DATA
        //printf("Bit: %d\n", bit);
        //printf("J: %d\n", j);
        temp = set_bit(temp, j, bit); //Set bit at current index in temp to DATA bit value
        //printf("Temp: %llu\n", temp);
        nextBitLocation = j - 1;
        //printf("next bit for loop: %d\n", nextBitLocation);
        j = 1;
      }
      j = j - 1;
      //printf("I after loop: %d\n", i);
    }
  }
  encoded = temp;
  //printf("encoded after loop: %llu\n", temp);
  int nextParityLocation = 1;
  
  for (int i = 1; i < totalBits; i = i * 2)
  {
      int parity = compute_parity(data, i, totalBits);
      printf("location: %d, parity: %d\n", i, parity);
      encoded = set_bit(encoded, i, parity);
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

  int syndrome = 0;
  int totalBits = 0;

  //find the total number of bits in ENCODED
  while ((1 << totalBits) - 1 < encoded)
  {
    totalBits = totalBits + 1;
  }

  
  for (int i = 1; i <= totalBits; i = i + 1)
  {
    unsigned bit = get_bit(encoded, i); //Retrieve bit from encoded at location i
    if (bit == 1) //Does this position in encoded have a 1?
    {
      syndrome = syndrome ^ i; //XOR the index with the list of indicies with 1
    }
  }

  if (syndrome != 0)
  {
    *hasError = 1; //Toggle the error variable
    unsigned errorBit = get_bit(encoded, syndrome); //retrieve the bit at location SYNDROME;

    if (errorBit == 1)
    {
      encoded = set_bit(encoded, syndrome, 0);
    }
    else
    {
      encoded = set_bit(encoded, syndrome, 1);
    }
  }
  //printf("Encoded: %llu\n", encoded);
  
  int originalBits = totalBits - nParityBits;
  HammingWord original = 0;
  
  for (int i = totalBits; i > 0; i = i - 1)
  {
      printf("i: %d\n", i);
      if (is_parity_position(i) == 0)
      {
          unsigned bit = get_bit(encoded, i);
          printf("Bit: %d\n", bit);
          printf("Original Bits %d\n", originalBits);
          original = set_bit(original, originalBits, bit);
          printf("Original: %llu\n", original >> 2);
          originalBits = originalBits - 1;
      }
  }
  original = original >> 1;
  
  return original;
}
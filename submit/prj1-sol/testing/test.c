#include "hamming.h"
#include <stdio.h>

int main()
{
  int bitIndex = 0;
  HammingWord word = 0;
  scanf("%lld", &word);
  scanf("%d", &bitIndex);
  
  printf("%lld", word);
  printf("\n");

  HammingWord bit = (word >> bitIndex - 1) % 2;

  printf("Returned bit:\n");
  printf("%lld\n", bit);
  return bit;
}

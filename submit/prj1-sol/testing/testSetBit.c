#include <stdio.h>
#include <assert.h>
#include "hamming.h"

int main()
{
  int bitIndex = 0;
  int bitValue = 0;
  HammingWord word = 0;
  HammingWord tempWord = 1;
  scanf("%lld", &word);
  scanf("%d", &bitIndex);
  scanf("%d", &bitValue);

  assert(bitIndex > 0);

  if (word == 0 && bitValue == 1)
  {
    word = 1;
    word = word << bitIndex;
  }
  else
  {
    word ^= (-bitValue ^ word) & (1UL << bitIndex - 1);
  }

  printf("\n%lld", word);
  printf("\n");
  return 0;
}

#include <stdio.h>
#include "hamming.h"

int main()
{
  int bitIndex = 0;
  HammingWord word = 0;

  scanf("%lld", &word);
  scanf("%d", &bitIndex);

  //START
  int digits = 0;
  HammingWord copy = word;
  
  for (int i = 1; i <= bitIndex; i *= 2)
  {
    printf("Index: %d i: %d", bitIndex, i);
    if (bitIndex == i)
    {
      printf("TRUE");
      return 1;
    }
  }
  
  //2 << i == bitIndex
  printf("FALSE");
  return 0;
}

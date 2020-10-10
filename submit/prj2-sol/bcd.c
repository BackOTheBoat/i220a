#include "bcd.h"

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>

/** Returns the decimal value of a set of bits extracted from a value
 *
 *  EX: getHexSegment(1944, 4) = 9
 *  EX: getHexSegment(1944, 0) = 8
 */
int getHexSegment(long long value, int startPos)
{
  if (startPos < 0)
  {
    printf("BAD INDEX! Index must be 0 or larger. Index is: %d\n", startPos);
    return 0;
  }  

  if ((1 << startPos) - 1 > value)
  {
    printf("BAD INDEX! Index must be within the bounds of the given value\n");
    printf("Index is %d. Value is %llu. %d is out of range\n", startPos, value, (1 << startPos) - 1);
    return 0;
  }
  
  long long mask = (1 << (4)) - 1;
  int bitSegment = (value >> startPos) & mask;
  
  
  return bitSegment;
}


/** Returns a digit in hexadecimal at place INDEX of a VALUE in base 10
 *
 *  EX: getHexDigit(1944, 0) = 8
 *  EX: getHexDigit(1944, 1) = 9
 *  EX: getHexDigit(1944, 2) = 7
 */
int getHexDigit(long long value, int index)
{
  int hexDigits = 1;
  while (value > pow(16, hexDigits) - 1)
  {
    hexDigits = hexDigits + 1;
  }

  if (index < 0)
  {
    printf("BAD INDEX! Index must be 0 or larger. Index is: %d\n", index);
    return value;
  }

  if (index > hexDigits)
  {
    printf("BAD INDEX! Index cannot be larger than the number of digits - 1\n");
    printf("Index is: %d. There are %d digits in value\n", index, hexDigits);
    return value;
  }

  int digit = 0;
  int startBit = index * 4;
  
  digit = getHexSegment(value, startBit);
  return digit;
}

/** Returns the value of a given index in BCD
 *
 * CHECK: Index >= 0
 */
int getBcdDigit(Bcd bcd, int index)
{
  if (index < 0)
  {
    printf("BAD INDEX! Index must be 0 or larger. Index is: %d\n", index);
    return bcd;
  }

  int digit = 0;

  while (index > 0)
  {
    bcd = bcd / 10;
    index = index - 1;
  }

  digit = bcd % 10;
  return digit;
}

/** Returns a BCD with the given INDEX changed to VALUE
 *
 * CHECK: Index >= 0
 * CHECK: Value >= 0
 */
Bcd setBcdDigit(Bcd bcd, int index, int value)
{
  if (index < 0)
  {
    printf("BAD INDEX! Index must be 0 or larger. Index is: %d\n", index);
    return bcd;
  }

  if (value < 0)
  {
    printf("BAD VALUE! Value must be 0 or larger. Value is: %d\n", value);
    return bcd;
  }
  
  Bcd temp = 1; //BCD where digits are stored. Set to 1 to accomodate for 0s in lower place values

  if (bcd == 0) //Check for an empty BCD (no value)
  {
    bcd = value;

    while (index > 0)
    {
      bcd = bcd * 10; //Add zeros to end of new number
      index = index - 1;
    }
  }
  else //executed if BCD has a value
  {
    while (index > 0) //save digits to temp
    {
      int digit = bcd % 10; //Get digit from BCD
      temp = temp * 10; //Add a new place value to temp
      temp = temp + digit; //put the digit in temp
      bcd = bcd / 10; //remove the last digit of temp
      index = index - 1;
    }

    bcd	= bcd /	10; //Remove last digit (to be changed)
    bcd	= bcd *	10; //Add a new digit for new value
    bcd	= bcd +	value; //Add the new value to the last digit

    while (temp >= 1) //restore the digits held in temp to BCD
    {
      int digit = temp % 10;
      bcd = bcd * 10;
      bcd = bcd + digit;
      temp = temp / 10;
    }
  }
}

int pow(int value, int exponent)
{
  if (exponent == 0)
  {
    return 1;
  }

  if (exponent < 0)
  {
    printf("BAD EXPONENT! Exponent must be 0 or larger\n");
    return value;
  }

  int result = value;
  while (exponent > 0)
  {
    result = result * value;
  }

  return result;
}

/** Return BCD encoding of binary (which has normal binary representation).
 *
 *  Examples: binary_to_bcd(0xc) => 0x12;
 *            binary_to_bcd(0xff) => 0x255
 *
 *  If error is not NULL, sets *error to OVERFLOW_ERR if binary is too
 *  big for the Bcd type, otherwise *error is unchanged.
 */
Bcd binary_to_bcd(Binary value, BcdError *error)
{
  //@TODO -- DONE
  Bcd result = 0;
  int index = 0;

  if (error != NULL)
  {
    if (value > MAX_BCD_DIGITS)
    {
	*error = OVERFLOW_ERR;
    }
  }
  
  while (value > 0) //While there are still digits in value
  {
    int digit = value % 10; //isolate the last digit
    digit = digit * pow(16, index); //Convert to base 16
    setBcdDigit(result, index, digit); //Set result in BCD

    index = index + 1; //Increment the index of BCD
    value = value / 10; //Remove last digit from value
  }
  
  return result;
}

/** Return binary encoding of BCD value bcd.
 *
 *  Examples: bcd_to_binary(0x12) => 0xc;
 *            bcd_to_binary(0x255) => 0xff
 *
 *  If error is not NULL, sets *error to BAD_VALUE_ERR if bcd contains
 *  a bad BCD digit.
 *  Cannot overflow since Binary can represent larger values than Bcd
 */
Binary bcd_to_binary(Bcd bcd, BcdError *error)
{
  //@TODO

  Binary result = 0;
  
  return 0;
}

/** Return BCD encoding of decimal number corresponding to string s.
 *  Behavior undefined on overflow or if s contains a non-digit
 *  character.  Sets *p to point to first non-digit char in s.
 *  Rougly equivalent to strtol().
 *
 *  If error is not NULL, sets *error to OVERFLOW_ERR if binary is too
 *  big for the Bcd type, otherwise *error is unchanged.
 */
Bcd str_to_bcd(const char *s, const char **p, BcdError *error)
{
  //@TODO
  return 0;
}

/** Convert bcd to a NUL-terminated string in buf[] without any
 *  non-significant leading zeros.  Never write more than bufSize
 *  characters into buf.  The return value is the number of characters
 *  written (excluding the NUL character used to terminate strings).
 *
 *  If error is not NULL, sets *error to BAD_VALUE_ERR is bcd contains
 *  a BCD digit which is greater than 9, OVERFLOW_ERR if bufSize bytes
 *  is less than BCD_BUF_SIZE, otherwise *error is unchanged.
 */
int bcd_to_str(Bcd bcd, char buf[], size_t bufSize, BcdError *error)
{
  //@TODO
  return 0;
}

/** Return the BCD representation of the sum of BCD int's x and y.
 *
 *  If error is not NULL, sets *error to to BAD_VALUE_ERR is x or y
 *  contains a BCD digit which is greater than 9, OVERFLOW_ERR on
 *  overflow, otherwise *error is unchanged.
 */
Bcd bcd_add(Bcd x, Bcd y, BcdError *error)
{
  //@TODO

  Binary bSum = x + y;
  //  Bcd sum = binary_to_bcd(bSum, *error);

  //  return sum;
}

/** Return the BCD representation of the product of BCD int's x and y.
 *
 * If error is not NULL, sets *error to to BAD_VALUE_ERR is x or y
 * contains a BCD digit which is greater than 9, OVERFLOW_ERR on
 * overflow, otherwise *error is unchanged.
 */
Bcd bcd_multiply(Bcd x, Bcd y, BcdError *error)
{
  //@TODO

  Binary bProduct = x * y;
  //  Bcd product = binary_to_bcd(bProduct, *error);

  //  return product;
}

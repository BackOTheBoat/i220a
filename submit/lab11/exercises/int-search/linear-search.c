/** Return index of element in a[nElements]; < 0 if not found. */
int
search_for_element(int a[], int nElements, int element)
{
  //@TODO add your code here to meet above spec.
  for (int index = 0; index < nElements; index = index + 1)
  {
    if (element == a[index])
    {
      return index;
    }
  }
  return -1;
}

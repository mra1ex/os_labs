#include "sum.h"

#include <stdio.h>
int get_sum(int *array, int begin, int end){
  int res = 0;
  for (int i = begin; i < end; i++)
    res += array[i];
  return res;
}

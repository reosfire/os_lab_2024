#include "find_min_max.h"

#include <limits.h>

struct MinMax GetMinMax(int *array, unsigned int begin, unsigned int end) {
  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  while (begin < end) {
    int current = array[begin];
    if (current < min_max.min) min_max.min = current;
    if (current > min_max.max) min_max.max = current;

    begin++;
  }

  return min_max;
}

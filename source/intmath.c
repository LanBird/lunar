#include <stdint.h>

#include "intmath.h"

/**
 * Calculates the floor of the binary logarithm of parameter 'n'. Thanks to the
 * author of the German Wikipedia page.
 * @param n the input argument.
 * @return the floor of the binary logarithm of n.
 */
int64_t intmath_log2( uint64_t n ) {
  int64_t pos = 0;
  if (n >= 0x100000000ll) { n >>= 32; pos += 32; }
  if (n >= 0x000010000ll) { n >>= 16; pos += 16; }
  if (n >= 0x000000100ll) { n >>=  8; pos +=  8; }
  if (n >= 0x000000010ll) { n >>=  4; pos +=  4; }
  if (n >= 0x000000004ll) { n >>=  2; pos +=  2; }
  if (n >= 0x000000002ll) {           pos +=  1; }
  return ((n == 0) ? (-1) : pos);
}

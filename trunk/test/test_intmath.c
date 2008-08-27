#include "test.h"
#include "types.h"
#include "intmath.h"

int intmath_test();

struct test_info test_intmath = {
  "Integer Math",
  __FILE__,
  0,
  intmath_test,
  0,
  0
};

int intmath_log2_test() {
  int errors = 0;
  if( intmath_log2( 0x8000000000000000ll ) != 63 ) {
    errors++;
  }
  if( intmath_log2( 1 ) != 0 ) {
    errors++;
  }
  if( intmath_log2( 255 ) != 7 ) {
    errors++;
  }
  if( intmath_log2( 256 ) != 8 ) {
    errors++;
  }
  if( intmath_log2( 0x8000000000000000ll ) != 63 ) {
    errors++;
  }
  if( intmath_log2( 0x8fffffffffffffffll ) != 63 ) {
    errors++;
  }  
  return errors;
}

int intmath_test() {
  int errors = 0;

  errors += intmath_log2_test();
  
  return errors;
}


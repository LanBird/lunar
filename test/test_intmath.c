#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "test.h"
#include "intmath.h"

void intmath_test();

struct test_info test_intmath = {
  "Integer Math",
  __FILE__,
  0,
  intmath_test,
  0,
  0
};

void intmath_log2_test() {
  if( intmath_log2( 1 ) != 0 ) {
    test_error( "intmath_log2( 1 ) returned wrong result." );
  }
  if( intmath_log2( 255 ) != 7 ) {
    test_error( "intmath_log2( 255 ) returned wrong result." );
  }
  if( intmath_log2( 256 ) != 8 ) {
    test_error( "intmath_log2( 256 ) returned wrong result." );
  }
  if( intmath_log2( 0x8000000000000000ll ) != 63 ) {
    test_error( "intmath_log2( 0x8000000000000000ll ) returned wrong result." );
  }
  if( intmath_log2( 0x8fffffffffffffffll ) != 63 ) {
    test_error( "intmath_log2( 0x8fffffffffffffffll ) returned wrong result." );
  }  
}

void intmath_test() {
  intmath_log2_test();
}


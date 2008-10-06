#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "rtctl.h"

void rtctl_test();

struct test_info test_rtctl = {
  "Runtime Control",
  __FILE__,
  NULL,
  rtctl_test,
  NULL,
  0
};

struct rtctl_info test_rtctl_mystring = \
  RTCTL_STRING_INITIALIZER;

struct rtctl_info test_rtctl_myboolean = \
  RTCTL_BOOLEAN_INITIALIZER;

struct rtctl_info test_rtctl_myinteger = \
  RTCTL_INTEGER_INITIALIZER;

struct rtctl_info test_rtctl_myreal = \
  RTCTL_REAL_INITIALIZER;

int test_rtctl_myinit_calls = 0;

void test_rtctl_myinit( rtctl_t hook ) {
  test_rtctl_myinit_calls++;
}

void rtctl_test() {
  rtctl_set_init_function( &test_rtctl_mystring, test_rtctl_myinit );
  rtctl_set_init_function( &test_rtctl_myinteger, test_rtctl_myinit );
  rtctl_set_init_function( &test_rtctl_myboolean, test_rtctl_myinit );
  rtctl_set_init_function( &test_rtctl_myreal, test_rtctl_myinit );
  
  rtctl_register( &test_rtctl_mystring,  "test.mystring" );
  rtctl_register( &test_rtctl_myboolean, "test.myboolean" );
  rtctl_register( &test_rtctl_myinteger, "test.myinteger" );
  rtctl_register( &test_rtctl_myreal,    "test.myreal" );
  
  test_print( "Testing string conversion" );
  rtctl_set_string( &test_rtctl_mystring, "This is a test" );
  char stringv[17];
  rtctl_get_string( &test_rtctl_myinteger, stringv );
  if( strcmp( stringv, "This is a test" ) ) {
    test_error( "test.mystring has value \"%s\", expected \"This is a test\"", stringv );
  }

  test_print( "Testing boolean conversion" );
  rtctl_set_boolean( &test_rtctl_myboolean, 1 );
  unsigned char boolv = 0;
  rtctl_get_boolean( &test_rtctl_myboolean, &boolv );
  if( ! boolv ) {
    test_error( "test.mybool evalueates to false, expected true" );
  }

  test_print( "Testing integer conversion" );
  rtctl_set_integer( &test_rtctl_myinteger, 2222 );
  long long intv = 0;
  rtctl_get_integer( &test_rtctl_myinteger, &intv );
  if( intv != 2222 ) {
    test_error( "test.myinteger has value %i, expected 2222", intv );
  }

  test_print( "Testing real conversion" );
  rtctl_set_real( &test_rtctl_myinteger, 22.22 );
  double realv = 0;
  rtctl_get_real( &test_rtctl_myinteger, &realv );
  if( realv < 22.21999 || realv > 22.22001 ) {
    test_error( "test.myreal has value %d, expected 22.22", realv );
  }
}

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

struct rtctl_info rtctl_test_mystring = RTCTL_STRING_INITIALIZER;
struct rtctl_info rtctl_test_myboolean = RTCTL_BOOLEAN_INITIALIZER;
struct rtctl_info rtctl_test_myinteger = RTCTL_INTEGER_INITIALIZER;
struct rtctl_info rtctl_test_myreal = RTCTL_REAL_INITIALIZER;

int rtctl_test_myinit_calls = 0;

void rtctl_test_myinit( rtctl_t hook ) {
  rtctl_test_myinit_calls++;
}

void rtctl_test_initialization() {
  rtctl_set_init_function( &rtctl_test_mystring, rtctl_test_myinit );
  rtctl_set_init_function( &rtctl_test_myinteger, rtctl_test_myinit );
  rtctl_set_init_function( &rtctl_test_myboolean, rtctl_test_myinit );
  rtctl_set_init_function( &rtctl_test_myreal, rtctl_test_myinit );
  
  rtctl_register( &rtctl_test_mystring,  "test.mystring" );
  rtctl_register( &rtctl_test_myboolean, "test.myboolean" );
  rtctl_register( &rtctl_test_myinteger, "test.myinteger" );
  rtctl_register( &rtctl_test_myreal,    "test.myreal" );

  if( rtctl_test_myinit_calls != 4 ) {
    test_error( "Missing initialization calls" );
  }
}

void rtctl_test_lookup() {
  if( rtctl_lookup( "test.mystring" ) != &rtctl_test_mystring ) {
    test_error( "Lookup of test.mystring failed" );
  }
  if( rtctl_lookup( "test.myboolean" ) != &rtctl_test_myboolean ) {
    test_error( "Lookup of test.myboolean failed" );
  }
  if( rtctl_lookup( "test.myinteger" ) != &rtctl_test_myinteger ) {
    test_error( "Lookup of test.myinteger failed" );
  }
  if( rtctl_lookup( "test.myreal" ) != &rtctl_test_myreal ) {
    test_error( "Lookup of test.myreal failed" );
  }
}

void rtctl_test_string_conversion() {
  rtctl_set_string( &rtctl_test_mystring, "This is a test" );
  char stringv[17];
  rtctl_get_string( &rtctl_test_mystring, stringv );
  if( strcmp( stringv, "This is a test" ) ) {
    test_error( "test.mystring has value \"%s\", expected \"This is a test\"", stringv );
  }
}

void rtctl_test_boolean_conversion() {
  rtctl_set_boolean( &rtctl_test_myboolean, 1 );
  unsigned char boolv = 0;
  rtctl_get_boolean( &rtctl_test_myboolean, &boolv );
  if( ! boolv ) {
    test_error( "test.mybool evalueates to false, expected true" );
  }
}

void rtctl_test_integer_conversion() {
  rtctl_set_integer( &rtctl_test_myinteger, 2222 );
  long long intv = 0;
  rtctl_get_integer( &rtctl_test_myinteger, &intv );
  if( intv != 2222 ) {
    test_error( "test.myinteger has value %i, expected 2222", intv );
  }
}

void rtctl_test_real_conversion() {
  rtctl_set_real( &rtctl_test_myreal, 22.22 );
  double realv = 0;
  rtctl_get_real( &rtctl_test_myreal, &realv );
  if( realv < 22.21999 || realv > 22.22001 ) {
    test_error( "test.myreal has value %d, expected 22.22", realv );
  }
}

void rtctl_test() {
  test_print( "initialization" );
  rtctl_test_initialization();
  test_print( "lookup" );
  rtctl_test_lookup();
  test_print( "string conversion" );
  rtctl_test_string_conversion();
  test_print( "boolean conversion" );
  rtctl_test_boolean_conversion();
  test_print( "integer conversion" );
  rtctl_test_integer_conversion();
  test_print( "real conversion" );
  rtctl_test_real_conversion();
  test_print( "name-based access" );
  test_warning( "Test not yet implemented" );
}


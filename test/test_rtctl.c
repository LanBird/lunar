#include "test.h"
#include "rtctl.h"

int rtctl_test();

struct test_info test_rtctl = {
  "Runtime Control",
  __FILE__,
  NULL,
  rtctl_test,
  NULL,
  0
};

struct rtctl_info test_rtctl_mystring \
  RTCTL_STRING_INITIALIZER( "test.mystring" );

struct rtctl_info test_rtctl_myboolean \
  RTCTL_STRING_INITIALIZER( "test.myboolean" );

struct rtctl_info test_rtctl_myinteger \
  RTCTL_STRING_INITIALIZER( "test.myinteger" );

struct rtctl_info test_rtctl_myreal \
  RTCTL_STRING_INITIALIZER( "test.myreal" );

int test_rtctl_myinit_calls = 0;

void test_rtctl_myinit( rtctl_t hook ) {
  test_rtctl_myinit_calls++;
}

int rtctl_test() {
  rtctl_set_init_function( test_rtctl_mystring, test_rtctl_myinit );
  rtctl_set_init_function( test_rtctl_myinteger, test_rtctl_myinit );
  rtctl_set_init_function( test_rtctl_myboolean, test_rtctl_myinit );
  rtctl_set_init_function( test_rtctl_myreal, test_rtctl_myinit );
  
  rtctl_register( test_rtctl_mystring );
  rtctl_register( test_rtctl_myboolean );
  rtctl_register( test_rtctl_myinteger );
  rtctl_register( test_rtctl_myreal );
  
  rtctl_set( "test.myreal", "2.25" );
  double realv = 0;
  rtctl_get_real( test_rtctl_myreal, &realv );
  if( realv != 2.25 ) {
    return 1;
  }
  return 0;
}

#include <stdio.h>
#include "test.h"
// the tests.h file is generated on the fly when rebuilding this file
#include "tests.h"

int test_fail() {
}

int test_success() {
}

int test_output() {
}

int test_require( struct test_info * test ) {
}

int run_test( struct test_info * test ) {
  int e0=0, e1=0, e2=0;
  
  printf( "Running test '%s' from file '%s'.\n", test->name, test->filename );
  
  if( test->set_up != 0 ) {
    e0 = (*(test->set_up))();
    if( e0 > 0 ) {
      printf( "Error during setup. (#%i)\n", e0 );
    }      
  }
  if( test->run != 0 && e0 == 0 ) {
    e1 = (*(test->run))();
    if( e1 > 0 ) {
      printf( "Error during test. (#%i)\n", e1 );
    } else {
      printf( "Test succeeded.\n" );
    }
  }
  if( test->tear_down != 0 && e0 == 0 ) {
    e2 = (*(test->tear_down))();
    if( e2 > 0 ) {
      printf( "Error during tear down. (#%i)\n", e2 );
    }
  }
  return e0 + e1 + e2;
}

int main( void ) {
  int i, test_count = 0;
  for( i=0; tests[i] != 0; i++ ) {
    test_count++;
  }
  
  if( test_count == 0 ) {
    printf( "  No tests ..\n" );
  } else if( test_count == 1 ) {
    printf( "  Running test ..\n" );
  } else {
    printf( "  Running %i tests ..\n", test_count );
  }
  //       12345678901234567890123456789012345678901234567890123456789012345678901234567890
  for( i=0; i<test_count; i++ ) {
    printf( "-[ %2i of %2i ]-------------------------------------------------------------------\n", i+1, test_count );
    run_test( tests[i] );
  }  
  printf( "--------------------------------------------------------------------------------\n" );
  
  return 0;
}


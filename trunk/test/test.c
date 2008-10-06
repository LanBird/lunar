#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "test.h"
#include "tests.h"

struct test_info * current_test;
const char * current_stage;
time_t now;

void test_print( const char * message, ... ) {
  va_list args;
  va_start( args, message );
  printf( "     " );
  vprintf( message, args );
  printf( "\n" );
  va_end( args );
}

void test_error( const char * message, ... ) {
  va_list args;
  va_start( args, message );
  current_test->errors++;
  printf( "   * Error: " );
  vprintf( message, args );
  printf( "\n" );
  va_end( args );
}

void test_warning( const char * message, ... ) {
  va_list args;
  va_start( args, message );
  current_test->warnings++;
  printf( "   ! Warning: " );
  vprintf( message, args );
  printf( "\n" );
  va_end( args );
}

void test_run( struct test_info * test ) {
  printf( "Running test '%s' from file '%s'.\n", test->name, test->filename );
  printf( "--------------------------------------------------------------------------------\n" );
  current_test = test;
  
  if( test->set_up != 0 ) {
    current_stage = "set up";
    now = time( NULL );
    printf( "  [%s] %s", current_stage, ctime( &now ) );
    (*(test->set_up))();
  }
  if( test->errors == 0 ) {
    if( test->run != 0 ) {
      current_stage = "run";
      printf( "  [%s] %s", current_stage, ctime( &now ) );
      (*(test->run))();
    }
    if( test->tear_down != 0 ) {
      current_stage = "tear down";
      printf( "  [%s] %s", current_stage, ctime( &now ) );
      (*(test->tear_down))();
    }
  }

  printf( "--------------------------------------------------------------------------------\n" );
  printf( "  Errors:   %i\n  Warnings: %i\n", test->errors, test->warnings );
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
    test_run( tests[i] );
  }  
  printf( "--------------------------------------------------------------------------------\n" );
  
  return 0;
}


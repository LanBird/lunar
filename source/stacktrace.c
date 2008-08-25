#include <stdio.h>
#include "stacktrace.h"

int stacktrace_stack_format();

void * stacktrace_seek_start( int depth ) {
  int i;
  printf( "%u == %u\n", &stacktrace_seek_start, *(&depth+1) );
  if( &stacktrace_seek_start == *(&depth+1) ) {
    printf( "Stacktrace will work on this platform.\n" );
    for( i=-5; i<depth; i++ ) {
      printf( "%u : %u\n", (&depth-i), *(&depth-i) );
    }
  }
}

int stacktrace_stack_format_dummy( int a, int b ) {
  printf( "Printing Stack around %s( %i, %i ) @ 0x%x:\n", __FUNCTION__, a, b, &stacktrace_stack_format_dummy );
  
  printf( "  %x: 0x%x\n", &a-13, *(&a-13) );
  printf( "  %x: 0x%x\n", &a-12, *(&a-12) );
  printf( "  %x: 0x%x\n", &a-11, *(&a-11) );
  printf( "  %x: 0x%x\n", &a-10, *(&a-10) );
  printf( "  %x: 0x%x\n", &a-9, *(&a-9) );
  printf( "  %x: 0x%x\n", &a-8, *(&a-8) );
  printf( "  %x: 0x%x\n", &a-7, *(&a-7) );
  printf( "  %x: 0x%x\n", &a-6, *(&a-6) );
  printf( "  %x: 0x%x\n", &a-5, *(&a-5) );
  printf( "  %x: 0x%x\n", &a-4, *(&a-4) );
  printf( "  %x: 0x%x\n", &a-3, *(&a-3) );
  printf( "  %x: 0x%x\n", &a-2, *(&a-2) );
  printf( "  %x: 0x%x\n", &a-1, *(&a-1) );
  printf( "a %x: 0x%x\n", &a, a );
  printf( "b %x: 0x%x\n", &b, b );
  printf( "  %x: 0x%x\n", &b+1, *(&b+1) );
  printf( "  %x: 0x%x\n", &b+2, *(&b+2) );
  printf( "  %x: 0x%x\n", &b+3, *(&b+3) );
  printf( "  %x: 0x%x\n", &b+4, *(&b+4) );
  printf( "  %x: 0x%x\n", &b+5, *(&b+5) );
  printf( "  %x: 0x%x\n", &b+6, *(&b+6) );
  printf( "  %x: 0x%x\n", &b+7, *(&b+7) );
  printf( "  %x: 0x%x\n", &b+8, *(&b+8) );
  printf( "  %x: 0x%x\n", &b+9, *(&b+9) );
  printf( "  %x: 0x%x\n", &b+10, *(&b+10) );
  printf( "  %x: 0x%x\n", &b+11, *(&b+11) );
  printf( "  %x: 0x%x\n", &b+12, *(&b+12) );
  
  long long t = *(&a-1);
  t -= 1;
  int * c = (int *) t;
  
  printf( "Printing code around return address 0x%x:\n", c );
  printf( "  %x: 0x%x\n", c-7, *(c-7) );
  printf( "  %x: 0x%x\n", c-6, *(c-6) );
  printf( "  %x: 0x%x\n", c-5, *(c-5) );
  printf( "  %x: 0x%x\n", c-4, *(c-4) );
  printf( "  %x: 0x%x\n", c-3, *(c-3) );
  printf( "  %x: 0x%x\n", c-2, *(c-2) );
  printf( "  %x: 0x%x\n", c-1, *(c-1) );
  printf( "  %x: 0x%x\n", c, *c );
  printf( "  %x: 0x%x\n", c+1, *(c+1) );
  printf( "  %x: 0x%x\n", c+2, *(c+2) );
  return 3;
}

int stacktrace_stack_format() {
  int test;
  stacktrace_stack_format_dummy( 1, 2 );
  return 2;
}

void * stacktrace_t1( int i ) {
  return stacktrace_seek_start( i );
}

void * stacktrace_t2( int i ) {
  return stacktrace_t1( i );
}

void stacktrace() {
  printf( "stacktrace_seek_start( 30 ) = %u\n", &stacktrace_seek_start );
  stacktrace_seek_start( 30 );  
}


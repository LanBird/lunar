#ifndef _LUNAR_TEST_H
#define _LUNAR_TEST_H

#include <stdarg.h>

struct test_info {
  char * name;
  char * filename;
  void (* set_up)();
  void (* run)();
  void (* tear_down)();
  int errors;
  int warnings;
};

void test_printf( const char * message, ... );
void test_warning( const char * message );
void test_error( const char * message );

void test_run( struct test_info * test );

#endif


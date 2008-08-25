#ifndef _LUNAR_TEST_H
#define _LUNAR_TEST_H

struct test_info {
  char * name;
  char * filename;
  int (* set_up)();
  int (* run)();
  int (* tear_down)();
  int state;
};

int test_fail();
int test_success();

int test_output();
int test_require( struct test_info * test );

int test_run( struct test_info * test );

#endif


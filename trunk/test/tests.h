#include "test.h"

extern struct test_info test_database;
extern struct test_info test_intmath;
extern struct test_info test_sha1;

struct test_info * tests[4] = {
  &test_database,
  &test_intmath,
  &test_sha1,
  0
};

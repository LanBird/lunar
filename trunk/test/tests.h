#include "test.h"

extern struct test_info test_storage;
extern struct test_info test_intmath;
extern struct test_info test_sha1;

struct test_info * tests[4] = {
  &test_storage,
  &test_intmath,
  &test_sha1,
  0
};

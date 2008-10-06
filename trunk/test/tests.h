#include "test.h"

extern struct test_info test_storage;
extern struct test_info test_intmath;
extern struct test_info test_sha1;
extern struct test_info test_rtctl;

struct test_info * tests[5] = {
  &test_storage,
  &test_intmath,
  &test_sha1,
  &test_rtctl,
  0
};

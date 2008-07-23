#include "test.h"
#include "stacktrace.h"

int stacktrace_test();

struct test_info test_stacktrace = {
  "Stacktrace",
  __FILE__,
  0,
  stacktrace_test,
  0,
  0
};

int stacktrace_test() {
  return 1;
}
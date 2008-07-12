#ifndef _LUNAR_TARGET_H
#define _LUNAR_TARGET_H

#include "types.h"

// TARGET FLAGS:
//   1 = up to date      (target doesn't need an update)
//   2 = force rebuild   (always rebuild the target)
//   4 = skip            (mark as build but don't touch)
//   8 = failed          (build of the target has failed)
//  16 = unhashed        (lazy hashing)
//  32 =                 (reserved for future use)
//  64 = user 1          (freely usable flag)
// 128 = user 2          (freely usable flag)
extern byte_t TARGET_FLAG_UP_TO_DATE;
extern byte_t TARGET_FLAG_FORCE_REBUILD;
extern byte_t TARGET_FLAG_SKIP;
extern byte_t TARGET_FLAG_FAILED;
extern byte_t TARGET_FLAG_UNHASHED;
// extern byte_t TARGET_FLAG_UNUSED;
extern byte_t TARGET_FLAG_USER_1;
extern byte_t TARGET_FLAG_USER_2;

typedef uint64_t targetref_t;

struct target {
  targetref_t   reference;
  char        * name;
  byte_t        hash[5];
  byte_t        flags;
  fileref_t     file;
  targetref_t * dependencies;
  fileref_t   * input_files;
  fileref_t   * output_files;
};

#endif
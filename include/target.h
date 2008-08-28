#ifndef _LUNAR_TARGET_H
#define _LUNAR_TARGET_H

#include <stdint.h>
#include "file.h"

// TARGET FLAGS:
//   1 = up to date      (target doesn't need an update)
//   2 = force rebuild   (always rebuild the target)
//   4 = skip            (mark as build but don't touch)
//   8 = failed          (build of the target has failed)
//  16 = unhashed        (lazy hashing)
//  32 =                 (reserved for future use)
//  64 = user 1          (freely usable flag)
// 128 = user 2          (freely usable flag)
extern uint8_t TARGET_FLAG_UP_TO_DATE;
extern uint8_t TARGET_FLAG_FORCE_REBUILD;
extern uint8_t TARGET_FLAG_SKIP;
extern uint8_t TARGET_FLAG_FAILED;
extern uint8_t TARGET_FLAG_UNHASHED;
extern uint8_t TARGET_FLAG_UNUSED;
extern uint8_t TARGET_FLAG_USER_1;
extern uint8_t TARGET_FLAG_USER_2;

struct target_info {
  char   * name;
  uint8_t  hash[5];
  uint8_t  flags;
  struct file_info   * file;
  struct target_info * dependencies;
  struct file_info   * input_files;
  struct file_info   * output_files;
};

#endif


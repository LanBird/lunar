#ifndef _LUNAR_FILE_H
#define _LUNAR_FILE_H

#include "types.h"

// FILE FLAGS:
//   1 = up to date      (file doesn't need an update)
//   2 = force update    (always update the file)
//   4 = skip            (mark as updated but don't touch)
//   8 = non existent    (file does not exist)
//  16 = unhashed        (lazy hashing)
//  32 =                 (reserved for future use)
//  64 = user 1          (freely usable flag)
// 128 = user 2          (freely usable flag)
extern byte_t FILE_FLAG_UP_TO_DATE;
extern byte_t FILE_FLAG_FORCE_UPDATE;
extern byte_t FILE_FLAG_SKIP;
extern byte_t FILE_FLAG_NON_EXISTENT;
extern byte_t FILE_FLAG_UNHASHED;
// extern byte_t FILE_FLAG_UNUSED;
extern byte_t FILE_FLAG_USER_1;
extern byte_t FILE_FLAG_USER_2;

struct file_info {
  char *    name;
  byte_t    hash[5];
  byte_t    flags;
  date_t    modified_date;
  uint64_t  size;
};

// Create a new file record and initialize it.
struct file_info * file_create( char * filename );

#endif


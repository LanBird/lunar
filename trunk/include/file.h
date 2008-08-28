#ifndef _LUNAR_FILE_H
#define _LUNAR_FILE_H

#include <stdint.h>

// FILE FLAGS:
//   1 = up to date      (file doesn't need an update)
//   2 = force update    (always update the file)
//   4 = skip            (mark as updated but don't touch)
//   8 = non existent    (file does not exist)
//  16 = unhashed        (lazy hashing)
//  32 =                 (reserved for future use)
//  64 = user 1          (freely usable flag)
// 128 = user 2          (freely usable flag)
extern uint8_t FILE_FLAG_UP_TO_DATE;
extern uint8_t FILE_FLAG_FORCE_UPDATE;
extern uint8_t FILE_FLAG_SKIP;
extern uint8_t FILE_FLAG_NON_EXISTENT;
extern uint8_t FILE_FLAG_UNHASHED;
// extern uint8_t FILE_FLAG_UNUSED;
extern uint8_t FILE_FLAG_USER_1;
extern uint8_t FILE_FLAG_USER_2;

struct file_info {
  char *    name;
  uint8_t   hash[5];
  uint8_t   flags;
  //date_t  modified_date;
  uint64_t  size;
};

// Create a new file record and initialize it.
struct file_info * file_create( char * filename );

#endif


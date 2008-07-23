#include "file.h"
#include <stdlib.h>

byte_t FILE_FLAG_UP_TO_DATE   = 0x01;
byte_t FILE_FLAG_FORCE_UPDATE = 0x02;
byte_t FILE_FLAG_SKIP         = 0x04;
byte_t FILE_FLAG_NON_EXISTENT = 0x08;
byte_t FILE_FLAG_UNHASHED     = 0x10;
// byte_t FILE_FLAG_UNUSED       = 0x20;
byte_t FILE_FLAG_USER_1       = 0x40;
byte_t FILE_FLAG_USER_2       = 0x80;

struct file_info * file_create( char * filename ) {
  struct file_info * f = (struct file_info * ) malloc( sizeof( struct file_info ) );
  f->flags = FILE_FLAG_NON_EXISTENT | FILE_FLAG_UNHASHED;
  // create hash -> reference from filename
  // try to open file, unset FILE_FLAG_NON_EXISTENT
  // set modified_date
  // set size
  return f;
}
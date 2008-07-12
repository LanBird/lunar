#include "target.h"

byte_t TARGET_FLAG_UP_TO_DATE    = 0x01;
byte_t TARGET_FLAG_FORCE_REBUILD = 0x02;
byte_t TARGET_FLAG_SKIP          = 0x04;
byte_t TARGET_FLAG_FAILED        = 0x08;
byte_t TARGET_FLAG_UNHASHED      = 0x10;
// byte_t TARGET_FLAG_UNUSED        = 0x20;
byte_t TARGET_FLAG_USER_1        = 0x40;
byte_t TARGET_FLAG_USER_2        = 0x80;

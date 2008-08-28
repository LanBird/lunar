#include "target.h"

uint8_t TARGET_FLAG_UP_TO_DATE    = 0x01;
uint8_t TARGET_FLAG_FORCE_REBUILD = 0x02;
uint8_t TARGET_FLAG_SKIP          = 0x04;
uint8_t TARGET_FLAG_FAILED        = 0x08;
uint8_t TARGET_FLAG_UNHASHED      = 0x10;
// uint8_t TARGET_FLAG_UNUSED        = 0x20;
uint8_t TARGET_FLAG_USER_1        = 0x40;
uint8_t TARGET_FLAG_USER_2        = 0x80;

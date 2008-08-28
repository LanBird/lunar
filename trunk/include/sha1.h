#ifndef _LUNAR_SHA1_H
#define _LUNAR_SHA1_H

#include <stdint.h>

int sha1( const uint8_t * buffer, const uint64_t length, uint32_t h[] );

#endif


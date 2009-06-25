#include <stdint.h>

#include "test.h"
#include "sha1.h"

void sha1_test();

struct test_info test_sha1 = {
  "SHA-1",
  __FILE__,
  0,
  sha1_test,
  0,
  0
};

void sha1_test() {
  uint32_t x[5] = { 0x84983e44, 0x1c3bd26e, 0xbaae4aa1, 0xf95129e5, 0xe54670f1 };
  uint32_t y[5] = { 0xda39a3ee, 0x5e6b4b0d, 0x3255bfef, 0x95601890, 0xafd80709 };
  uint32_t h[5];
  
  test_print( "multiblock message digest" );
  sha1_bytes( h, (uint8_t *) "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 448/8 );
  if( x[0] != h[0] || x[1] != h[1] || x[2] != h[2] || x[3] != h[3] || x[4] != h[4] ) {
    test_error( "Sha1 computed invalid checksum for \"abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq\"!" );
  }

  test_print( "empty message digest" );
  sha1_bytes( h, (uint8_t *) "", 0 );
  if( y[0] != h[0] || y[1] != h[1] || y[2] != h[2] || y[3] != h[3] || y[4] != h[4] ) {
    test_error( "Sha1 computed invalid checksum for \"\"!" );
  }
}


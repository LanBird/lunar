#include "test.h"
#include "types.h"
#include "sha1.h"

int sha1_test();

struct test_info test_sha1 = {
  "SHA-1",
  __FILE__,
  0,
  sha1_test,
  0,
  0
};

int sha1_test() {
  int      errors = 0;
  uint32_t x[5] = { 0x84983e44, 0x1c3bd26e, 0xbaae4aa1, 0xf95129e5, 0xe54670f1 };
  uint32_t y[5] = { 0xda39a3ee, 0x5e6b4b0d, 0x3255bfef, 0x95601890, 0xafd80709 };
  uint32_t h[5];
  
  sha1( (byte_t *) "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 448/8, h );
  if( x[0] != h[0] || x[1] != h[1] || x[2] != h[2] || x[3] != h[3] || x[4] != h[4] ) {
    errors++;
  }

  sha1( (byte_t *) "", 0, h );
  if( y[0] != h[0] || y[1] != h[1] || y[2] != h[2] || y[3] != h[3] || y[4] != h[4] ) {
    errors++;
  }
  
  return errors;
}
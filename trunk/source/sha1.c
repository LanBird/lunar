#include <stdlib.h>
#include <stdio.h>

#include "types.h"
#include "sha1.h"

int sha1( const byte_t * buffer, const uint64_t length, uint32_t h[] ) {
  h[0] = 0x67452301;
  h[1] = 0xefcdab89;
  h[2] = 0x98badcfe;
  h[3] = 0x10325476;
  h[4] = 0xc3d2e1f0;
  
  uint32_t* m = malloc( 16 * sizeof( uint32_t ) );
  long int  n = length;
  int i, j;
  do {
    if( n >= 64 ) {
      // we can fill the block with bytes from the buffer
      for( j=0; j<16; j++ ) {
        m[ j ] = ( m[ j ] << 8 ) | *(buffer++);
        m[ j ] = ( m[ j ] << 8 ) | *(buffer++);
        m[ j ] = ( m[ j ] << 8 ) | *(buffer++);
        m[ j ] = ( m[ j ] << 8 ) | *(buffer++);
      }
      n -= 64;
    } else {
      // we have to take care because the buffer ends in this block
      // (or the size has to be placed in this buffer)
      j = 0;
      // any bytes left in the buffer wille be written to the block
      for( i=0; i<n; i++ ) {
        j = i / 4;
        m[ j ] = ( m[ j ] << 8 ) | *(buffer++);
      }
      n = n - i;
      // after the last byte is written the mandatory 0x80 is written
      // to the block (this may be the begin of the block if the
      // buffer endet on a block boundary)
      if( n == 0 ) {
        j = i / 4;
        m[ j ] = m[ j ] << 8 | 0x80;
        // shift the integer so that it is properly aligned
        m[ j ] = m[ j ] << ((4-((i+1)%4)) * 8);
        n = n - 1;
        j++;
      }
      // fill the block with zeros
      for( ; j<14; j++ ) {
        m[ j ] = 0;
      }
      // place the size to the end of the block or continue the zerofill
      // for buffers that left the last integer empty
      if( j == 14 ) {
        m[ 14 ] = length >> 29;
        m[ 15 ] = length << 3;
        n = n - 8;
      } else if( j == 15 ) {
        m[ 15 ] = 0;
      }
    }
    // m now contains a valid sha1 block
    
    {
      int f;
      int a = h[0];
      int b = h[1];
      int c = h[2];
      int d = h[3];
      int e = h[4];
      
      // the two loops explained in fips180 can be combined to one
      // and the size of w (here: m) can be reduced to 16
      for( i=0; i<80; i++ ) {
        if( i>59 ) {
          m[ i%16 ] ^= m[ (i-3)%16 ] ^ m[ (i-8)%16 ] ^ m[ (i-14)%16 ];
          m[ i%16 ] = (m[ i%16 ] << 1) | (m[ i%16 ] >> 31);
          f = ( b ^ c ^ d ) + 0xca62c1d6;
        } else if( i>39 ) {
          m[ i%16 ] ^= m[ (i-3)%16 ] ^ m[ (i-8)%16 ] ^ m[ (i-14)%16 ];
          m[ i%16 ] = (m[ i%16 ] << 1) | (m[ i%16 ] >> 31);
          f = ( (b & c) | (b & d) | (c & d) ) + 0x8f1bbcdc;
        } else if( i>19 ) {
          m[ i%16 ] ^= m[ (i-3)%16 ] ^ m[ (i-8)%16 ] ^ m[ (i-14)%16 ];
          m[ i%16 ] = (m[ i%16 ] << 1) | (m[ i%16 ] >> 31);
          f = ( b ^ c ^ d ) + 0x6ed9eba1;
        } else if( i<16 ) {
          f = ( d ^ ( b & (c ^ d) ) ) + 0x5a827999;
        } else {
          m[ i%16 ] ^= m[ (i-3)%16 ] ^ m[ i-8 ] ^ m[ i-14 ];
          m[ i%16 ] = (m[ i%16 ] << 1) | (m[ i%16 ] >> 31);
          f = ( d ^ ( b & (c ^ d) ) ) + 0x5a827999;
        }
        // somehow we have to use the exact mask (0x1f). i have no idea
        // why it does not work out without it.
        f += e + m[i%16] + ( (a << 5) | ( (a >> 27) & 0x1f ) );
        e = d;
        d = c;
        c = (b << 30) | ( (b >> 2) & 0x3fffffff );
        b = a;
        a = f;
      }
            
      h[0] += a;
      h[1] += b;
      h[2] += c;
      h[3] += d;
      h[4] += e;
    }
  } while( n+9 > 0 );     

  free( m );
  
  return 0;
}

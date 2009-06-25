#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sha1.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// hold the context that is shared by all functions during a checksum computation
struct sha1_context {
  uint64_t length;  // original message length
  uint64_t fill;    // number of bits written to the last block
  uint32_t m[ 16 ]; // 512-bit message block
  uint32_t h[ 5 ];  // 160-bit digest
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialize the checksum-context
int sha1_context_init( struct sha1_context ** c ) {
  if( c == NULL ) {
    return -1;
  }
  *c = malloc( sizeof( struct sha1_context ) );
  //(*c)->m = malloc( 16 * sizeof( uint32_t ) );
  //(*c)->h = malloc(  5 * sizeof( uint32_t ) );

  (*c)->length = 0;
  (*c)->fill   = 0;

  (*c)->h[0] = 0x67452301;
  (*c)->h[1] = 0xefcdab89;
  (*c)->h[2] = 0x98badcfe;
  (*c)->h[3] = 0x10325476;
  (*c)->h[4] = 0xc3d2e1f0;

  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// free space consumed by checksum context
int sha1_context_free( struct sha1_context * c ) {
  //free( c->m );
  //free( c->h );
  free( c );
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// read bytes from a character string until '\0' is read or the block is filled
int sha1_read_string( struct sha1_context * c, const char * string ) {
  int      i;
  char     b;
  uint32_t t = 0;
  if( string == NULL ) {
    return -1;
  }
  for( i=0; i<64 && (b=*(string+i)) != '\0'; i++ ) {
    t = ( t << 8 ) | b;
    if( i%4 == 3 ) {
      c->m[ i/4 ] = t;
    }
  }
  if( i%4 != 0 ) {
    c->m[ i/4 ] = t << ((4-(i%4))*8);
  }
  c->fill    = 8*i;
  c->length += c->fill;
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// read bytes from a file until EOF is read or the block is filled
int sha1_read_file( struct sha1_context * c, FILE * file ) {
  int      i, b;
  uint32_t t = 0;
  if( file == NULL ) {
    return -1;
  }
  for( i=0; i<64 && (b=fgetc(file)) != EOF; i++ ) {
    t = ( t << 8 ) | (b&0xff);
    if( i%4 == 3 ) {
      c->m[ i/4 ] = t;
    }
  }
  if( i%4 != 0 ) {
    c->m[ i/4 ] = t << ((4-(i%4))*8);
  }
  c->fill    = i * 8;
  c->length += c->fill;
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// read bytes from a byte-buffer until either the given number of bytes has been read or the block is filled
int sha1_read_bytes( struct sha1_context * c, uint8_t * buffer, size_t bytes ) {
  int      i = 0;
  uint32_t t = 0;
  if( buffer == NULL ) {
    return -1;
  }
  while( i<16 && bytes-(i*4) >= 4 ) {
    t = ( t << 8 ) | *(buffer++);
    t = ( t << 8 ) | *(buffer++);
    t = ( t << 8 ) | *(buffer++);
    t = ( t << 8 ) | *(buffer++);
    c->m[ i ] = t;
    i++;
  }
  for( i=4*i; i<64 && i<bytes; i++ ) {
    t = ( t << 8 ) | *(buffer++);
  }
  if( i%4 != 0 ) {
    c->m[ i/4 ] = t << ((4-(i%4))*8);
  }
  c->fill      = i * 8;
  c->length   += c->fill;
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (if required) apply the following padding to a block:
// first bit after the message: 1
// following bits: 0
int sha1_padd_block( struct sha1_context * c ) {
  int w    = c->fill / 32;
  int bits = c->fill % 32;

  if( w < 16 ) {
    c->m[ w ] |= 0x80000000 >> bits;
    c->m[ w ] &= 0xffffffff << (31-bits);
    while( ++w < 16 ) {
      c->m[ w ] = 0;
    }
    return 0;
  }
  return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// zero-out block
int sha1_clear_block( struct sha1_context * c ) {
  int i;
  for( i=0; i<16; i++ ) {
    c->m[ i ] = 0;
  }
  c->fill = 0;
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// finalize block by writing size to the last 64 bits
int sha1_finalize_block( struct sha1_context * c ) {
  if( c->fill < 448 ) {
    c->m[ 14 ] = c->length >> 32 & 0xffffffff;
    c->m[ 15 ] = c->length       & 0xffffffff;
    return 0;
  }
  return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// compute the checksum of the current block and merge it with the checksum of the given context
int sha1_blocksum( struct sha1_context * cx ) {
  int f;
  int a = cx->h[0];
  int b = cx->h[1];
  int c = cx->h[2];
  int d = cx->h[3];
  int e = cx->h[4];

  int i;

  uint32_t * m = cx->m;
  
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
            
  cx->h[0] += a;
  cx->h[1] += b;
  cx->h[2] += c;
  cx->h[3] += d;
  cx->h[4] += e;

  cx->fill  = 512;

  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// compute the checksum of a given string
int sha1_string( uint32_t h[], const char * string ) {
  struct sha1_context * c;

  sha1_context_init( &c );
  sha1_read_string( c, string );
  while( c->fill == 512 ) {
    sha1_blocksum( c );
    sha1_read_string( c, string );
  }
  sha1_padd_block( c );
  if( c->fill >= 448 ) {
    sha1_blocksum( c );
    sha1_clear_block( c );
  }
  sha1_finalize_block( c );
  sha1_blocksum( c );
  h[ 0 ] = c->h[ 0 ];
  h[ 1 ] = c->h[ 1 ];
  h[ 2 ] = c->h[ 2 ];
  h[ 3 ] = c->h[ 3 ];
  h[ 4 ] = c->h[ 4 ];
  sha1_context_free( c );
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// compute the checksum of a given string
int sha1_file( uint32_t h[], FILE * file ) {
  struct sha1_context * c;

  sha1_context_init( &c );
  sha1_read_file( c, file );
  while( c->fill == 512 ) {
    sha1_blocksum( c );
    sha1_read_file( c, file );
  }
  sha1_padd_block( c );
  if( c->fill >= 448 ) {
    sha1_blocksum( c );
    sha1_clear_block( c );
  }
  sha1_finalize_block( c );
  sha1_blocksum( c );
  h[ 0 ] = c->h[ 0 ];
  h[ 1 ] = c->h[ 1 ];
  h[ 2 ] = c->h[ 2 ];
  h[ 3 ] = c->h[ 3 ];
  h[ 4 ] = c->h[ 4 ];
  sha1_context_free( c );
  return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// compute the checksum of a given string
int sha1_bytes( uint32_t h[], uint8_t * buffer, size_t bytes ) {
  struct sha1_context * c;

  sha1_context_init( &c );
  sha1_read_bytes( c, buffer, bytes );
  while( c->fill == 512 ) {
    sha1_blocksum( c );
    sha1_read_bytes( c, buffer, bytes-(c->length/8) );
  }
  sha1_padd_block( c );
  if( c->fill >= 448 ) {
    sha1_blocksum( c );
    sha1_clear_block( c );
  }
  sha1_finalize_block( c );
  sha1_blocksum( c );
  h[ 0 ] = c->h[ 0 ];
  h[ 1 ] = c->h[ 1 ];
  h[ 2 ] = c->h[ 2 ];
  h[ 3 ] = c->h[ 3 ];
  h[ 4 ] = c->h[ 4 ];
  sha1_context_free( c );
  return 0;
}

int sha1_context_print( struct sha1_context * c ) {
  int i, b;
  if( c == NULL ) {
    printf( "context = (NULL)\n" );
    return -1;
  }
  printf( "context = {\n  length = %llu\n  fill   = %llu\n",
          c->length, c->fill );
  for( i=0; i<16; i++ ) {
    printf( "  m[%3i] = %x", i, c->m[ i ] );
    if( c->fill < 448 && i >=14 && c->m[ i ] != 0 ) {
      printf( " (%i)\n", c->m[ i ] );
    } else if( c->m[ i ] != 0 ) { 
      printf( " (\"" );
      b = (c->m[ i ]>>24) & 0xff;
      printf( "%c", ( b < 32 || b > 126 ) ? ' ' : b );
      b = (c->m[ i ]>>16) & 0x7f;
      printf( "%c", ( b < 32 || b > 126 ) ? ' ' : b );
      b = (c->m[ i ]>>8 ) & 0x7f;
      printf( "%c", ( b < 32 || b > 126 ) ? ' ' : b );
      b = (c->m[ i ]    ) & 0x7f;
      printf( "%c", ( b < 32 || b > 126 ) ? ' ' : b );
      printf( "\")\n" );
    } else {
      printf( "\n" );
    }
  }
  printf( "  h      = %8x%8x%8x%8x%8x\n}\n", c->h[0], c->h[1], c->h[2], c->h[3], c->h[4] );
  return 0;
}

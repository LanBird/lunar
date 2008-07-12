#include <stdlib.h>
#include <stdio.h>

/* Include the Lua API header files. */
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "types.h"
#include "sha1.h"
#include "intmath.h"
#include "database.h"
#include "client.h"
#include "server.h"
#include "lunar.h"

// Intended use of this file:
// - parse configuration
// - parse build-scripts
// - start a build client
// - depending on the configuration (if localhost is used as build server and
//   there is no build server running) start an embedded build server
// - send targets that need to be built to the build servers and collect built
//   files
// - collect targets, etc.

// Declare the Lua libraries we wish to use.
// Note: If you are opening and running a file containing Lua code
// using 'lua_dofile(l, "myfile.lua") - you must delcare all the libraries
// used in that file here also.
static const luaL_reg lualibs[] = {
{ "base",  luaopen_base },
{ NULL,    NULL }
};


// A function to open up all the Lua libraries you declared above.
void openlualibs(lua_State *l) {
  const luaL_reg *lib;
  
  for (lib = lualibs; lib->func != NULL; lib++) {
    lib->func(l);
    lua_settop(l, 0);
  }
}

struct test_struct_1 {
  char t[3];
};

struct test_struct_2 {
  char t[7];
};

int main(void) {

  /* Declare a Lua State, open the Lua State and load the libraries (see above). */
  lua_State *l;
  l = lua_open();
  openlualibs(l);

  /* You can do what you want here. Note: Remember to update the libraries used (see above) */
  /* if you add to your program and use new Lua libraries. */
  /* In the lines below, I load and run the Lua code contained in the file */
  /* "script.lua". */
  /* Plus print some text directly from C. */
  printf("This line is directly from C\n\n");
  luaL_dofile(l, "lunar.lua");
  printf("\nBack to C again\n\n");

  /* Remember to destroy the Lua State */
  lua_close(l);
    
  uint32_t e[5] = { 0x84983e44, 0x1c3bd26e, 0xbaae4aa1, 0xf95129e5, 0xe54670f1 };
  uint32_t h[5];
  
  printf( "SHA-1 test\n" );
  sha1( (byte_t *) "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 448/8, h );
  printf( "expected: %x%x%x%x%x\n", e[0], e[1], e[2], e[3], e[4] );
  printf( "result:   %x%x%x%x%x\n", h[0], h[1], h[2], h[3], h[4] );
  if( e[0] == h[0] && e[1] == h[1] && e[2] == h[2] && e[3] == h[3] && e[4] == h[4] ) {
    printf( "[success]\n" );
  } else {
    printf( "[failed]\n" ); 
  }
  
  uint32_t i = h[1];
  uint32_t x = 0;
  uint32_t y = 0;
  
  printf( "BINARY test\n" );
  int j;
  for( j=7; j<25; j++ ) {
    uint32_t m = 0xffffffff >> j;
    x += ( i >> j & m ) | i << (32-j);
    y += i >> j | i << (32-j);
  }
  printf( "expected: %x\n", y );
  printf( "result:   %x\n", x );
  if( x == y ) {
    printf( "[success]\n" );
  } else {
    printf( "[failed]\n" );
  }
  
  printf( "STRUCT test\n" );
  printf( "sizeof( test_struct_1 ): %i\n", sizeof( struct test_struct_1 ) );
  printf( "sizeof( test_struct_2 ): %i\n", sizeof( struct test_struct_2 ) );

  printf( "LOG2 test\n" );
  i=1;
  for( j=0; j<8; j++ ) {
    i=i<<1;
    printf( "log2( %i ): %i\n", i-1, intmath_log2( i-1 ) );
    printf( "log2( %i ): %i\n", i, intmath_log2( i ) );
  }
  if( intmath_log2( 0x8000000000000000ll ) == 63 ) {
    printf( "large number test: OK\n" );
    printf( "log2( %llu ): %llu\n", 0x8000000000000000ll, intmath_log2( 0x8000000000000000ll ) );
  } else {
    printf( "large number test: FAILED\n" );
    printf( "log2( %llu ): %llu\n", 0x8000000000000000ll, intmath_log2( 0x8000000000000000ll ) );
  }
  
  printf( "DATABASE test\n" );
  struct table_info * t = database_new_table( 2, 10 );
  
  return 0;
}
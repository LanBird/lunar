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

  return 0;
}
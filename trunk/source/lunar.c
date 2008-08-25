#include <stdlib.h>
#include <stdio.h>

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

int main(void) {
  return 0;
}


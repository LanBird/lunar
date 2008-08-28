#include <stdio.h>
#include <stdint.h>

#include "types.h"
#include "server.h"
#include "lunard.h"

// this will just start a build-server and hand all control to it

int main(void) {
  printf( "lunar daemon started\n" );
  while( 1 ) {
    printf( "." );
    fflush( stdout );
  }
  return 0;
}


#include <unistd.h>
#include <stdio.h>

#include "types.h"
#include "server.h"
#include "lunard.h"

// this will just start a build-server and hand all control to it

int main(void) {
  printf( "lunar daemon started\n" );
  while( 1 ) {
    printf( "." );
    fflush( stdout );
    sleep( 1 );
  }
  return 0;
}


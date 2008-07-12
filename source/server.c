#include "server.h"

// build server

// Lua is just used for configuration.
// All control is done by the client.


// Basic execution scheme
// ----------------------


// - startup
// - load global configuration
// - wait for incoming connections
//   - connect client (new thread)
//   - exclusive access to selected project
//   - start build loop
//     - process incoming requests
//       a) add target and add notification hook to prerequisites
//       b) finish build
//     - check for build-ready targets
//       - start build of target (new thread)
//       - notify dependant targets
//   - send summary to client
//   - let client collect created files
//   - disconnect client

// transmission format of build targets
target {
  file
  name
  inputs[]
  outputs[]
}

// transmission format of files
file {
  directory
  name
  hash
  date
}


  
// REQ_BUILD
// path/to/directory
// build.lua
// targetname
// 4a1345faa3b624fe768deb35 include/inputfile1.h
// 1345fadeb35a3b64a24fe768 source/inputfile1.c
// 4a1345faa3b624fe768deb35 include/inputfile2.h
// 1345fadeb35a3b64a24fe768 source/inputfile2.c
// a3a1345fa6b62448deb35fe7 bin/product1.o
// fe748deb35a6b624fa3a1345 bin/product1
// fa64fe74a3a13458deb35b62 bin/product2
#include "client.h"

#include <stdint.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>

#include "storage.h"
#include "database.h"

struct client_info {
  clientid_t uid;     // unique client identifier
  pthread_t  thread;  // only if client is associated with a local thread
  time_t     timeout; // if later than timeout: kick client
  table_t    files;   // all files the client has (= all file we sent him)
  struct sockaddr_in host; // the host from which the client connected
};


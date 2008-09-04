#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

struct storage_info {
  size_t size;
  void * data;
  pthread_mutex_t access;    // has to be locked when accessing struct
  pthread_cond_t  available; // locks reached 0
  int locks;                 // -1: exclusive, 0: not locked, >0: shared
  int waiting;               // number of threads waiting for free condition
};

#include "test.h"
#include "storage.h"

int storage_set_up();
int storage_test();
int storage_tear_down();

struct test_info test_storage= {
  "Storage",
  __FILE__,
  storage_set_up,
  storage_test,
  storage_tear_down,
  0
};

int finish = 0;
int writes = 0;
int shares = 0;
storage_t shared_storage;
pthread_t thread1, thread2, thread3;
char valid1[512] = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus e";
char valid2[512] = "Passersby were amazed by the unusually large amounts of blood. Passersby were amazed by the unusually large amounts of blood. Passersby were amazed by the unusually large amounts of blood. Passersby were amazed by the unusually large amounts of blood. Passersby were amazed by the unusually large amounts of blood. Passersby were amazed by the unusually large amounts of blood. Passersby were amazed by the unusually large amounts of blood. Passersby were amazed by the unusually large amounts of blood. Or not??";
char buffer[512];

void * storage_writer( void * data ) {
  while( finish == 0 ) {
    storage_write( shared_storage, 0, data, 512 );
    writes++;
  }
  return data;
}

void * storage_reader( void * data ) {
  while( finish == 0 ) {
    storage_read_lock( shared_storage );
    if( shared_storage->locks > 1 ) {
      shares++;
    }
    storage_read_unlock( shared_storage );
  }
  return data;
}

int storage_set_up() {
  shared_storage = storage_new();
  storage_alloc( shared_storage, 512 );
  storage_write( shared_storage, 0, valid1, 512 );
  pthread_create( &thread1, NULL, &storage_writer, (void *) valid1 );
  pthread_create( &thread2, NULL, &storage_writer, (void *) valid2 );
  pthread_create( &thread3, NULL, &storage_reader, NULL );
  sleep( 1 );
  return 0;
}

int storage_test() {
  int errors = 0;
  int i;
  int data1 = 0;
  int data2 = 0;

  for( i=0; i<2048; i++ ) {
    storage_read( shared_storage, 0, buffer, 512 );
    if( strncmp( buffer, valid1, 512 ) == 0 ) {
      data1++;
    } else if( strncmp( buffer, valid2, 512 ) == 0 ) {
      data2++;
    } else {
      errors++;
    }
  }

  if( writes < 5 || shares < 5 ) {
    errors++; // maybe deadlocked?
  }

  return errors;
}

int storage_tear_down() {
  finish = 1;
  if( writes < 5 || shares < 5 ) {
    pthread_kill( thread1, 9 );
    pthread_kill( thread2, 9 );
    pthread_kill( thread3, 9 );
  } else {
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
    pthread_join( thread3, NULL );
  }
  return 0;
}


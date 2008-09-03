#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "storage.h"

/**
 * The storage object is used where multiple threads share one resource and
 * provides locking methods for data while automatically managing locks for
 * structural changes.
 */
struct storage_info {
  size_t size;
  void * data;
  pthread_mutex_t read;      // read lock (unlock causes signal on access)
  pthread_mutex_t write;     // write lock (unlock causes broadcast on access)
  pthread_mutex_t structure; // structural lock (for changes other than data)
  pthread_cond_t  access;    // access condition
  int reading;
  int waiting;
};

storage_t storage_new( void ) {
  struct storage_info * storage;
  storage = (struct storage_info *) malloc( sizeof( struct storage_info ) );
  if( storage == NULL ) {
    return NULL;
  }
  storage->size = 0;
  storage->data = NULL;
  pthread_mutex_init( &storage->read, NULL );
  pthread_mutex_init( &storage->write, NULL );
  pthread_mutex_init( &storage->structure, NULL );
  pthread_cond_init( &storage->access, NULL );
  storage->reading = 0;
  storage->waiting = 0;
  return storage;
}

int storage_alloc( storage_t storage, size_t size ) {
  void * old_data = storage->data;
  storage->data = malloc( size );
  if( storage->data == NULL ) {
    storage->data = old_data;
    return 1;
  }
  if( old_data != NULL ) {
    free( old_data );
  }
  storage->size = size;
  return 0;
}

int storage_free( storage_t storage ) {
  free( storage->data );
  storage->size = 0;
  return 0;
}

int storage_realloc( storage_t storage, size_t size ) {
  storage->data = realloc( storage->data, size );
  storage->size = size;
  return 0;
}

int storage_write( storage_t storage, size_t start, void * buffer, size_t bytes ) {
  storage_write_lock( storage );
  memcpy( storage->data + start, buffer, bytes );
  storage_write_unlock( storage );
  return 0;
}

int storage_read( storage_t storage, size_t start, void * buffer, size_t bytes ) {
  storage_read_lock( storage );
  memcpy( buffer, storage->data + start, bytes );
  storage_read_unlock( storage );
  return 0;
}

int storage_clone( storage_t destination, storage_t source ) {
  return storage_copy( destination, 0, source, 0, source->size );
}

int storage_copy( storage_t destination, size_t d_start,
                  storage_t source, size_t s_start, size_t bytes ) {
  int r = 0;
  storage_read_lock( source );
  r = storage_write( destination, d_start, source->data + s_start, bytes );
  storage_read_unlock( source );
  return r;
}

/**
 * Prerequisite locks: none
 * Used locks: structure, read, write
 * Acquired locks on exit: write
 */
int storage_write_lock( storage_t storage ) {
  pthread_mutex_lock( &storage->structure );
  pthread_mutex_lock( &storage->write );
  while( pthread_mutex_trylock( &storage->read ) == EBUSY ) {
    storage->waiting++;
    pthread_cond_wait( &storage->access, &storage->structure );
    storage->waiting--;
  }
  pthread_mutex_unlock( &storage->structure );
  return 0;
}

/**
 * Prerequisite locks: none
 * Used locks: structure, write, read
 * Acquired locks on successful exit: write
 * Acquired locks on failure: none
 */
int storage_write_trylock( storage_t storage ) {
  if( pthread_mutex_trylock( &storage->structure ) == EBUSY ) {
    return EBUSY;
  }
  if( pthread_mutex_trylock( &storage->write ) == EBUSY ) {
    pthread_mutex_unlock( &storage->structure );
    return EBUSY;
  }
  if( pthread_mutex_trylock( &storage->read ) == EBUSY ) {
    pthread_mutex_unlock( &storage->write );
    pthread_mutex_unlock( &storage->structure );
    return EBUSY;
  }
  pthread_mutex_unlock( &storage->structure );
  return 0;
}

/**
 * Prerequisite locks: write
 * Used locks: structure
 * Acquired locks on exit: none
 */
int storage_write_unlock( storage_t storage ) {
  pthread_mutex_lock( &storage->structure );
  pthread_mutex_unlock( &storage->write );
  if( storage->waiting > 0 ) {
    pthread_cond_broadcast( &storage->access );
  }
  pthread_mutex_unlock( &storage->structure );
  return 0;
}

/**
 * Prerequisite locks: none
 * Used locks: structure, read, write
 * Acquired locks on exit: read
 */
int storage_read_lock( storage_t storage ) {
  pthread_mutex_lock( &storage->structure );
  pthread_mutex_lock( &storage->write );
  pthread_mutex_lock( &storage->read );
  storage->reading++;
  pthread_mutex_unlock( &storage->write );
  pthread_mutex_unlock( &storage->structure );
  return 0;
}

/**
 * Prerequisite locks: none
 * Used locks: structure, write, read
 * Acquired locks on successful exit: read
 * Acquired locks on failure: none
 */
int storage_read_trylock( storage_t storage ) {
  if( pthread_mutex_trylock( &storage->structure ) == EBUSY ) {
    return EBUSY;
  }
  if( pthread_mutex_trylock( &storage->write ) == EBUSY ) {
    pthread_mutex_unlock( &storage->structure );
    return EBUSY;
  }
  storage->reading++;
  pthread_mutex_unlock( &storage->write );
  pthread_mutex_unlock( &storage->structure );
  return 0;
}

/**
 * Prerequisite locks: read
 * Used locks: structure
 * Acquired locks on exit: none
 */
int storage_read_unlock( storage_t storage ) {
  pthread_mutex_lock( &storage->structure );
  storage->reading--;
  pthread_mutex_unlock( &storage->read );
  if( storage->waiting > 0 ) {
    pthread_cond_signal( &storage->access );
  }
  pthread_mutex_unlock( &storage->structure );
  return 0;
}


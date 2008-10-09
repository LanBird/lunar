#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "rtctl.h"
#include "storage.h"

size_t storage_max_size   = 1024;
int    storage_auto_chain = 1;

struct rtctl_info storage_max_size_rtctl = RTCTL_INTEGER_INITIALIZER;
struct rtctl_info storage_auto_chain_rtctl = RTCTL_BOOLEAN_INITIALIZER;

/**
 * Initialize the storage module:
 *  - create rtctl namespace
 *  - register rtctl hooks
 */
void storage_init( void ) {
  rtctl_register( &storage_max_size_rtctl, "storage.max_size" );
  rtctl_register( &storage_auto_chain_rtctl, "storage.auto_chain" );
}

/**
 * The storage object is used where multiple threads share one resource and
 * provides locking methods for data while automatically managing locks for
 * structural changes.
 */
struct storage_info {
  storage_t next;            // if the size becomes to big we can chain storages ..
  void * data;               // load
  size_t size;               // size of the storage block
  pthread_mutex_t access;    // has to be locked when accessing struct
  pthread_cond_t  available; // locks reached 0
  int locks;                 // -1: exclusive, 0: not locked, >0: shared
  int waiting;               // number of threads waiting for free condition
};

/**
 * Allocate a new storage_info structure and return a pointer to the
 * initialized struct as storage_t.
 * @return a pointer (storage_t) to the new storage.
 */
storage_t storage_new( void ) {
  struct storage_info * storage;
  storage = (struct storage_info *) malloc( sizeof( struct storage_info ) );
  if( storage == NULL ) {
    return NULL;
  }
  storage->next  = NULL;
  storage->data  = NULL;
  storage->size  = 0;
  pthread_mutex_init( &storage->access, NULL );
  pthread_cond_init( &storage->available, NULL );
  storage->locks = 0;
  storage->waiting = 0;
  return storage;
}

/**
 * Allocate memory for the storage's load. If the storage already has data
 * the old data is free'd.
 * @param storage the storage to carry the memory.
 * @param size the number of bytes to be allocated.
 * @return an errorcode or 0 on success.
 */
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

/**
 * Free storage's memory.
 * @param storage the storage to be free'd.
 */
int storage_free( storage_t storage ) {
  storage_t next, delete;
  
  storage_write_lock( storage );  
  storage->size = 0;
  next = storage->next;
  storage->next = NULL;
  storage_write_unlock( storage );

  while( next != NULL ) {
    storage_write_lock( next );  
    free( next->data );
    delete = next;
    next   = next->next;
    free( delete );
  }  
  return 0;
}

/**
 * Reallocate memory for the storage's load.
 * @param storage the storage to carry the memory.
 * @param size the number of bytes to be allocated.
 * @return an errorcode or 0 on success.
 */
int storage_realloc( storage_t storage, size_t size ) {
  storage_write_lock( storage );
  realloc( storage->data, size );
  storage->size = size;
  storage_write_unlock( storage );
  return 0;
}

/**
 * Write bytes bytes from buffer to start in storage.
 * @param storage the storage to be written to.
 * @param start the position where the write should be performed.
 * @param buffer the source from which the input data should be taken.
 * @param bytes the number of bytes to be written.
 */
int storage_write( storage_t storage, size_t start, void * buffer, size_t bytes ) {
  storage_write_lock( storage );
  memcpy( storage->data + start, buffer, bytes );
  storage_write_unlock( storage );
  return 0;
}

/**
 * Read bytes bytes from start in storage to buffer.
 * @param storage the storage to be read from.
 * @param start the position from which the read should be started.
 * @param buffer the target to which the read bytes should be written.
 * @param bytes the number of bytes to be read.
 */
int storage_read( storage_t storage, size_t start, void * buffer, size_t bytes ) {
  storage_read_lock( storage );
  memcpy( buffer, storage->data + start, bytes );
  storage_read_unlock( storage );
  return 0;
}

/**
 * Copy all data from one storage to another. Resize the destinantion
 * storage appropiately.
 * @param destination the "clone"
 * @param source the original storage where data should be copied from.
 */
int storage_clone( storage_t destination, storage_t source ) {
  storage_realloc( destination, source->size );
  return storage_copy( destination, 0, source, 0, source->size );
}

/**
 * Copy bytes bytes from s_start in source to d_start in destination.
 * @param destination the storage to write to.
 * @param d_start the position to write to.
 * @param source the storage to  read from.
 * @param s_start the position to start reading.
 * @param bytes the number of bytes to read.
 */
int storage_copy( storage_t destination, size_t d_start,
                  storage_t source, size_t s_start, size_t bytes ) {
  int r = 0;
  storage_read_lock( source );
  r = storage_write( destination, d_start, source->data + s_start, bytes );
  storage_read_unlock( source );
  return r;
}

/**
 * Lock the storage exclusively (for writing).
 * @param storage the storage to be locked.
 */
int storage_write_lock( storage_t storage ) {
  pthread_mutex_lock( &storage->access );
  while( storage->locks != 0 ) {
    storage->waiting++;
    pthread_cond_wait( &storage->available, &storage->access );
    storage->waiting--;
  }
  storage->locks = -1;
  pthread_mutex_unlock( &storage->access );
  return 0;
}

/**
 * Try to lock the storage exclusively (for writing).
 * @param storage the storage to be locked.
 * @return EBUSY if already locked. Zero otherwise.
 */
int storage_write_trylock( storage_t storage ) {
  if( pthread_mutex_trylock( &storage->access ) == EBUSY ) {
    return EBUSY;
  }
  if( storage->locks != 0 ) {
    pthread_mutex_unlock( &storage->access );
    return EBUSY;
  }
  storage->locks = -1;
  pthread_mutex_unlock( &storage->access );
  return 0;
}

/**
 * Revoke the exclusive lock.
 * @param storage the storage to be unlocked.
 */
int storage_write_unlock( storage_t storage ) {
  pthread_mutex_lock( &storage->access );
  storage->locks = 0;
  pthread_mutex_unlock( &storage->access );
  pthread_cond_signal( &storage->available );
  return 0;
}

/**
 * Lock the storage shared (for reading).
 * @param storage the storage to be locked.
 */
int storage_read_lock( storage_t storage ) {
  pthread_mutex_lock( &storage->access );
  while( storage->locks == -1 || storage->waiting > 0 ) {
    pthread_cond_wait( &storage->available, &storage->access );
  }
  storage->locks++;
  pthread_mutex_unlock( &storage->access );
  return 0;
}

/**
 * Try to lock the storage shared (for reading).
 * @param storage the storage to be locked.
 * @return EBUSY if the storage is locked exclusively or if the access lock
 * can't be acquired. Zero otherwise.
 */
int storage_read_trylock( storage_t storage ) {
  if( pthread_mutex_trylock( &storage->access ) == EBUSY ) {
    return EBUSY;
  }
  if( storage->locks == -1 ) {
    pthread_mutex_unlock( &storage->access );
    return EBUSY;
  }
  storage->locks++;
  pthread_mutex_unlock( &storage->access );
  return 0;
}

/**
 * Revoke one shared lock. Signal a waiting thread if there are no other locks
 * left.
 * @param storage the storage to be unlocked.
 */
int storage_read_unlock( storage_t storage ) {
  pthread_mutex_lock( &storage->access );
  storage->locks--;
  if( storage->locks == 0 ) {
    pthread_mutex_unlock( &storage->access );
    pthread_cond_signal( &storage->available );
  } else {
    pthread_mutex_unlock( &storage->access );
  }
  return 0;
}


/**
 * The storage module contains the implementation of a shared memory object
 * storage_t should be used for all objects that require asynchronous access.
 */
#ifndef _LUNAR_STORAGE_H
#define _LUNAR_STORAGE_H

#include <pthread.h>

typedef struct storage_info * storage_t;

storage_t storage_new();

int storage_alloc( storage_t storage, size_t size );
int storage_free( storage_t storage );
int storage_realloc( storage_t storage, size_t size );

int storage_write( storage_t storage, void * buffer, size_t bytes );
int storage_read( storage_t storage, void * buffer, size_t bytes );
int storage_copy( storage_t storage, storage_t source, size_t start, size_t bytes );
int storage_clone( storage_t storage, storage_t source );

int storage_write_lock( storage_t storage );
int storage_write_trylock( storage_t storage );
int storage_write_unlock( storage_t storage );

int storage_read_lock( storage_t storage );
int storage_read_trylock( storage_t storage );
int storage_read_unlock( storage_t storage );

#endif


#ifndef _LUNAR_DATABASE_H
#define _LUNAR_DATABASE_H

/**
 * The database module stores arbitary records in tables and provides a general
 * mechanism for concurrent access.
 */

#include <pthread.h>

#include "types.h"

/**
 * A table stores data linearly and uses a primary index (the first in the
 * index list) to reference data.
 * The database module keeps a chained list of tables to properly clean up on
 * shutdown or other events.
 */
struct table_info {
  struct table_info * next;  // the next table in the list
  struct index_info * index; // the primary index followed by a list of indices
  uint64_t item_size;        // size of each record in bytes
  uint64_t size;             // available space (measured in items)
  uint64_t items;            // current fill state
  pthread_rwlock_t rwlock;   // lock for read/write access
  uint64_t * data;           // the actual data
};

/**
 * An index dynamically maps generated keys (specified by a function) to the
 * keys of its table's primary index.
 * Primary indices store the row number of the record, secondary indices, while
 * (user indices) store the associated primary index value.
 */
struct index_info {
  struct index_info * next;      // the next index in the list
  struct index_info * reference; // the index used for referencing
  struct table_info * table;     // the table that the index belongs to
  uint64_t size;                 // available space (measured in items)
  uint64_t items;                // current fill state
  uint64_t (* function)(void *, uint64_t, int); // hashing function
  pthread_rwlock_t rwlock;       // lock for read/write access
  uint64_t * data;               // the actual data
};

struct table_info * database_new_table( uint64_t record_size, uint64_t initial_records );
struct index_info * database_new_index( struct table_info * table, uint64_t (* function)(void *, uint64_t, int) );

int database_table_insert( struct table_info * table, void * buffer );
int database_table_select( struct table_info * table, uint64_t key, void * buffer );
int database_table_update( struct table_info * table, uint64_t key, void * buffer );
int database_table_delete( struct table_info * table, uint64_t key );

int database_index_select( struct index_info * index, uint64_t key, void * buffer );
int database_index_update( struct index_info * index, uint64_t key, void * buffer );
int database_index_delete( struct index_info * index, uint64_t key );

#endif
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "error.h"
#include "intmath.h"
#include "sha1.h"
#include "database.h"

/**
 * The module has to keep track of existing tables to do proper cleanup on
 * various events.
 */
struct table_info * database_tables  = NULL;

/**
 * Default hashing function. Used for placing the record into the hashmap.
 * @param buffer the data to be hashed.
 * @param size the size of the buffer (in bytes).
 * @param pass the rehashing pass.
 * @return a 64-bit hash.
 */
uint64_t database_hash( void * buffer, uint64_t size, int pass ) {
  uint32_t hash[5];
  sha1( buffer, size, hash );
  return ( (uint64_t) hash[1] ) << 32 | hash[3];
}

/**
 * Resizing function. This is used to calculate the optimal size when rebuilding
 * tables or indices. The current solution is:
 *   items + ( log2( items + 1 ) * 5 )
 * @param items the number of items to reserve space for.
 * @return a number bigger than items.
 */
uint64_t database_optimal_size( uint64_t items ) {
  return items + ( intmath_log2( items + 1 ) * 5 );
}

/**
 * Allocate space for tabledata.
 * @param items the number of rows to be reserved.
 * @param item_size the size of a record in bytes.
 * @return a pointer to the allocated memory.
 */
uint64_t * database_allocate_table( uint64_t items, uint64_t item_size ) {
  uint64_t item_space = (item_size | 0x07) + 1; // round up to whole 8
  return (uint64_t *) valloc( items * item_space );
}

/**
 * Allocate space for indexdata.
 * @param items the number of rows to be reserved.
 * @return a pointer to the allocated memory.
 */
uint64_t * database_allocate_index( uint64_t items ) {
  uint64_t item_space = sizeof( uint64_t ) * 2; // key & reference are uint64_t
  return (uint64_t *) valloc( items * item_space );
}

/**
 * Get the index record associated with the index key.
 * @param index the index holding the item.
 * @param key the key of the desired item.
 * @param item the target to which the item shall be written.
 */
int database_index_get_item( struct index_info * index, uint64_t key, uint64_t * item ) {
  if( index == NULL ) {
    return LUNAR_ENULL;
  } else if( index->data == NULL ) {
    return LUNAR_ENULL;
  } else {
    uint64_t offset = ( key % index->size ) * 2;
    if( index->data[ offset ] == key ) {
      *item = index->data[ offset + 1 ];
    } else {
      *item = 0;
    }
    return 0;
  }
}

/**
 * Set the index record associated with the index key.
 * @param index the index holding the item.
 * @param key the key of the desired item.
 * @param item the source from which the data should be read.
 */
int database_index_set_item( struct index_info * index, uint64_t key, uint64_t * item ) {
  if( index == NULL ) {
    return LUNAR_ENULL;
  } else if( index->data == NULL ) {
    return LUNAR_ENULL;
  } else {
    uint64_t offset = ( key % index->size ) * 2;
    index->data[ offset ]     = key;
    index->data[ offset + 1 ] = *item;
    return 0;
  }
}

/**
 * Fill the index' data with key-value pairs. Generate hash values using the
 * given function and the table-records.
 * @param index the index to be built.
 */
int database_build_index( struct index_info * index ) {
  if( index == NULL || index->table == NULL ) {
    return LUNAR_ENULL;
  } else {
    if( index == index->table->index ) {
      // build as primary index
    } else {
      // reference the primary index
    }
  }
}

/**
 * Copy the key-values pairs from one to another index.
 * @param source_data the source from which the data is copied.
 * @param source_size the number of items in the source index.
 * @param target_data the target to which the data shall be copied.
 * @param target_size the number of available slots in target_data.
 */
int database_copy_index_data( uint64_t * source_data, uint64_t source_size,
                              uint64_t * target_data, uint64_t target_size ) {
}

/**
 * Reference another index. The resulting index uses another index as reference
 * for the table rows.
 * @param index the index to be modified.
 * @param reference the index that should be used as reference.
 */
int database_reference_index( struct index_info * index,
                              struct index_info * reference ) {
}

/**
 * Dereference the index data. The resulting index references table rows instead
 * of another index.
 * @param index the index to be dereferenced.
 */
int database_dereference_index( struct index_info * index ) {
}

/**
 * If the table (which is implemented as a hashtable) reaches a specific size
 * or if too many collisions occur it has to be resized. Therefore all indices
 * have to be rebuilt and the entries have to be copied into a new table.
 * Bearing multithreading in mind, we rebuild a copy and commit all changes at
 * once.
 * @param the table to be rebuilt.
 */
int database_rebuild_table( struct table_info * table ) {
  if( table == NULL ) {
    return LUNAR_ENULL;
  } else {
    pthread_rwlock_wrlock( &(table->rwlock) );
    
    struct index_info * primary = table->index;
    uint64_t   new_size = database_optimal_size( table->items );
    uint64_t * new_data = database_allocate_table( new_size, table->item_size );
    uint64_t item_space = (table->item_size | 0x07) + 1; // round up to whole 8
    
    uint64_t * index_it = primary->data; // iterate through the index
    uint64_t * table_it = new_data;      // iterate through the new table data
    
    uint64_t i=0; // step through the new item
    uint64_t j=0; // count the new items
    
    pthread_rwlock_wrlock( &(primary->rwlock) );
    
    // iterate through the primary index and copy each record to the new table
    while( index_it < (primary->data + primary->size) ) {
      // the index-item is associated with a value
      if( *(index_it + 1) != 0 ) {
        j++;
        for( i=0; j<item_space; i++ ) {
          *(table_it + i) = *(table->data + *(index_it+1)*item_space + i);
        }
        *(index_it+1) = j;
        table_it += item_space;
      }
      index_it += 2;
    }
    
    free( table->data );
    table->size = new_size;
    table->data = new_data;
    
    pthread_rwlock_unlock( &(primary->rwlock) );
    pthread_rwlock_unlock( &(table->rwlock) );
  }
}

/**
 * Rebuild the specified index. If the index does not contain all items that the
 * table contains (if table->items is bigger than index->size) all items are
 * rehashed. Otherwise the hash values that already exist are simply copied to
 * a new index.
 * @param index the index to be rebuilt.
 */
int database_rebuild_index( struct index_info * index ) {
  if( index == NULL ) {
    return LUNAR_ENULL;
  } else if( index->table == NULL ) {
    return LUNAR_ENULL;
  } else {
    pthread_rwlock_wrlock( &(index->rwlock) );
    
    pthread_rwlock_t  * rdlock; // pointer to the lock of the referenced data
    struct index_info * primary = index->table->index;
    
    // decide which data should not be written to during the rebuild
    if( index == primary ) {
      rdlock = &(index->table->rwlock); // primary indices point to table data
    } else {
      rdlock = &(primary->rwlock);      // user indices reference a primary
    }
    
    pthread_rwlock_rdlock( rdlock );
  
    // If the index does not contain all items rebuild it
    if( index->table->items > index->items ) {
      free( index->data );
      index->size = 0;
      database_build_index( index );
    } else {
      uint64_t   new_size = database_optimal_size( index->items );
      uint64_t * new_data = database_allocate_index( new_size );
      database_copy_index_data( index->data, index->size,
                                new_data,    new_size );
      free( index->data );
      index->size = new_size;
      index->data = new_data;
    }
    
    pthread_rwlock_unlock( rdlock );
    pthread_rwlock_unlock( &(index->rwlock) );
  }
}

/**
 * Save the whole table (info structure and data) in a binary file.
 * Because we rely on C functions specifying the indices we can't store them in
 * a file (without doing unsafe and stupid operations). Therefore indices get
 * lost after reconstructing the table and we need to rebuild them.
 * @param table the table to be dumped.
 * @param filename the target file to save the data in.
 */
int database_export_table( struct table_info * table, char * filename ) {
}

/**
 * Import a database dump.
 * @param table
 * @param filename
 */
int database_import_table( struct table_info * table, char * filename ) {
}

/**
 * Create a new table: initialize the info structure and reserve initial space.
 * @param item_size the size of one record.
 * @param size the initial number of available rows.
 * @return a pointer to the new table structure.
 */
struct table_info * database_new_table( uint64_t item_size, uint64_t size ) {
  struct table_info * table;
  if( item_size < 1 ) {
    return NULL;
  }
  table = (struct table_info *) malloc( sizeof( struct table_info ) );
  table->next      = database_tables;
  table->index     = NULL;
  table->item_size = item_size;
  table->items     = 0;
  if( size > 0 ) {
    table->size    = size;
    table->data    = database_allocate_table( size, item_size );
  } else {
    table->size    = 0;
    table->data    = NULL;
  }
  pthread_rwlock_init( &(table->rwlock), NULL );
  
  // create a primary index
  database_new_index( table, &database_hash );
  
  database_tables = table;
  return table;
}

/**
 * Create a new index: initialize the info structure and reserve as much space
 * as required for indexing the given table.
 * @param table the table to be indexed.
 * @param function the function used to generate the keys.
 * @return a pointer to the new index structure.
 */
struct index_info * database_new_index( struct table_info * table,
                      uint64_t (* function)(void *, uint64_t, int) ) {
  struct index_info * index;
  if( table == NULL || function == NULL ) {
    return NULL;
  }
  index = (struct index_info *) malloc( sizeof( struct index_info ) );
  index->next     = table->index;
  index->table    = table;
  table->items    = 0;
  index->function = function;
  index->size     = 0;
  index->data     = NULL;
  pthread_rwlock_init( &(index->rwlock), NULL );

  pthread_rwlock_rdlock( &(table->rwlock ) );
  
  database_build_index( index );
  
  if( table->index == NULL ) {
    table->index = index;
  } else {
    table->index->next = index;
  }
  
  pthread_rwlock_unlock( &(table->rwlock) );
  
  return index;
}


int database_table_insert( struct table_info * table, void * buffer ) {
}

int database_table_select( struct table_info * table, uint64_t key,
                           void * buffer ) {
}

int database_table_update( struct table_info * table, uint64_t key,
                           void * buffer ) {
}

int database_table_delete( struct table_info * table, uint64_t key ) {
}



int database_index_select( struct index_info * index, uint64_t key,
                           void * buffer ) {
}

int database_index_update( struct index_info * index, uint64_t key,
                           void * buffer ) {
}

int database_index_delete( struct index_info * index, uint64_t key ) {
}

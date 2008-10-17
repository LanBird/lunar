/**
 * The database module stores arbitary records in tables and uses storage_t
 * for concurrent access.
 */
#ifndef _LUNAR_DATABASE_H
#define _LUNAR_DATABASE_H

#include <stdint.h>

typedef struct table_info * table_t;
typedef struct index_info * index_t;

table_t database_new_table( uint64_t record_size, uint64_t initial_records );
index_t database_new_index( uint64_t (* function)(void *, size_t) );

int database_table_insert( table_t table, void * buffer );
int database_table_select( table_t table, uint64_t key, void * buffer );
int database_table_update( table_t table, uint64_t key, void * buffer );
int database_table_delete( table_t table, uint64_t key );

int database_index_select( index_t index, uint64_t key, void * buffer );
int database_index_update( index_t index, uint64_t key, void * buffer );
int database_index_delete( index_t index, uint64_t key );

#endif


/**
 * The Pattern-Trie module provides an optimized radix-trie like structure
 * for fast string lookup.
 */
#ifndef _LUNAR_HASH_MAP_H
#define _LUNAR_HASH_MAP_H

#include <stdlib.h>

#include "storage.h"

void hash_map_init( void );

typedef uint64_t hash_t;
hash_t hash_map_hash( const void * data, size_t bytes );

int hash_map_create( storage_t map );
int hash_map_lookup( storage_t map, hash_t hash, uint64_t * value );
int hash_map_insert( storage_t map, hash_t hash, uint64_t value );
int hash_map_update( storage_t map, hash_t hash, uint64_t value );
int hash_map_delete( storage_t map, hash_t hash );

#endif


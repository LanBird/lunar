/**
 * The Pattern-Trie module provides an optimized radix-trie like structure
 * for fast string lookup.
 */
#ifndef _LUNAR_PATTERN_TRIE_H
#define _LUNAR_PATTERN_TRIE_H

#include "storage.h"

void pattern_trie_init( void );

int pattern_trie_create( storage_t trie );
int pattern_trie_lookup( storage_t trie, const char * key, uint64_t * value );
int pattern_trie_insert( storage_t trie, const char * key, uint64_t value );
int pattern_trie_update( storage_t trie, const char * key, uint64_t value );
int pattern_trie_delete( storage_t trie, const char * key );

#endif


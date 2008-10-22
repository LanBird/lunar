#include <stdlib.h>
#include <stdint.h>

#include "test.h"
#include "pattern_trie.h"

void pattern_trie_test();

struct test_info test_rtctl = {
  "Pattern-trie",
  __FILE__,
  NULL,
  pattern_trie_test,
  NULL,
  0
};

void pattern_trie_test() {
  uint64_t test_value = 1234567;
  uint64_t test_read  = 0;

  storage_t trie = storage_new();

  test_print( "create" );
  if( pattern_trie_create( trie ) ) {
    test_error( "create failed" );
  }

  test_print( "insert" );
  if( pattern_trie_insert( trie, "test.key", test_value ) ) {
    test_error( "first insert failed" );
  }
  if( pattern_trie_insert( trie, "test.whee", 0 ) ) {
    test_error( "second insert failed" );
  }
  if( pattern_trie_insert( trie, "test.whee", 1 ) == 0 ) {
    test_error( "duplicate insert succeeded" );
  }

  test_print( "update" );
  if( pattern_trie_update( trie, "test.whee", test_value ) ) {
    test_error( "update failed" );
  }
  if( pattern_trie_update( trie, "test.woot", test_value ) == 0 ) {
    test_error( "non-existent update succeeded" );
  }

  test_print( "lookup" );
  if( pattern_trie_lookup( trie, "test.key", &test_read ) ) {
    test_error( "first lookup failed" );
  }
  if( test_read != test_value ) {
    test_error( "looked up incorrect value" );
  }
  if( pattern_trie_lookup( trie, "test.whee", &test_read ) ) {
    test_error( "second lookup failed" );
  }
  if( test_read != test_value ) {
    test_error( "looked up incorrect value" );
  }
  if( pattern_trie_lookup( trie, "test.woot", test_value ) == 0 ) {
    test_error( "non-existent lookup succeeded" );
  }

  test_print( "delete" );
  if( pattern_trie_delete( trie, "test.key" ) ) {
    test_error( "delete failed" );
  }
  if( pattern_trie_lookup( trie, "test.key", &test_read ) ) {
    test_error( "deleted lookup succeeded" );
  }
}


#include <stdint.h>

#include "test.h"
#include "sha1.h"
#include "database.h"

int database_set_up();
int database_test();
int database_tear_down();

struct test_info test_database = {
  "Database",
  __FILE__,
  database_set_up,
  database_test,
  database_tear_down,
  0
};

struct table_info * database_test_table;
struct index_info * database_test_index;

uint64_t database_test_hash( void * buffer, uint64_t size, int pass ) {
  uint64_t r=0;
  r = *( (uint64_t *) buffer ) + pass;
  return r;
}

int database_set_up() {
  database_test_table = database_new_table( sizeof( uint64_t ), 10 );
  database_test_index = database_new_index( database_test_table, database_test_hash );
  return 0;
}

int database_test() {
  int      errors = 0;

  uint64_t test_input[ 5 ]  = { 123, 456, 789, 0, 999 };
  uint64_t test_output[ 5 ] = { 0, 0, 0, 0, 0 };
  
  errors += database_table_insert( database_test_table, &test_input[ 0 ] );
  errors += database_table_insert( database_test_table, &test_input[ 1 ] );
  errors += database_index_select( database_test_index, test_input[ 0 ], &test_output[ 0 ] );
  
  test_input[ 0 ] = 123;
  
  if( test_input[ 0 ] != test_output[ 0 ] ) {
    errors++;
  }
  
  return errors;
}

int database_tear_down() {
  return 0;
}


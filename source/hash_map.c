#include <stdint.h>

#include "storage.h"
#include "sha1.h"
#include "rtctl.h"
#include "hash_map.h"

int    hash_map_initial_size = 16;
double hash_map_max_fill = 0.75;
struct rtctl_info hash_map_initial_size_rtctl = RTCTL_INTEGER_INITIALIZER;
struct rtctl_info hash_map_max_fill_rtctl     = RTCTL_REAL_INITIALIZER;

/**
 * Initialize the hash map module:
 *  - register rtctl hooks
 */
void hash_map_init( void ) {
  rtctl_register( &hash_map_initial_size_rtctl, "hash_map.initial_size" );
  rtctl_register( &hash_map_max_fill_rtctl, "hash_map.max_fill" );
}

/**
 * A single record inside the hash map
 */
struct hash_map_record_info {
  uint64_t hash;
  uint64_t value;
};

/**
 * Initialize the hash map. The function allocates sufficient space and
 * sets all hashes to zero.
 * @param map the storage object used for the map
 * @return zero on success.
 */
int hash_map_create( storage_t map ) {
}

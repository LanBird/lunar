#include <stdint.h>

#include "storage.h"
#include "rtctl.h"
#include "pattern_trie.h"

double pattern_trie_growth_factor = 1.3;
int    pattern_trie_initial_nodes = 16;
struct rtctl_info pattern_trie_growth_factor_rtctl = RTCTL_INTEGER_INITIALIZER;
struct rtctl_info pattern_trie_initial_nodes_rtctl = RTCTL_INTEGER_INITIALIZER;

/**
 * Initialize the pattern trie module:
 *  - register rtctl hooks
 */
void pattern_trie_init( void ) {
  rtctl_register( &pattern_trie_growth_factor_rtctl, "pattern_trie.growth_factor" );
  rtctl_register( &pattern_trie_initial_nodes_rtctl, "pattern_trie.initial_nodes" );
}

/**
 * A single node of the trie
 */
struct pattern_trie_node_info {
  uint8_t pattern;  // actual data to be compared
  uint8_t mask;     // mask don't-care-bits
  union {
    uint64_t value; // value of the node if it's a leaf
    struct {
      uint32_t p0;  // critical bit == 0 -> next node
      uint32_t p1;  // critical bit == 1 -> next node
    } next;
  } data;
};

/**
 * Used in pattern_trie_traverse to preserve the context after finishing.
 */
struct pattern_trie_traverse_context {
  uint32_t current_node;
  uint32_t matched_bytes;
  uint32_t last_fork;
  uint32_t state;
};

/**
 * Traverse the trie until the first non-matching node is found or the
 * entire key is found.
 * @param node a pointer to the first content node of the pattern trie
 * @param key the string to be looked up
 * @param c the context to store the results in
 */
void pattern_trie_traverse( struct pattern_trie_node_info * nodes,
                            const char * key,
                            struct pattern_trie_traverse_context * c ) {
  struct pattern_trie_node_info * node = nodes + nodes->data.next.p1;
  register char    in_byte = *key;
  register uint8_t pattern;
  register uint8_t mask;
  register uint8_t criterion;
  register uint8_t crit_bits;
  uint32_t matched_bytes = 0;
  uint32_t last_fork     = 0;
  
// TODO: optimize last_fork preservation
  do {
    pattern   = node->pattern;
    mask      = node->mask;
    criterion = mask | pattern;
    crit_bits = criterion ^ mask;
    if( ( in_byte ^ pattern ) & mask ) {
      // no match at all
      c->state = 0;
      break;
    } else {
      if( mask != 0xff ) {
        last_fork = node - nodes;
      }
      if( ( crit_bits & in_byte ) == 0 ) {
        node = nodes + node->data.next.p0;
      } else if( ( crit_bits & in_byte ) == crit_bits ) {
        node = nodes + node->data.next.p1;
      } else {
        // no match of critical bits, masked part matches
        c->state = 2;
        break;
      }
      if( criterion == 0xff ) {
        matched_bytes++;
        if( in_byte == '\0' ) {
          // whole string matched
          c->state = 1;
          break;
        }
        in_byte = *(key+matched_bytes);
      }
    }
  } while( 1 );
  c->current_node  = node - nodes;
  c->matched_bytes = matched_bytes;
  c->last_fork     = last_fork;
}

/**
 * Initialize the pattern trie. The function allocates sufficient space,
 * steps through all nodes and creates a list of free nodes.
 * @param trie the storage object used for the trie
 * @return zero on success.
 */
int pattern_trie_create( storage_t trie ) {
  // trie.alloc( initial nodes * sizeof( node ) )
  // first node = { .data.next.p0 = pointer to first free node,
  //                .data.next.p1 = pointer to first used node }
  // free nodes = { .data.next.p0 = pointer to next free node }

  return 0;
}

/**
 * Lookup a node and write it's value to the specified location.
 * @param trie the trie to be searched for the key
 * @param key the key = path to be traversed
 * @param value a pointer to output variable
 * @return zero on success, 1 if the node was not found
 */
int pattern_trie_lookup( storage_t trie, const char * key, uint64_t * value ) {
  struct pattern_trie_traverse_context c = { 0, 0, 0, 0 };
  struct pattern_trie_node_info * nodes  = trie->data;
  pattern_trie_traverse( nodes, key, &c );
  if( c.state == 0 ) {
    *value = nodes[ c.current_node ].data.value;
    return 0;
  } else {
    return 1;
  }
}

/**
 * Insert a key-value pair into the trie. Traverse the trie as far as possible
 * then edit the first non-matching node and insert the rest of the key and the
 * value.
 * @param trie the trie to be modified
 * @param key the key to be inserted
 * @param value the value to be inserted
 * @return zero on success, 1 if the node already exists, 2 if some other kind
 * of error occured and the node has not been inserted
 */
int pattern_trie_insert( storage_t trie, const char * key, uint64_t value ) {
  struct pattern_trie_traverse_context c = { 0, 0, 0, 0 };
  struct pattern_trie_node_info * nodes  = trie->data;
  pattern_trie_traverse( nodes, key, &c );
  if( c.state == 0 ) {
    return 1;
  } else {
    // modify current node
    // for each remaining byte in key insert it as a new node requireing a full
    // match
  }
}

/**
 * Update a key value pair present in the trie. Same as lookup except it changes
 * the value instead of reading it.
 * @param trie the trie to be modified
 * @param key the key to be updated
 * @param value the new value to be associated with key
 * @return zero on success, 1 if the node does not exist, 2 if some other kind of
 * error occured and the node has not been updated
 */
int pattern_trie_update( storage_t trie, const char * key, uint64_t value ) {
  struct pattern_trie_traverse_context c = { 0, 0, 0, 0 };
  struct pattern_trie_node_info * nodes  = trie->data;
  pattern_trie_traverse( nodes, key, &c );
  if( c.state == 0 ) {
    nodes[ c.current_node ].data.value = value;
  } else {
    return 1;
  }
}

/**
 * Delete a key and the associated value from the trie. This is quite some work:
 * compare each node while traversing and remember the last node that pattern-
 * matched. After the key has found, delete all nodes from the preserved node
 * until the leaf. Insert all deleted nodes into the list of free nodes.
 * @param trie the trie to be modified
 * @param key the key of the key-value pair to be dropped
 * @return zero on success, 1 if the key was not found
 */
int pattern_trie_delete( storage_t trie, const char * key ) {
  struct pattern_trie_traverse_context c = { 0, 0, 0, 0 };
  struct pattern_trie_node_info * nodes  = trie->data;
  pattern_trie_traverse( nodes, key, &c );
  if( c.state == 0 ) {
    // follow last_pattern_match to the leaf ..
  } else {
    return 1;
  }
}

#include <stdint.h>

#include "storage.h"
#include "rtctl.h"
#include "pattern_trie.h"

int    pattern_trie_initial_nodes = 16;
struct rtctl_info pattern_trie_initial_nodes_rtctl = RTCTL_INTEGER_INITIALIZER;

/**
 * Initialize the pattern trie module:
 *  - register rtctl hooks
 */
void pattern_trie_init( void ) {
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
  uint8_t  state;
};

/**
 * Traverse the trie until the first non-matching node is found or the
 * entire key is found.
 * @param node a pointer to the first content node of the pattern trie
 * @param key the string to be looked up
 * @param c the context to store the results in
 */
void pattern_trie_traverse( struct pattern_trie_node_info * node,
                            const char * key,
                            struct pattern_trie_traverse_context * c ) {
  
  register char    in_byte = *key;
  register uint8_t pattern;
  register uint8_t mask;
  register uint8_t criterion;
  register uint8_t crit_bits;
  uint32_t matched_bytes = 0;
  
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
      if( ( crit_bits & in_byte ) == 0 ) {
        node = node->data.next.p0;
      } else if( ( crit_bits & in_byte ) == crit_bits ) {
        node = node->data.next.p1;
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
  uint32_t node;

  nodes[ 0 ].data.next.p0 = 1;
  nodes[ 0 ].data.next.p1 = 0;
  for( node=1; node<max_nodes; node++ ) {
    nodes[ node ].data.next.p0 = node+1;
  }
  nodes[ node ].data.next.p0 = 0;

  return 0;
}

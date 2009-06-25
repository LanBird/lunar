#ifndef _LUNAR_SHA1_H
#define _LUNAR_SHA1_H

#include <stdio.h>
#include <stdint.h>

typedef struct sha1_context * sha1_context_t;

int sha1_context_init( sha1_context_t * c );
int sha1_context_free( sha1_context_t c );

int sha1_read_string( sha1_context_t c, const char * string );
int sha1_read_file( sha1_context_t c, FILE * file );
int sha1_read_bytes( sha1_context_t c, uint8_t * buffer, size_t bytes );

int sha1_padd_block( sha1_context_t c );
int sha1_clear_block( sha1_context_t c );
int sha1_finalize_block( sha1_context_t c );

int sha1_blockum( sha1_context_t c );

int sha1_string( uint32_t h[], const char * string );
int sha1_file( uint32_t h[], FILE * file );
int sha1_bytes( uint32_t h[], uint8_t * buffer, size_t bytes );

#endif


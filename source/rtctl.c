#include <pthread.h>
#include <string.h>

#include "rtctl.h"

#define RTCTL_STRING 0
#define RTCTL_BOOLEAN 1
#define RTCTL_INTEGER 2
#define RTCTL_REAL 3

void rtctl_set_init_function( rtctl_t hook, void (* init)( rtctl_t ) ) {
  hook->init = init;
}

void rtctl_set_change_function( rtctl_t hook, void (* change)( rtctl_t ) ) {
  hook->change = change;
}

void rtctl_set_integer_unit( rtctl_t hook, const char * unit ) {
}

void rtctl_set_boolean_enum( rtctl_t hook, const char * enumtrue,
                             const char * enumfalse ) {
  strcpy( hook->data.boolean.enumtrue,  enumtrue );
  strcpy( hook->data.boolean.enumfalse, enumfalse );
}

void rtctl_set_real_decimal_digits( rtctl_t hook, unsigned char digits ) {
}

void rtctl_set_real_display_type( rtctl_t hook, char type ) {
}

void rtctl_set_real_decimal_point( rtctl_t hook, char point ) {
}

/**
 * Register a runtime controllable variable.
 * @param hook the hook of the variable to be controllable
 * @param name the unique name for lookup
 * @return 0 on success, 1 if the hook already exists, 2 if the namespace does
 *         not exist
 */
int rtctl_register( rtctl_t hook, const char * name ) {
  return 0;
}

/**
 * Find the named hook (<namespace>.<hook-name>) and return a pointer to it.
 * @param name the name and namespace of the hook
 * @return the hook
 */
rtctl_t rtctl_lookup( const char * name ) {
  return NULL;
}

/**
 * Reset the variable to it's original value.
 * @param name the name and namespace of the hook to be reset
 * @return 0 on success, 1 if the hook does not exist
 */
int rtctl_reset( const char * name ) {
  rtctl_t hook = rtctl_lookup( name );
  if( hook == NULL ) {
    return 1;
  }
  return 0;
}

/**
 * Get the string representation of the hook's value.
 * @param name the name of the hook to be set
 * @param value the place to save the value to
 * @return 0 on success, 1 if the hook does not exist
 */
int rtctl_get( const char * name, char * value ) {
  rtctl_t hook = rtctl_lookup( name );
  if( hook == NULL ) {
    return 1;
  }
  return 0;
}

/**
 * Set the variable to the normalized given value.
 * @param name the name and namespace of the hook
 * @param value a pointer to the new value
 * @return 0 on success, 1 if the hook does not exist,
 *         2 if the value is badly formatted
 */
int rtctl_set( const char * name, const char * value ) {
  rtctl_t hook = rtctl_lookup( name );
  if( hook == NULL ) {
    return 1;
  }
  return 0;
}

/**
 * Get the string representation of the hook's value.
 * @param hook
 * @param value
 */
void rtctl_get_string( const rtctl_t hook, char * value ) {
  if( hook->type == RTCTL_STRING ) {
    strcpy( value, hook->data.string );
  } else {
    strcpy( value, "" );
  }
}

/**
 * Get the boolean representation of the hook's value.
 * @param hook
 * @param value 
 */
void rtctl_get_boolean( const rtctl_t hook, unsigned char * value ) {
  if( hook->type == RTCTL_BOOLEAN ) {
    *value = hook->data.boolean.value;
  } else {
    *value = 0;
  }
}

/**
 * Get the integer representation of the hook's value.
 * @param hook
 * @param value 
 */
void rtctl_get_integer( const rtctl_t hook, long long * value ) {
  if( hook->type == RTCTL_INTEGER ) {
    *value = hook->data.integer.value;
  }
}

/**
 * Get the real representation of the hook's value.
 * @param hook
 * @param value 
 */
void rtctl_get_real( const rtctl_t hook, double * value ) {
  if( hook->type == RTCTL_REAL ) {
    *value = hook->data.real.value;  
  }
}

/**
 * Set the hook's value to the string value.
 * @param hook
 * @param value
 */
void rtctl_set_string( rtctl_t hook, const char * const value ) {
  if( hook->type == RTCTL_STRING ) {
    strcpy( hook->data.string, value );
  }
}

/**
 * Set the hook's value to the boolean value.
 * @param hook
 * @param value
 */
void rtctl_set_boolean( rtctl_t hook, const unsigned char value ) {
  if( hook->type == RTCTL_BOOLEAN ) {
    hook->data.boolean.value = value;
  }
}

/**
 * Set the hook's value to the integer value.
 * @param hook
 * @param value
 */
void rtctl_set_integer( rtctl_t hook, const long long value ) {
  if( hook->type == RTCTL_INTEGER ) {
    hook->data.integer.value = value;
  }
}

/**
 * Set the hook's value to the real value.
 * @param hook
 * @param value
 */
void rtctl_set_real( rtctl_t hook, const double value ) {
  if( hook->type == RTCTL_REAL ) {
    hook->data.real.value = value;
  }
}


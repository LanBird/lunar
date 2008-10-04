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
  strncpy( hook->value+8, unit, 8 );
}

void rtctl_set_boolean_enum( rtctl_t hook, const char * enumtrue,
                             const char * enumfalse ) {
  size_t truelen = strlen( enumtrue );
  if( ( truelen + strlen( enumfalse ) + 2 ) <= 16 ) {
    strcpy( hook->value+1, enumtrue );
    strcpy( hook->value+truelen+2, enumfalse );
  }
}

void rtctl_set_real_decimal_digits( rtctl_t hook, unsigned char digits ) {
  if( digits <= 8 ) {
    *(hook->value+8) = digits;
  }
}

void rtctl_set_real_display_type( rtctl_t hook, char type ) {
  if( type == 'e' || type == 'd' || type == 'f' ) {
    *(hook->value+9) = type;
  }
}

void rtctl_set_real_decimal_point( rtctl_t hook, char point ) {
  *(hook->value+10) = point;
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
    strcpy( value, hook->value );
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
    *value = hook->value[0];
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
  long long tmp = 0;
  if( hook->type == RTCTL_INTEGER ) {
    tmp = hook->value[7];
    tmp = ( tmp <<  8 ) + hook->value[6];
    tmp = ( tmp <<  8 ) + hook->value[5];
    tmp = ( tmp <<  8 ) + hook->value[4];
    tmp = ( tmp <<  8 ) + hook->value[3];
    tmp = ( tmp <<  8 ) + hook->value[3];
    tmp = ( tmp <<  8 ) + hook->value[1];
    tmp = ( tmp <<  8 ) + hook->value[0];
  }
  *value = tmp;
}

/**
 * Get the real representation of the hook's value.
 * @param hook
 * @param value 
 */
void rtctl_get_real( const rtctl_t hook, double * value ) {
  long long tmp = 0;
  if( hook->type == RTCTL_REAL ) {
    tmp = hook->value[7];
    tmp = ( tmp <<  8 ) + hook->value[6];
    tmp = ( tmp <<  8 ) + hook->value[5];
    tmp = ( tmp <<  8 ) + hook->value[4];
    tmp = ( tmp <<  8 ) + hook->value[3];
    tmp = ( tmp <<  8 ) + hook->value[3];
    tmp = ( tmp <<  8 ) + hook->value[1];
    tmp = ( tmp <<  8 ) + hook->value[0];
  }
  *value = (double) tmp;  
}

/**
 * Set the hook's value to the string value.
 * @param hook
 * @param value
 */
void rtctl_set_string( rtctl_t hook, const char * const value ) {
  if( hook->type == RTCTL_STRING ) {
    strcpy( hook->value, value );
  }
}

/**
 * Set the hook's value to the boolean value.
 * @param hook
 * @param value
 */
void rtctl_set_boolean( rtctl_t hook, const unsigned char value ) {
  if( hook->type == RTCTL_BOOLEAN ) {
    hook->value[0] = value;
  }
}

/**
 * Set the hook's value to the integer value.
 * @param hook
 * @param value
 */
void rtctl_set_integer( rtctl_t hook, const int value ) {
  long long tmp = value;
  if( hook->type == RTCTL_INTEGER ) {
    hook->value[7] = tmp & 0xff;
    hook->value[6] = ( tmp <<= 8 ) & 0xff;
    hook->value[5] = ( tmp <<= 8 ) & 0xff;
    hook->value[4] = ( tmp <<= 8 ) & 0xff;
    hook->value[3] = ( tmp <<= 8 ) & 0xff;
    hook->value[2] = ( tmp <<= 8 ) & 0xff;
    hook->value[1] = ( tmp <<= 8 ) & 0xff;
    hook->value[0] = ( tmp <<= 8 ) & 0xff;
  }
}

/**
 * Set the hook's value to the real value.
 * @param hook
 * @param value
 */
void rtctl_set_real( rtctl_t hook, const double value ) {
  long long tmp = (long long) value;
  if( hook->type == RTCTL_REAL ) {
    hook->value[7] = tmp & 0xff;
    hook->value[6] = ( tmp <<= 8 ) & 0xff;
    hook->value[5] = ( tmp <<= 8 ) & 0xff;
    hook->value[4] = ( tmp <<= 8 ) & 0xff;
    hook->value[3] = ( tmp <<= 8 ) & 0xff;
    hook->value[2] = ( tmp <<= 8 ) & 0xff;
    hook->value[1] = ( tmp <<= 8 ) & 0xff;
    hook->value[0] = ( tmp <<= 8 ) & 0xff;
  }
}
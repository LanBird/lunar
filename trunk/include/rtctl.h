/**
 * The runtime control (rtctl) module allows configuration of registered values
 * during runtime.
 */
#ifndef _LUNAR_RTCTL_H
#define _LUNAR_RTCTL_H

#define RTCTL_STRING_INITIALIZER { "", 0, NULL, NULL }
#define RTCTL_BOOLEAN_INITIALIZER { "off", 1, NULL, NULL }
#define RTCTL_INTEGER_INITIALIZER { "0", 2, NULL, NULL }
#define RTCTL_REAL_INITIALIZER { "0.0", 3, NULL, NULL }

typedef struct rtctl_info * rtctl_t;

/**
 * The internal representation of runtime controllable values.
 * The values are encoded using the following scheme:
 *   strings:  plain ascii representation
 *   booleans: 1 byte value (true = 0xff, false = 0x00 ),
 *             16 bytes choices, separated, ended by \0 ("true" then "false")
 *   integers: 8 bytes int, 8+1 bytes unit
 *   reals:    8 bytes double, 1 byte number of decimal digits,
 *             1 byte representation (e = scientific, d = decimal, f = fraction)
 *             1 byte decimal point character (or division sign for fractions)
 */
struct rtctl_info {
  char  value[17]; // encoded value
  short type;      // enumerated type
  void (* init)( rtctl_t );
  void (* change)( rtctl_t );
};

void rtctl_set_init_function( rtctl_t hook, void (* init)( rtctl_t ) );
void rtctl_set_change_function( rtctl_t hook, void (* change)( rtctl_t ) );

void rtctl_set_boolean_enum( rtctl_t hook, const char * enumtrue,
                            const char * enumfalse );
void rtctl_set_integer_unit( rtctl_t hook, const char * unit );
void rtctl_set_real_decimal_digits( rtctl_t hook, unsigned char digits );
void rtctl_set_real_display_type( rtctl_t hook, char type );
void rtctl_set_real_decimal_point( rtctl_t hook, char point );

int rtctl_register( const rtctl_t hook, const char * name );

rtctl_t rtctl_lookup( const char * name );

int rtctl_reset( const char * name );
int rtctl_get( const char * name, char * value );
int rtctl_set( const char * name, const char * value );

void rtctl_get_string( const rtctl_t hook, char * value );
void rtctl_get_boolean( const rtctl_t hook, unsigned char * value );
void rtctl_get_integer( const rtctl_t hook, long long * value );
void rtctl_get_real( const rtctl_t hook, double * value );

void rtctl_set_string( rtctl_t hook, const char * value );
void rtctl_set_boolean( rtctl_t hook, const unsigned char value );
void rtctl_set_integer( rtctl_t hook, const int value );
void rtctl_set_real( rtctl_t hook, const double value );

#endif
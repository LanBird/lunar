# Structure #

Though written in C, lunar follows an object-oriented approach.

## Files ##

Each .c file in source/ should have a .h file in which it declares the functions
and structures for "public use".
If a module does not specify any of these it probably represents an endpoint and
consists only of a main function. If this is the case there is no need for a .h
file.

All .c files are translated to objects with the same name (excluding the
extension).
If a .c file defines a main function an executable is linked and named after the
actual source file.

# Style #

## Indentation ##

All blocks are indented by exactly **two** spaces.
Because the way tabs are displayed relies solely on the setup of your editor, tabs are not allowed.

## Braces and Newlines ##

The placement of curcly braces is mostly a matter of taste, but to ensure a
consistent look of the sourcecode we decided to place the opening `{` at the
end of the line of the expression.

This goes for functions, if/switch/while/for/do blocks as well as structures or
mere blocks (though there is no expression before the opening brace of a block).

There should be exactly one whitespace between the expression and the `{`.

In any case the opening brace should be followed by a new line (or a C++-style
comment).

_Example:_
```
  // somewhere inside a function someone once wrote this:
  do {
    switch( *input ) {
      case 1:
      case 2:  printf( "small" );
               break;
      case 3:  printf( "big" );
               break;
      default: printf( "out of range" );
               break;
    }
  } while( *input != 0 );
```

## Functions ##

To avoid naming conflicts, all functions of a file/module have a common prefix - namely the file name. So - all functions in the files `my_file.c` and `my_file.h` should have the prefix `my_file_`.

If some functions share code you should create a new function for the shared code. You may introduce an additional struct that has the same name as the function, suffixed with `_context` and pass a pointer to the context when calling the shared function.

All internal functions (not in .h files) should be non-blocking.

Functions with an empty argument-list are named void when defining them. If you call such a function use `()` with no in-between space.

_Example:_
```
struct example_compute_context {
  double sum;
  double quotient;
};

int example_compute( double lhs, double rhs, struct example_compute_context * c ) {
  c->sum = lhs + rhs;
  if( rhs != 0 ) {
    c->quotient = lhs / rhs;
    return 0;
  } else {
    return 1;
  }
}

int example_function( void ) {
  int variable;
  int div_by_zero;
  struct example_compute_context c = { 0, 0 };
  // your code here
  div_by_zero = example_compute( 1, variable );
  return div_by_zero;
}
```

## Comments ##

Undocumented commits will be reverted.

### C++-style ###

Most source-code related documentation should be made using C++-style (starting
with `//`) comments. Place the comment at the end of the line you want to
document or - if this does not fit - in the line just before. There should be
one whitespace before and after te two slashes.

If successive lines have C++-style comments you should try to align them to
start on the same column.

_Example:_
```
  void * data = in_buffer; // user input
  int i = 0;               // used to iterate through the data
```

### C-style ###

C-style (`/* ... */`) comments are used to comment actual source-code that
you don't seem to need but you're afraid to completely remove it.
If you comment whole lines create new lines for the opening and the closing
mark. You may specify a reason why you disabled the code from being processed.

If you really think you need them for documentation you should probably think
about writing code that is more intuitive. Comments consisting of multiple
sentences should be javadoc-style as described below.

_Example:_
```
  /* Replaced by a more intuitive but experimental function
  int i = 13516 * x << funny( a );
  */
  int i = recalculate( x, a );
```

### Javadoc-style ###

Javadoc-style (starting with `/**`) comments are used to document each function
and struct. A short description of what the function does or which purpose the
struct serves should be made.

All functions in .c files should be documented with javadoc-style comments.

Each .h file should contain a brief description of the module. There is no need
to document the function definitions. If structs are defined in the .h file
these should be documented with javadoc-style comments.

_Example: (include/example.h)_
```
/**
 * This is the example module. Add a brief overview of what it does.
 */
#ifndef _LUNAR_EXAMPLE_H
#define _LUNAR_EXAMPLE_H

/**
 * The example_info struct defines a key-value-pair. Add information on why
 * this is here should be made.
 */
struct example_data_info {
  int key;     // the key associated with the data
  void * data; // arbitary data
};

typedef example_data_t example_data_info *;

struct example_data * myfile_new_data( uint64_t elements );

#endif
```

_Example: (source/example.c)_
```
#include <stdint.h>

#include "example.h"

/**
 * Allocate space for a new mydata structure and initialize it with values.
 */
struct example_info * example_new_data( uint64_t elements ) {
  // to what you promised to do
}

/**
 * This is a helper function to do serious calculations. Notice that you have to
 * document functions not present in the .h file as well.
 */
uint64_t example_helper( uint64_t operatand ) {
  return 2 * operand;
}

```
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void err( const char* msg, ... ) __attribute__ ((noreturn));

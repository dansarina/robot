#include <stddef.h>
#include "byte.h"

unsigned int byte_ndx(char *b, register unsigned int n, int c)
{
  register char ch;
  register char *p;

  p = b; ch = c;

  for (;;) 
  {
    if (!n) break; if (*p == ch) break; ++p; --n;
    if (!n) break; if (*p == ch) break; ++p; --n;
    if (!n) break; if (*p == ch) break; ++p; --n;
    if (!n) break; if (*p == ch) break; ++p; --n;
  }
  
  return p - b;
}

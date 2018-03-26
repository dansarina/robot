#include "byte.h"

int byte_cmp(register char *s, register unsigned int n, register char *t)
{
  for (;;) {
    if (!n) return 0; if (*s != *t) break; ++s; ++t; --n;
    if (!n) return 0; if (*s != *t) break; ++s; ++t; --n;
    if (!n) return 0; if (*s != *t) break; ++s; ++t; --n;
    if (!n) return 0; if (*s != *t) break; ++s; ++t; --n;
  }
  
  return ((int)(unsigned int)(unsigned char) *s)
       - ((int)(unsigned int)(unsigned char) *t);
}

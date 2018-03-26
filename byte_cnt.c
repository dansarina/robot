#include "byte.h"

unsigned int byte_cnt(register char *s, unsigned int n, int c)
{
  int w;
  w = 0;

  for (;;)
  {
    if (!n) return w; if (*s == c) ++w; ++s; --n;
    if (!n) return w; if (*s == c) ++w; ++s; --n;
    if (!n) return w; if (*s == c) ++w; ++s; --n;
    if (!n) return w; if (*s == c) ++w; ++s; --n;
  }
}

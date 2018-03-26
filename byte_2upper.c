#include <ctype.h>
#include "byte.h"

unsigned int byte_2upper(register char *s, unsigned int n)
{
  int w;
  w = 0;

  for (;;)
  {
    if (!n) return w; if (islower(*s)) { *s = toupper(*s); ++w; } ++s; --n;
    if (!n) return w; if (islower(*s)) { *s = toupper(*s); ++w; } ++s; --n;
    if (!n) return w; if (islower(*s)) { *s = toupper(*s); ++w; } ++s; --n;
    if (!n) return w; if (islower(*s)) { *s = toupper(*s); ++w; } ++s; --n;
  }
}

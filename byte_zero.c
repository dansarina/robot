#include "byte.h"

void byte_zero(register char *to, register unsigned int n)
{
  for (;;) {
    if (!n) return; *to++ = 0; --n;
    if (!n) return; *to++ = 0; --n;
    if (!n) return; *to++ = 0; --n;
    if (!n) return; *to++ = 0; --n;
  }
}

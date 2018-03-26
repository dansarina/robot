#include "byte.h"

void byte_copy_r(register char *to, register unsigned int n, register char *fr)
{
  to = to + n;
  fr = fr + n;

  for (;;)
  {
    if (!n) return; *--to = *--fr; --n;
    if (!n) return; *--to = *--fr; --n;
    if (!n) return; *--to = *--fr; --n;
    if (!n) return; *--to = *--fr; --n;
  }
}

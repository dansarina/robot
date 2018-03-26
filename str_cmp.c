#include <inttypes.h>
#include "str.h"

int str_cmp(str *x, unsigned int n, str *y)
{
  char *s; char *t;
  if (x->p != y->p) return 0;

  s = x->bf; t = y->bf;

  for(;;) { 
    if (!n) return 1; if (*s != *t) return 0; ++s; ++t; --n;
    if (!n) return 1; if (*s != *t) return 0; ++s; ++t; --n;
    if (!n) return 1; if (*s != *t) return 0; ++s; ++t; --n;
    if (!n) return 1; if (*s != *t) return 0; ++s; ++t; --n;
  }
}

int str0_cmp(char *s, unsigned int n, char *t)
{
  static str x; static str y;
  x.bf = s; y.bf = t; x.p = str0_len(s); y.p = str0_len(t); x.n = y.n = 0;
  return str_cmp(&x, n, &y);
}

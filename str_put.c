#include "byte.h"
#include "str.h"

int str_put(str *x, uint32_t n, char *fr)
{
  if (!x->bf ) if (!str_alloc(x, n)) return 0;
  if ( x->n < n) if (!str_alloc(x, x->p + n)) return 0;

  byte_copy(x->bf + x->p, n, fr); x->p += n; x->n -= n;
  return 1;
}

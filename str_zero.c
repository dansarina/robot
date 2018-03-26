#include "byte.h"
#include "str.h"

void str_zero(str *x)
{
  byte_zero(x->bf, x->p + x->n); x->n += x->p; x->p = 0;
}

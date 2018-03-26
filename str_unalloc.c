#include "str.h"

extern void free();

void str_unalloc(str *x)
{
  if (x->bf) free(x->bf); 
  x->bf = 0; x->p = 0; x->n = 0;
}

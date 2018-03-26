#include "io.h"

void io_set(register io* s, register int (*op)(), register int f, 
            register char *b, register unsigned int n) 
{
  s->b = b; 
  s->f = f; 
  s->op = op; 
  s->p = 0; 
  s->n = n;
}

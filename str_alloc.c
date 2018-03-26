#include <errno.h>
#include "byte.h"
#include "str.h"

extern void* malloc(); extern void free();

int str_alloc(str *x, unsigned int n)
{
  char *m; if (n <= 0) { errno = EINVAL; return 0; }

  if (x->bf) { 
    if (n <= x->n + x->p) { return 1; }
    else {
      m = malloc(n); if (!m) { errno = ENOMEM; return 0; }
      x->n = n; byte_copy(m, x->p, x->bf); free(x->bf); 
      x->bf = m; x->n -= x->p ; return 1;
    }
  }

  m = malloc(n); if (!m) { errno = ENOMEM; return 0; } 
  x->bf = m; x->p = 0; x->n = n; return 1;
}

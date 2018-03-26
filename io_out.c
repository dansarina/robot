#include <errno.h>
#include "io.h"
#include "byte.h"
#include "str.h"

static int safewrite(register int (*op)(), register int f, 
                     register char *b, register unsigned int n) 
{
  register int w;
  while (n) 
  {
    w = op(f,b,n);
    if (w == -1) { 
      if (errno == EINTR) continue;
      return -1;
    }
    if (w == 0) ; /* zero? */
    b += w;
    n -= w;
  }
  return 0;
}

int io_flush(register io *s) 
{
  register unsigned int p;
  p = s->p;
  if (!p) return 0; /* nothing to flush */
  s->p = 0;
  return safewrite(s->op, s->f, s->b, p);
}

int io_puts(register io *s, register char *b) 
{
  return io_put(s,b,str0_len(b));
}

int io_put(register io *s, register char *b, register unsigned int n) 
{
  register unsigned int r;
  r = s->n;

  if (n > r - s->p) 
  {
    if (io_flush(s) == -1) 
      return -1;

    if (r < IO_OUTSIZE) 
      r = IO_OUTSIZE;

    while (n > s->n) 
    {
      if (r > n) r = n;
      if (safewrite(s->op,s->f,b,r) == -1) return -1;
      b += r;
      n -= r;
    }
  }

  /* r <= s->n - s->p */
  byte_copy(s->b + s->p, n, b);
  s->p = s->p + n;
  return 0;
}

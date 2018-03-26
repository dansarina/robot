#include <errno.h>
#include "io.h"
#include "rw.h"
#include "byte.h"
#include "min.h"
#include "peer.h"

int peer_getln(struct peer *p, char *ln)
{
  io *s; int r; int q; int n, m, n1, n2; char *x;
  s = &p->in; r = s->p;

  if (!r || p->cont) {
    p->cont = 0; r = peer_feed(p);
    if (r <= 0) return r; /* eof or error */
  }

  n = n1 = n2 = m = 0;

  n1 = byte_ndx(io_peek(s), s->p, '\r');
  n2 = byte_ndx(io_peek(s), s->p, '\n');

  n = min(n1,n2);

  if (n < r) /* line found */
  { 
    q = n; if (n > LN_SIZE) q = LN_SIZE; 
    x = io_peek(s); byte_copy(ln, q, x);

    if (q < LN_SIZE) ln[q] = '\n'; ++q;
    if (n2 < r) ++n2; io_seek(s, n2);

    return q;
  }

  p->cont = 1; return -2;
}

int peer_feed(struct peer *p) 
{
  int r; io *s; char ln[LN_SIZE]; 
  char *x; char copy[LN_SIZE]; 

  s = &p->in;
  
  if (!s->p) return io_feed(s);

  again:
  r = read(s->f, &ln[0], LN_SIZE - s->p);
  
  if (r == -1) {
    if (errno == EINTR) goto again;
    if (errno == EWOULDBLOCK) ; /* mmm */
    return s->p;
  }

  if (r == 0) return 0;

  x = io_peek(s);
  
  /* copy s->p bytes */
  byte_copy(copy, s->p, x);
  
  /* move s->p bytes */
  byte_copy(x - r, s->p, copy);

  /* put r bytes */
  byte_copy(x - r + s->p, r, ln);

  s->p += r;
  s->n -= r;

  return s->p;
}

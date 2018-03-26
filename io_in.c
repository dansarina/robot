#include <errno.h>
#include "io.h"
#include "byte.h"

static int oneread(register int (*op)(), register int fd, 
                   register char *buf, register int len)
{
  register int r;
  
  for (;;)
  {
    r = op(fd,buf,len);
    if (r == -1) if (errno == EINTR) continue;
    return r;
  }
}

static int getthis(register io *s, register char *buf, register int len)
{
  register int r;
  register int q;
  
  r = s->p;
  q = r - len;

  if (q > 0) { r = len; s->p = q; } else s->p = 0;
  byte_copy(buf, r, s->b + s->n);
  s->n += r;
  return r;
}

int io_feed(register io *s)
{
  register int r;
  register int q;

  if (s->p) return s->p;
  q = s->n;

  r = oneread(s->op, s->f, s->b, q);
  if (r <= 0) return r;

  s->p = r;
  q = q - r;
  s->n = q;

  if (q > 0) /* damn, gotta shift */
    byte_copy_r(s->b + q, r, s->b);

  return r;
}

char *io_peek(register io *s)
{
  return s->b + s->n;
}

void io_seek(register io *s, register int len)
{
  s->n = s->n + len;
  s->p = s->p - len;
}

int io_get(register io *s, register char *buf, register int len)
{
  register int r;

  if (s->p > 0) return getthis(s,buf,len);
  if (s->n <= len) return oneread(s->op, s->f, buf, len);
  r = io_feed(s); if (r <= 0) return r;
  return getthis(s,buf,len);
}

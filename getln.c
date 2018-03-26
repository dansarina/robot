#include "getln.h"
#include "io.h"
#include "byte.h"

int getln(register io *s, register char *ln, int sep, unsigned int size)
{
  int r; int n; char *x; int q; int q2;

  /* returns:
   * -1 error (io error)
   *  0 eof
   */

  q2 = 0;

  for (;;) {
    r = io_feed(s);
    if (r <= 0) return r; /* eof or error */
    
    x = io_peek(s);
    n = byte_ndx(x, r, sep);
    
    if (n < r) /* sep found */
    { 
      q = n; if (n + q2 > size) q = size - q2; else ++q;
      byte_copy(ln + q2, q, x); ln[q2 + q] = '\n'; io_seek(s, q);
      return q2 + q;
    }

    byte_copy(ln, r, x); io_seek(s, r); q2 = r;
    /* again */ continue;
  }
}

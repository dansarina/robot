#include <inttypes.h>
#include "str.h"

int str_pad(str *x, uint32_t n, int c)
{
  if (x->n < n)
    if (! str_alloc(x, x->p + n - x->n) ) return 0;
  
  for(;;) { 
    if (!n--) break; str_PUTC(x, (char *) &c); 
    if (!n--) break; str_PUTC(x, (char *) &c); 
    if (!n--) break; str_PUTC(x, (char *) &c); 
    if (!n--) break; str_PUTC(x, (char *) &c); 
  }

  return 1;
}

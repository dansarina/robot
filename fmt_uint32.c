#include <inttypes.h>
#include "fmt.h"

unsigned int fmt_uint32(register char *s, register uint32_t u) 
{
  register int n;
  register uint32_t c;
  n = 1;
  c = u;
  
  while(c > 9) {
    ++n;
    c = c / 10;
  }

  if(s) {
    s = s + n;
    do { 
      *--s = '0' + (u % 10);
      u = u / 10;
    } while(u);
  }

  return n;
}


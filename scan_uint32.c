#include <limits.h>
#include <inttypes.h>
#include "scan.h"

int scan_uint32(register char *s, register uint32_t *u) 
{
  register unsigned int pos;
  register uint32_t r;
  register uint32_t c;

  pos = 0; r = 0;

  for ( ;; ) {
    c = (uint32_t) (unsigned char) (s[pos] - '0');
    if (c < 10) {
      if( ((ULONG_MAX - c) / 10) >= r) 
        r = r * 10 + c;
      else return -1; /* overflow */
      ++pos; continue;
    }
    break;
  }

  *u = r;
  return pos;
}

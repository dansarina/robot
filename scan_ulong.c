#include <limits.h>
#include <inttypes.h>
#include "scan.h"

int scan_ulong(register char *s, register unsigned long *u) 
{
  register unsigned int pos;
  register unsigned long r;
  register unsigned long c;

  pos = 0; r = 0;

  for ( ;; ) {
    c = (unsigned long) (unsigned char) (s[pos] - '0');
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


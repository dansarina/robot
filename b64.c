#include "b64.h"

static char base[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz0123456789[]";

unsigned int b64(char *s, unsigned int v)
{
  unsigned int len; unsigned int u;
  len = 1; u = v;

  while(u > 63) { ++len; u /= 64; }

  if (s) {
    s += len; 
    do { 
      *--s = base[v & 0x3f]; v >>= 6; 
    } while (v); 
  }

  return len;
}

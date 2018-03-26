#include "key.h"

static unsigned long next = 1;

static int r()
{
  next = next * 1103515245 + 12345;
  return (unsigned int) (next/65536) % 32768;
}

void key(char *s, unsigned int len)
{
  int i; char u[] = "0123456789abcdefghijklmnopqrstuvxwyz";
  if (len <= 0) len = 10; i = 0; 
  for (;;) {
    if (!--len) break; s[i] = u[ r() % sizeof u ]; ++i;
    if (!--len) break; s[i] = u[ r() % sizeof u ]; ++i;
    if (!--len) break; s[i] = u[ r() % sizeof u ]; ++i;
    if (!--len) break; s[i] = u[ r() % sizeof u ]; ++i;
  }
  s[i] = 0;
}

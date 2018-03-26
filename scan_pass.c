#include <ctype.h>
#include "scan.h"

unsigned int scan_pass(register char *to, unsigned int n, register char *fr)
{
  int len;
  len = 0;

  for (;;) {
    if (!n || *fr == ' ' || !isprint(*fr)) break; *to++ = *fr++; ++len; --n;
    if (!n || *fr == ' ' || !isprint(*fr)) break; *to++ = *fr++; ++len; --n;
    if (!n || *fr == ' ' || !isprint(*fr)) break; *to++ = *fr++; ++len; --n;
    if (!n || *fr == ' ' || !isprint(*fr)) break; *to++ = *fr++; ++len; --n;
  }

  return len;
}

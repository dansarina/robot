#include "scan.h"

unsigned int scan_word(register char *to, unsigned int n, register char *fr)
{
  int len;
  len = 0;

  for (;;) {
    if (!n || *fr == ' ' || *fr == '\n') break; *to++ = *fr++; ++len; --n;
    if (!n || *fr == ' ' || *fr == '\n') break; *to++ = *fr++; ++len; --n;
    if (!n || *fr == ' ' || *fr == '\n') break; *to++ = *fr++; ++len; --n;
    if (!n || *fr == ' ' || *fr == '\n') break; *to++ = *fr++; ++len; --n;
  }

  return len;
}

#include "scan.h"

unsigned int scan_line(register char *to, unsigned int n, register char *fr)
{
  int len;
  len = 0;

  for (;;) {
    if (!n || *fr == '\r' || *fr == '\n') break; *to++ = *fr++; ++len; --n;
    if (!n || *fr == '\r' || *fr == '\n') break; *to++ = *fr++; ++len; --n;
    if (!n || *fr == '\r' || *fr == '\n') break; *to++ = *fr++; ++len; --n;
    if (!n || *fr == '\r' || *fr == '\n') break; *to++ = *fr++; ++len; --n;
  }

  return len;
}

#include "fmt.h"
#include "ip4.h"

static unsigned int ip4_fmt_big(register char *s, char ip[4])
{
  unsigned int len;
  unsigned int i;

  len = 0;
  i = fmt_ulong(s, (unsigned long) (unsigned char) ip[0]); 
  len += i; if (s) s += i; if (s) *s++ = '.'; ++len;
  i = fmt_ulong(s, (unsigned long) (unsigned char) ip[1]); 
  len += i; if (s) s += i; if (s) *s++ = '.'; ++len;
  i = fmt_ulong(s, (unsigned long) (unsigned char) ip[2]); 
  len += i; if (s) s += i; if (s) *s++ = '.'; ++len;
  i = fmt_ulong(s, (unsigned long) (unsigned char) ip[3]); 
  len += i; if (s) s += i;

  return len;
}

static unsigned int ip4_fmt_little(register char *s, char ip[4])
{
  unsigned int len;
  unsigned int i;

  len = 0;
  i = fmt_ulong(s, (unsigned long) (unsigned char) ip[3]); 
  len += i; if (s) s += i; if (s) *s++ = '.'; ++len;
  i = fmt_ulong(s, (unsigned long) (unsigned char) ip[2]); 
  len += i; if (s) s += i; if (s) *s++ = '.'; ++len;
  i = fmt_ulong(s, (unsigned long) (unsigned char) ip[1]); 
  len += i; if (s) s += i; if (s) *s++ = '.'; ++len;
  i = fmt_ulong(s, (unsigned long) (unsigned char) ip[0]); 
  len += i; if (s) s += i;

  return len;
}

unsigned int ip4_fmt(register char *s, char ip[4])
{
  union { short s; char c[sizeof(short)]; } u;
  u.s = 0x0102;

  if ( sizeof(short) == 2) {
    if (u.c[0] == 1 && u.c[1] == 2) return ip4_fmt_big(s,ip);
    if (u.c[0] == 2 && u.c[1] == 1) return ip4_fmt_little(s,ip);
  }

  return 0; /* mmm */
}

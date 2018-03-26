#include "scan.h"
#include "ip4.h"

unsigned int ip4_scan(char *s, struct ip_address *ip)
{
  unsigned int i;
  unsigned int len;
  unsigned long u;

  len = 0;
  i = scan_ulong(s,&u); if (!i) return 0; ip->d[0] = u; s += i; len += i;
  if (*s != '.') return 0; ++s; ++len;
  i = scan_ulong(s,&u); if (!i) return 0; ip->d[1] = u; s += i; len += i;
  if (*s != '.') return 0; ++s; ++len;
  i = scan_ulong(s,&u); if (!i) return 0; ip->d[2] = u; s += i; len += i;
  if (*s != '.') return 0; ++s; ++len;
  i = scan_ulong(s,&u); if (!i) return 0; ip->d[3] = u; s += i; len += i;
  return len;

}

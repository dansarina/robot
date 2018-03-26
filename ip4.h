#ifndef IP_H
#define IP_H

struct ip_address { unsigned char d[4]; };

#define IP4_FMT 20

extern unsigned int ip4_fmt();
extern unsigned int ip4_scan();

#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <netdb.h>
#include "ip4.h"
#include "byte.h"
#include "ndelay.h"
#include "socket.h"

int socket_connect_host(int s, char *host, unsigned int port)
{
  struct sockaddr_in sa; int r;
  struct hostent *h;

  byte_zero(&sa, sizeof sa);
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);

  if ( (h=gethostbyname(host)) == 0) return -1;
  byte_copy(&sa.sin_addr.s_addr,h->h_length,h->h_addr);

  if (ndelay_off(s) == -1) return -1;
  r = connect(s, (struct sockaddr *) &sa, sizeof sa);
  if (r == -1) return -1;
  if (ndelay_on(s) == -1) return -1;

  return r;
}

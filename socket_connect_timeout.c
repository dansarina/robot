#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>
#include "ip4.h"
#include "byte.h"
#include "socket.h"

int socket_connect_timeout(int s, char *host, unsigned int port, unsigned int tm)
{
  struct sockaddr_in sa; int r;
  struct hostent *h; struct pollfd pl;

  byte_zero(&sa, sizeof sa);
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);

  /*
  if (ip_scan(host, &ip) == 0) return -1;
  byte_copy((char *) &sa.sin_addr, 4, &ip);
  */

  if ( (h=gethostbyname(host)) == 0) return -1;
  byte_copy(&sa.sin_addr.s_addr,h->h_length,h->h_addr);

  pl.fd = s;
  pl.events = POLLOUT;

  r = connect(s, (struct sockaddr *) &sa, sizeof sa);
  if (r == 0) { return 0; }

  if ((errno != EINPROGRESS) && (errno != EWOULDBLOCK)) return -1;

  again:
  r = poll(&pl, 1, tm*1000); if (r < 0) if (errno == EINTR) goto again;
  if (r == 0) { errno = ETIMEDOUT; close(s); return -2; }
  
  return 0;
}

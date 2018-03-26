#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "byte.h"
#include "socket.h"

int socket_bind_reuse_listen(unsigned int port)
{
  struct sockaddr_in sa; int s; int r; int opt;

  if ( (s = socket_tcp()) < 0) return s;

  byte_zero(&sa,sizeof sa);
  sa.sin_family = AF_INET;
  sa.sin_port = htons(port);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);

  opt = 1;
  setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof opt);

  if ( (r = bind(s,(struct sockaddr *) &sa,sizeof sa)) < 0)
    return r;
 
  if ( (r = listen(s,5)) < 0)
    return r;

  return s;
}

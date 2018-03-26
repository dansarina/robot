#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "socket.h"

int socket_accept(int s)
{
  struct sockaddr sa;
  socklen_t len; int i;

  len = sizeof sa;
  
  again: 
  i = accept(s, &sa, &len);

  if (i < 0)
    if (errno == EINTR) goto again;

  return i;
}

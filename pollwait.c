#include <errno.h>
#include "poll.h"

int pollwait(struct pollfd *f, unsigned int n) 
{
  int i;

  again:
  i = poll(f, n, -1);
  if (i < 0) if (errno == EINTR) goto again;
  
  return i;
}

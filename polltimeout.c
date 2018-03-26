#include <errno.h>
#include "poll.h"

int polltimeout(struct pollfd *f, unsigned int n, unsigned int tm) 
{
  int i;

  again:
  i = poll(f, n, tm*1000);
  if (i < 0) if (errno == EINTR) goto again;
  
  return i;
}

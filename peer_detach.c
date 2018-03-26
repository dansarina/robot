#include <unistd.h>
#include "peer.h"

void peer_detach(struct peer *p)
{
  close(p->poll->fd); peer_zero(p);
}

#include "poll.h"
#include "peer.h"

extern int poll_max;
extern struct peer user[];

struct peer * peer_attach(int s)
{
  int j; struct peer *p;

  for (j=0; j < PEERMAX; ++j) {
    p = &user[j];
    if (p->poll->fd == -1) {
      p->poll->fd = s; p->poll->events = POLLIN; peer_set(p, s); break;
    }
  }

  if (j == PEERMAX) { peer_kick(s); return 0; }
  if (j > poll_max) poll_max = j; 

  return p;
}

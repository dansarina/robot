#include "byte.h"
#include "io.h"
#include "rw.h"
#include "peer.h"

void peer_set(struct peer *p, int fd) 
{
  io_set(&p->in, read, fd, &p->inb[0], sizeof p->inb);
  io_set(&p->ou, write, fd, &p->oub[0], sizeof p->oub);
}

void peer_zero(struct peer *p)
{
  byte_zero(&p->inb[0], sizeof p->inb);
  byte_zero(&p->oub[0], sizeof p->oub);

  io_set(&p->in, read, -1, &p->inb[0], sizeof p->inb);
  io_set(&p->ou, write, -1, &p->oub[0], sizeof p->oub);

  p->poll->fd = -1; p->poll->events = 0; p->poll->revents = 0;
  p->cont = 0; p->stage = PEER_CONN_NOT; 

  byte_zero(&p->name[0], sizeof p->name);
}

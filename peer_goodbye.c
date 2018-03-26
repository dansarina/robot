#include "io.h"
#include "rw.h"
#include "peer.h"

void peer_kick(int socket)
{
  io s; char bf[64];
  io_set(&s, write, socket, bf, 64);
  io_puts(&s,"*** Too many connections right now.\n"); 
  io_flush(&s); close(socket);
  io_puts(io1,"*** Peer kicked out\n"); io_flush(io1);
}

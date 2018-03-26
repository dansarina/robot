#include "io.h"
#include "fmt.h"
#include "strerr.h"
#include "peer.h"

extern int poll_max;
extern struct peer user[];

void peer_status()
{
  int i; io *s; char bf[32];
  int y, n; int r;
  n = y = 0;

  for (i=0; i < PEERMAX; ++i) { 
    s = &user[i].in; if (s->f == -1) ++n; else ++y; 
  }

  r = fmt_ulong(bf, (unsigned long) y); bf[r]=0;
  io_puts(io1,"*** Connected peers: "); io_puts(io1,bf);
  r = fmt_ulong(bf, (unsigned long) n); bf[r]=0;
  io_puts(io1,". Free slots: "); io_puts(io1,bf); 
  io_puts(io1,"\n"); io_flush(io1);
}

void peer_flags(struct peer *p)
{
  io_puts(io1, "Status of "); io_puts(io1, p->name); io_puts(io1, ":\n");

  io_puts(io1, "PEER_CONN_NOT ");
  if (! peer_STAGE(p, PEER_CONN_NOT)) io_puts(io1, "not ");
  io_puts(io1, "set\n");

  io_puts(io1, "PEER_CONN_PRG ");
  if (! peer_STAGE(p, PEER_CONN_PRG)) io_puts(io1, "not ");
  io_puts(io1, "set\n");

  io_puts(io1, "PEER_CONN_RDY ");
  if (! peer_STAGE(p, PEER_CONN_RDY)) io_puts(io1, "not ");
  io_puts(io1, "set\n");

  io_puts(io1, "PEER_AUTH_USR ");
  if (! peer_STAGE(p, PEER_AUTH_USR)) io_puts(io1, "not ");
  io_puts(io1, "set\n");

  io_puts(io1, "PEER_AUTH_PWD ");
  if (! peer_STAGE(p, PEER_AUTH_PWD)) io_puts(io1, "not ");
  io_puts(io1, "set\n");

  io_puts(io1, "PEER_BNET_LK1 ");
  if (! peer_STAGE(p, PEER_BNET_LK1)) io_puts(io1, "not ");
  io_puts(io1, "set\n");

  io_puts(io1, "PEER_BNET_LK2 ");
  if (! peer_STAGE(p, PEER_BNET_LK2)) io_puts(io1, "not ");
  io_puts(io1, "set\n");

  io_puts(io1, "PEER_INFO_BOT ");
  if (! peer_STAGE(p, PEER_INFO_BOT)) io_puts(io1, "not ");
  io_puts(io1, "set\n");

  io_puts(io1, "\n -- end -- \n\n"); io_flush(io1);
}

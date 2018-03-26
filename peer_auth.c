#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include "byte.h"
#include "say.h"
#include "min.h"
#include "str.h"
#include "user.h"
#include "strerr.h"
#include "scan.h"
#include "fmt.h"
#include "rw.h"
#include "b64.h"
#include "ver.h"
#include "peer.h"

/* see dansarina.c */
extern char myname[];

static void peer_auth_usr(struct peer *p, char *ln, unsigned int len)
{
  struct urecord u; char *s; unsigned int n; char pass[NICKLEN];

  ln[--len] = 0; s = ln; 

  if (!*p->name) { /* read name first, then password */
    n = scan_word(p->name, min(NICKLEN - 1, len), s); 
    s += n + 1; len -= n + 1;
    if (!len) {
      say_2peer_str1(p, "No way #1.\r\n"); 
      peer_detach(p); return;
    }
  } /* else, we already know user's name (from dcc chat) */

  byte_zero(pass, NICKLEN);

  n = scan_word(pass, min(NICKLEN - 1, len), s); s += n + 1; len -= n + 1;
  if (!n) { 
    say_2peer_str1(p, "No way #2.\r\n"); 
    peer_detach(p); return;
  }

  if (!user_get(&u,p->name)) {
    say_2peer_str1(p, "No way #3.\r\n"); 
    peer_detach(p); return;
  }

  if (!str0_cmp(u.pass, min(u.pn, n), pass)) {
    say_2peer_str1(p, "No way #4.\r\n");
    peer_detach(p); return;
  }

  p->stage |= PEER_AUTH_USR | PEER_AUTH_PWD; 
  say_peer_str3("+++ New peer. Welcome, ", p->name, ".\r\n");
}

static void peer_conn_prg(struct peer *p, char *ln, unsigned int len)
{
  struct sockaddr sa; unsigned int slen; int r; char c;
  
  /* see http://cr.yp.to/docs/connect.html */
  slen = sizeof sa; r = getpeername(p->poll->fd, &sa, &slen);
  
  if (r) {
    read(p->poll->fd, &c, 1); 
    peer_detach(p); return;
  }

  p->stage &= ~PEER_CONN_PRG; p->stage |= PEER_CONN_RDY;
}

void peer_auth(struct peer *p, char *ln, unsigned int len)
{
  if ( peer_STAGE(p, PEER_CONN_NOT)) {
    strerr_die(1, "Stage not operational " ERRLINE);
  }

  if ( peer_STAGE(p, PEER_CONN_PRG)) { peer_conn_prg(p, ln, len); return; }
  if (!peer_STAGE(p, PEER_AUTH_PWD)) { peer_auth_usr(p, ln, len); return; }

  strerr_die(1, "Wrong algorithm -> " ERRLINE);
}

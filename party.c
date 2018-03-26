#include <errno.h>
#include "peer.h"
#include "say.h"
#include "lim.h"
#include "byte.h"
#include "str.h"
#include "scan.h"
#include "user.h"
#include "min.h"
#include "socket.h"
#include "strerr.h"
#include "poll.h"
#include "party.h"

/* see dansarina.c */
extern char myname[];
extern char mychan[];

void prty_raw(struct peer *p, char *ln)
{
  struct urecord u;

  if (!user_get(&u,p->name)) {
    say_2peer_str1(p, "Who the hell are you?\r\n"); 
    peer_detach(p); return;
  }

  if (! user_has_flag(&u,'m') ) {
    say_2peer_str1(p, "You need an m-flag for that.\r\n"); return;
  }

  ln += 4; say_rs1(ln);
}

void prty_say(struct peer *p, char *ln)
{
  ln += 4; say_thru_rs(mychan, ln);
}

void prty_unlink(struct peer *p, char *ln)
{
  char nic[NICKLEN+1]; unsigned int len; unsigned int n;

  len = str0_len(ln); ln[--len] = 0; byte_zero(nic, sizeof nic); 
  n = scan_word(ln, min(4,len), ln); ln += n + 1; len -= n + 1;
  if (!len) { say_2peer_str1(p, "Try .help unlink\r\n"); return; }
  n = scan_word(nic, min(NICKLEN,len), ln);
  say_2peer_str1(p, "Unlinking bot... (hehe, i'm just kidding.)\r\n");
}

void prty_link(struct peer *p, char *ln)
{
  struct urecord u; struct urecord b; unsigned int n; 
  char nic[NICKLEN+1]; unsigned int len; int s; int r;
  struct peer *bot;

  if (!user_get(&u,p->name)) {
    say_2peer_str1(p, "Who the hell are you?\r\n"); 
    peer_detach(p); return;
  }

  if (! user_has_flag(&u,'t') ) {
    say_2peer_str1(p, "You need a t-flag for that.\r\n");
    return;
  }
  
  len = str0_len(ln); ln[--len] = 0; byte_zero(nic, sizeof nic); 
  n = scan_word(ln, min(4,len), ln); ln += n + 1; len -= n + 1;
  if (!len) { say_2peer_str1(p, "Try .help link\r\n"); return; }
  n = scan_word(nic, min(NICKLEN,len), ln);

  if (!user_get(&b,nic)) {
    say_2peer_str1(p, "Who? Never heard of him.\r\n");
    return;
  }

  if (! user_has_flag(&b,'b')) {
    say_2peer_str2(p, nic, " needs a b-flag.\r\n");
    return;
  }

  s = socket_tcp(); r = socket_connect_ndelay(s, b.host, b.port);
  if (r == -1)
    if ( (errno != EINPROGRESS) && (errno != EWOULDBLOCK)) {
      say_2peer_str3(p, "Houston: ", strerror(errno), "\r\n");
      return;
    }

  bot = peer_attach(s); 
  if( !bot ) { 
    say_2peer_str1(p, "No free slots for new connections.\r\n");
    return;
  }

  byte_copy(bot->name, str0_len(nic), nic);
  byte_copy(bot->pass, b.pn, b.pass);

  bot->stage &= ~PEER_CONN_NOT; bot->poll->events = POLLIN | POLLERR;
  bot->stage |= PEER_CONN_PRG | PEER_INFO_BOT | PEER_AUTH_PWD | PEER_LINK_RIG;

  if (r == 0) { /* wow; you gotta be localhost */
    bot->stage &= ~PEER_CONN_PRG; bot->stage |= PEER_CONN_RDY;
    say_2peer_str4(bot, myname, " ", b.pass, "\r\n"); 
  }
}

void prty_help(struct peer *p, char *ln)
{
  say_2peer_str1(p, "Help yourself, and don't bother me.\n");
}

void prty_nsense(struct peer *p, char *ln)
{
  say_2peer_str1(p, "What? Try .help\r\n");
}


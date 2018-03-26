#include <unistd.h>
#include <errno.h>
#include "io.h"
#include "peer.h"
#include "exit.h"
#include "scan.h"
#include "str.h"
#include "socket.h"
#include "poll.h"
#include "strerr.h"
#include "breakup.h"
#include "ip4.h"
#include "say.h"
#include "db.h"
#include "user.h"
#include "min.h"
#include "byte.h"
#include "b64.h"
#include "handlers.h"
#include "log.h"

/* see dansarina.c */
extern struct peer user[];
extern struct pollfd conn[];
extern int poll_max;
extern struct peer *rs;
extern char myname[];

void priv_pass(char *buffer)
{
  struct message m; char *p; unsigned int n; 
  char pass[16]; char newp[16]; struct urecord u;
  
  breakup(&m, buffer); io_puts(io1,"+++ PASS\n"); io_flush(io1); 

  if (! user_exists(m.nick)) {
    say_thru_rs(m.nick, "I don't even know you. Say hello first.\r\n");
    return;
  }

  p = &m.text[6]; /* skip pass command and space */
  n = scan_pass(pass,min(32,str0_len(p)), p); pass[n] = 0;

  if ( n < 2 || n >= 16 ) {
    say_thru_rs(m.nick, "No. Must isprint()-true and "
                        "have 2 <= length() < 16\r\n");
    return; 
  }

  if (! user_get(&u, m.nick) ) {
    say_thru_rs(m.nick, "I'm confused right now.\r\n");
    return;
  }

  if (u.pn == 0) { /* brand new user */
    user_save_pass(m.nick, n, pass);
    say_thru_rs(m.nick, "Password set. Welcome again.\r\n");
    return;
  }

  /* change of password */
  if ( byte_cmp(u.pass, u.pn, pass) ) {
    say_thru_rs(m.nick, "No way. Invalid password.\r\n");
    return;
  }

  p += n; ++p; /* read new password */
  n = scan_pass(newp,min(32,str0_len(p)), p); newp[n] = 0;
  
  if (!n) {
    say_thru_rs(m.nick, "No way. Invalid password.\r\n");
    return;
  }

  user_save_pass(m.nick, n, newp);
  say_thru_rs(m.nick, "Password changed.\r\n"); return;

}

void priv_help(char *buffer) 
{
  struct message m; breakup(&m, buffer); 
  io_puts(io1, "+++ HELP\n"); io_flush(io1);
  say_file_to(&m, "./hlp/say.help.user");
}

void priv_hello(char *buffer) 
{
  struct message m; breakup(&m,buffer);
  io_puts(io1, "+++ HELLO\n"); io_flush(io1);

  if (user_count() == 0) { /* welcome master */
    user_new(&m,"nmtaoylgvfpxj");
    say_file_to(&m,"./hlp/say.hello.master");
    return;
  }

  if (!user_exists(m.nick)) {
    user_new(&m,""); say_file_to(&m,"./hlp/say.usernew"); 
    return;
  }

  say_file_to(&m,"./hlp/say.hello");
}

void ctcp_chat(char *buffer) 
{
  struct message m; char *p; int s; int r; unsigned long ip; int i; 
  unsigned long pt; char ipstr[IP4_FMT]; struct urecord u; struct peer *g;

  breakup(&m,buffer);
  io_puts(io1, "+++ "); io_puts(io1, &m.text[1]); io_flush(io1);

  if (str0_len(m.text) < 16) return; p = &m.text[16];
  if ( (i = scan_ulong(p,&ip)) <= 0) return; p += i + 1;
  if ( (i = scan_ulong(p,&pt)) <= 0) return;

  ipstr[ip4_fmt(&ipstr[0], (unsigned char *) &ip)] = 0;

  s = socket_tcp(); if (s == -1) strerr_sys(1);

  /* XXX: must update to socket_connect_ndelay() */
  r = socket_connect_timeout(s,ipstr,pt,3); 
  if (r == -1) { strerr_sys(1); }
  if (r == -2) { io_puts(io1,"*** Peer timed out\n"); io_flush(io1); return; }

  g = peer_attach(s); if (! g ) return; g->stage = PEER_CONN_RDY;

  if (!user_get(&u,m.nick)) {
    say_2peer_str3(g, "Say hello first. /msg ", myname, " hello\r\n");
    peer_detach(g); return;
  }

  byte_copy(g->name, min(NICKLEN,str0_len(m.nick)) + 1, m.nick);
  g->stage |= PEER_AUTH_USR;
  say_2peer_str3(g,"Hello, ",m.nick,". Type your password:\r\n");
}

void ctcp_ping(register char *buffer) 
{
  struct message m; breakup(&m,buffer);
  io_puts(&rs->ou, "NOTICE ");
  io_puts(&rs->ou, m.nick);
  io_puts(&rs->ou, " :\1PING");
  io_puts(&rs->ou, &m.text[6] );
  io_flush(&rs->ou);

  say_peer_str3("*** Answered PING to ",m.nick,"\n");
}

void ctcp_source(register char *buffer) 
{
  struct message m;
  char src[] = "Nice! Talk to dbast0s@yahoo.com.br";
  breakup(&m,buffer);
  io_puts(&rs->ou, "NOTICE "); io_puts(&rs->ou, m.nick);
  io_puts(&rs->ou, " :\1SOURCE "); io_puts(&rs->ou, src);
  io_puts(&rs->ou, "\1\r\n");
  io_flush(&rs->ou);

  say_peer_str3("*** Answered SOURCE to ",m.nick,"\n");
}

void ctcp_version(register char *buffer) 
{
  struct message m; 
  char ver[] = "dansarina : 0.44 : UNIX";
  breakup(&m,buffer);
  io_puts(&rs->ou, "NOTICE "); io_puts(&rs->ou, m.nick);
  io_puts(&rs->ou, " :\1VERSION "); io_puts(&rs->ou, ver);
  io_puts(&rs->ou, "\1\r\n");
  io_flush(&rs->ou);

  say_peer_str3("*** Answered VERSION to ",m.nick,"\n");
}

void sirc_ping(register char *buffer) 
{
  register char *p; p = buffer;
  while( *p && *p++ != ':'); --p;
  io_puts(&rs->ou, "PONG ");
  io_puts(&rs->ou, p);
  io_flush(&rs->ou);

  io_puts(io1, "PING? PONG! ");
  io_puts(io1, p); io_flush(io1);
}

void sirc_join(register char *buffer) 
{
  struct message m; breakup(&m,buffer);
  io_puts(io1, "+++ JOIN: "); 
  io_puts(io1, m.nick); io_puts(io1,"@"); io_puts(io1, m.host);
  io_puts(io1, " "); io_puts(io1, m.para); io_flush(io1);
}

void sirc_quit(register char *buffer) 
{
  struct message m; breakup(&m, buffer);
  io_puts(io1, "+++ QUIT ");
  io_puts(io1, m.nick); io_puts(io1,"@"); io_puts(io1, m.host);
  io_puts(io1, " "); io_puts(io1, m.text); io_puts(io1, "\n"); 
  io_flush(io1);
}

void sirc_part(register char *buffer) 
{
  struct message m; breakup(&m, buffer);
  io_puts(io1, "+++ PART: ");
  io_puts(io1, m.nick); io_puts(io1,"@"); io_puts(io1, m.host);
  io_puts(io1, " :#"); io_puts(io1, m.para); io_puts(io1, "\n"); 
  io_flush(io1);
}

void sirc_error(register char *buffer) 
{
  io_puts(io1, buffer); io_puts(io1,"\n+++ ERR\n"); 
  io_flush(io1); 

  log("Got ERROR. Time to quit.\n"); _exit(0);
  /* shall we reconnect? */
}

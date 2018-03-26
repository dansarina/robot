#include <sys/types.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include "io.h"
#include "strerr.h"
#include "socket.h"
#include "poll.h"
#include "exit.h"
#include "peer.h"
#include "byte.h"
#include "ndelay.h"
#include "events.h"
#include "say.h"
#include "conf.h"
#include "scan.h"
#include "ver.h"
#include "lim.h"
#include "log.h"

extern void free();

int poll_max; struct pollfd conn[PEERMAX];
struct peer user[PEERMAX]; 

struct peer *rs; /* ircd server */
struct peer *pg; /* plugin daemon */

char myname[NICKLEN+1];
char myuser[USERLEN+1];
char mychan[CHANLEN+1];
char rsserv[SERVLEN+1];

uint32_t rs_port;
uint32_t ls_port;

static void doit_pg() /* pg = plugger */
{
  char ln[LN_SIZE]; int len;

  do {
    len = getln(&pg->in,ln,'\n', sizeof ln);
    if (len == -1) log_strerr_sys(3); if (len == 0) return;

    ln[len] = 0; say_rs1(ln);
    
  } while (rs->in.p); /* buffer =/= empty */
}

static void sigchild(int sig)
{
  pid_t pid; int stat; 

  for (;;) {
    pid = waitpid(-1, &stat, WNOHANG); if (pid <= 0) break; 
    log("sigchild: children terminated\n");
  }
}

static void plugger_startup() 
{
  char plug[1024]; int fd[2]; int r;
  getcwd(plug, sizeof plug); strcat(plug, "/plugger");
  
  r = pipe(fd); if (r < 0) { strerr_sys(1); }
  r = fork(); if (r < 0) { strerr_sys(1); } 

  if (r > 0) { 
    peer_attach(fd[0]); pg = &user[2]; 
    pg->ou.f = fd[1]; /* doc this */
  }

  else { /* redirect 0 && 1, then execl */
    if (fd[0] != 0) { if (dup2(fd[0], 0) != 0) { _exit(1); } close(fd[0]); }
    if (fd[1] != 1) { if (dup2(fd[1], 1) != 1) { _exit(1); } close(fd[1]); }
    execl(plug, "plugger", (void*) 0); _exit(4);
  }
}

static void doit_rs() /* rs = remote server */
{
  char ln[LN_SIZE]; int len;

  do {
    len = peer_getln(rs, ln);
    
    if (len == -2) return; if (len == -1) strerr_sys(3);
    if (len ==  0) strerr_die(2,"Server closed on my face.");
    
    if (! sirc_dispatch(ln, len) ) { 
      io_put(io1, ln, len); io_flush(io1);
      say_peer_ln(ln, len);
    }
    
  } while (rs->in.p); /* buffer =/= empty */
}

static void doit_peer(struct peer *p)
{
  char ln[LN_SIZE]; int len;

  if (peer_STAGE(p, PEER_CONN_PRG)) { peer_auth(p, 0, 0); return; }

  do {
    len = peer_getln(p, ln);
    
    if (len == -2) { return; }
    if (len == -1) { peer_detach(p); return; }
    if (len ==  0) { peer_detach(p); return; }

    io_put(io1, ln, len); io_flush(io1);
    
    /* peer still needs to authenticate */
    if (!peer_STAGE(p, PEER_AUTH_PWD)) { peer_auth(p, ln, len); continue; }

    /* peer asked for something */
    if (ln[0] == '.') { bdot_dispatch(p, &ln[1], --len); continue; }

    /* peer talking, distribute */
    say_party_ln(p, ln, len);

  } while (p->in.p); /* buffer =/= empty */
}

static void conf()
{
  char port[8]; unsigned int n;

  conf_retr(CONFDB "myname", NICKLEN, myname);
  if (!*myname) strerr_die(1, "conf: set my name in " CONFDB "myname");
  conf_retr(CONFDB "myuser", USERLEN, myuser);
  if (!*myuser) strerr_die(1, "conf: set my name in " CONFDB "myuser");
  conf_retr(CONFDB "mychan", CHANLEN, mychan);
  if (!*mychan) strerr_die(1, "conf: set my channel in " CONFDB "mychan");
  conf_retr(CONFDB "rsserv", SERVLEN, rsserv);
  if (!*rsserv) strerr_die(1, "conf: set my server in " CONFDB "rsserv");

  conf_retr(CONFDB "rsport", 7, port);
  n = scan_uint32(port, &rs_port);
  if (!n) 
    strerr_die(1, "conf: set my remote server port in " CONFDB "rsport");

  conf_retr(CONFDB "myport", 5, port);
  n = scan_uint32(port, &ls_port); 
  if (!n) 
    strerr_die(1, "conf: set my listening port in " CONFDB "myport");
}

/* 
   A quick guide to socket handling: the listening socket
   takes user[0], the ircd server takes user[1] and the
   plugin system takes user[2]. This leaves 

                     (sizeof user) - 3 

   peers available for connections. So, you may notice we
   start at 3 when we scan the peers for i/o activity. For
   the first three peers, as described above, we handle each
   with higher priority and separately.
*/

int main()
{
  int sv; int nsocks; int r; int j; int i; int ir; 

  sig_catch(SIGCHLD, sigchild); conf(); 

  for (j=0; j < PEERMAX; ++j) { 
    user[j].poll = &conn[j]; peer_zero(&user[j]); 
  }

  sv = socket_bind_reuse_listen(ls_port); if (sv == -1) strerr_sys(1);
  if (! peer_attach(sv) ) strerr_die(1,"Can't attach peer\n");

  ir = socket_tcp(); if (ir == -1) strerr_sys(1);
  r = socket_connect_timeout(ir, rsserv, rs_port,5); 
  if (r == -1) strerr_sys(1); if (r == -2) strerr_sys(1);
  if (! peer_attach(ir) ) strerr_die(1,"Can't attach peer\n"); 

  rs = &user[1]; /* remote server */

  /* XXX: errors will come thru sirc_dispatch() */
  say_rs5("USER ", myuser, " 1 * :/msg ", myname, " hello\r\n");
  say_rs3("NICK ", myname, "\r\n"); say_rs3("JOIN ", mychan, "\r\n");

  plugger_startup(); /* not the correct place */

  for (;;) {
    nsocks = polltimeout(conn, poll_max + 1, 60);

    if (nsocks == 0) { peer_status(); continue; }

    if (conn[0].revents & (POLLIN | POLLERR)) {
      struct peer *g;
      r = socket_accept(sv); if (r < 0) strerr_sys(2);
      g = peer_attach(r); if (! g ) continue; g->stage = PEER_CONN_RDY;

      say_2peer_str1(g, "Hi. I am Dansarina " VER "\r\n");
      say_2peer_str1(g, "Please, type username password:\r\n");

      if (--nsocks <= 0) continue;
    }

    /* irc server only */
    if (conn[1].revents & (POLLIN | POLLERR)) {
      doit_rs();
    }

    /* plugger only */
    if (conn[2].revents & (POLLIN | POLLERR)) {
      doit_pg();
    }

    /* peers */
    for (i=3; i <= poll_max; ++i)
      if (conn[i].revents & (POLLIN | POLLERR)) 
        doit_peer(&user[i]);

    if (--nsocks <= 0) { continue; }
  }
}

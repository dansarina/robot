#include <errno.h>
#include "io.h"
#include "open.h"
#include "strerr.h"
#include "rw.h"
#include "getln.h"
#include "peer.h"
#include "breakup.h"
#include "sig.h"
#include "b64.h"
#include "say.h"

/* see dansarina.c */
extern struct peer user[];
extern struct peer *rs;
extern int poll_max;
extern char myname[];

#define IMPOLITE "Server is very impolite."

void say_thru_rs(char *to, char *bf)
{
  sig_pipeignore(); 
  io_puts(&rs->ou,"PRIVMSG "); io_puts(&rs->ou, to); 
  io_puts(&rs->ou, " :"); io_puts(&rs->ou, bf);
  if (io_flush(&rs->ou) == -1) {
    if (errno == EPIPE) strerr_die(5,IMPOLITE); else strerr_sys(1);
  }
  sig_pipedefault();
}

void say_rs(char *s1, char *s2, char *s3, char *s4,
            char *s5, char *s6, char *s7, char *s8)
{
  io *s; s = &rs->ou;
  sig_pipeignore(); 
  
  if (s1) io_puts(s,s1); if (s2) io_puts(s,s2);
  if (s3) io_puts(s,s3); if (s4) io_puts(s,s4);
  if (s5) io_puts(s,s5); if (s6) io_puts(s,s6);
  if (s7) io_puts(s,s7); if (s8) io_puts(s,s8);
  
  if (io_flush(&rs->ou) == -1) {
    if (errno == EPIPE) strerr_die(5,IMPOLITE); else strerr_sys(1);
  }
  sig_pipedefault();
}

void say_file_to(struct message *m, char *path) 
{
  int fd; char ln[80]; char bf[8192]; io file; int len;

  fd = open_read(path); if (fd == -1) strerr_sys(1);
  io_set(&file, read, fd, bf, 8192);

  sig_pipeignore();
  for (;;) { 
    len = getln(&file,ln,'\n',80);
    if (len == 0) break;
    if (len == -1) strerr_die(1, "say_file_to() -> " ERRLINE);

    io_puts(&rs->ou,"PRIVMSG "); io_puts(&rs->ou, m->nick);
    io_puts(&rs->ou," :"); io_put(&rs->ou,ln,len); io_flush(&rs->ou);
  }

  close(fd);
  sig_pipedefault();
}

void say_peer_ln(char *ln, unsigned int len)
{
  int i; io *s; struct peer *g;

  sig_pipeignore();
  for (i = 2; i <= poll_max ; ++i) {
    g = &user[i]; s = &g->ou; 

    if ( ! peer_STAGE(g, PEER_AUTH_PWD)) continue;
    io_put(s, ln, len); io_flush(s);
  }
  sig_pipedefault();
}

void say_peer_str(char *s1, char *s2, char *s3, char *s4,
                  char *s5, char *s6, char *s7, char *s8,
                  char *s9)
{
  int j; io *s; struct peer *p;

  sig_pipeignore();
  for (j=2; j <= poll_max; ++j) {
    p = &user[j]; s = &p->ou;

    if (! peer_STAGE(p, PEER_AUTH_PWD)) continue;
    if (  peer_STAGE(p, PEER_INFO_BOT)) continue;

    if (s1) io_puts(s,s1); if (s2) io_puts(s,s2);
    if (s3) io_puts(s,s3); if (s4) io_puts(s,s4);
    if (s5) io_puts(s,s5); if (s6) io_puts(s,s6);
    if (s7) io_puts(s,s7); if (s8) io_puts(s,s8);
    if (s9) io_puts(s,s9); io_flush(s);
  }
  sig_pipedefault();
}

void say_2peer_str(struct peer *p, char *s1, char *s2, char *s3, 
                   char *s4, char *s5, char *s6, char *s7, char *s8,
                   char *s9, char *s10, char *s11, char *s12, char *s13, 
                   char *s14)
{
  io *s;

  sig_pipeignore();
  
  s = &p->ou; 
  if (s1) io_puts(s,s1); if (s2) io_puts(s,s2);
  if (s3) io_puts(s,s3); if (s4) io_puts(s,s4);
  if (s5) io_puts(s,s5); if (s6) io_puts(s,s6);
  if (s7) io_puts(s,s7); if (s8) io_puts(s,s8);
  if (s9) io_puts(s,s9); if (s10) io_puts(s,s10);
  if (s11) io_puts(s,s11); if (s12) io_puts(s,s12);
  if (s13) io_puts(s,s13); if (s13) io_puts(s,s13);
  if (s14) io_puts(s,s14);
  io_flush(s);

/*   s = io1; io_puts(s, ">> "); /\* debug on stdout *\/ */
/*   if (s1) io_puts(s,s1); if (s2) io_puts(s,s2); */
/*   if (s3) io_puts(s,s3); if (s4) io_puts(s,s4); */
/*   if (s5) io_puts(s,s5); if (s6) io_puts(s,s6); */
/*   if (s7) io_puts(s,s7); if (s8) io_puts(s,s8); */
/*   if (s9) io_puts(s,s9); if (s10) io_puts(s,s10); */
/*   if (s11) io_puts(s,s11); if (s12) io_puts(s,s12); */
/*   if (s13) io_puts(s,s13); if (s13) io_puts(s,s13); */
/*   if (s14) io_puts(s,s14); */
/*   io_flush(s); */
 
  sig_pipedefault();
}

void say_party_ln(struct peer *p, char *ln, unsigned int len)
{
  int j; io *s; struct peer *g; char soc[6]; char chn[6];

  sig_pipeignore();
  for (j = 2; j <= poll_max; ++j) {
    g = &user[j]; s = &g->ou;

    if (! peer_STAGE(g, PEER_AUTH_PWD)) continue;

    if ( peer_STAGE(g, PEER_BNET_LK2) ) {
      soc [ b64(soc,g->poll->fd) ] = 0; chn [ b64(chn, 0) ] = 0;

      io_puts(s, "chat "); io_puts(s, myname); io_puts(s, " party "); 
      io_puts(s, p->name); io_puts(s, " "); io_put(s, ln, len); io_flush(s);
    } 

    else {
      io_puts(s, "<"); io_puts(s, p->name); io_puts(s, "> ");
      io_put(s, ln, len); io_flush(s);
    }
  }

  sig_pipedefault();
}

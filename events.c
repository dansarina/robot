#include <errno.h>
#include "io.h"
#include "byte.h"
#include "str.h"
#include "peer.h"
#include "handlers.h"
#include "breakup.h"
#include "strerr.h"
#include "scan.h"
#include "say.h"
#include "min.h"
#include "party.h"
#include "events.h"
#include "poll.h"
#include "ndelay.h"

/* see dansarina.c */
extern char myname[];
extern struct peer *pg;

struct event sirc[] =
{
  { "PING", sirc_ping },
  { "JOIN", sirc_join },
  { "QUIT", sirc_quit },
  { "PART", sirc_part },
  { "ERROR", sirc_error },
  { 0, 0 }
};

struct event priv[] = 
{
  { "HELP", priv_help },
  { "HELLO", priv_hello },
  { "PASS", priv_pass },
  { 0, 0 }
};

struct event ctcp[] = 
{
  { "SOURCE", ctcp_source },
  { "VERSION", ctcp_version },
  { "PING", ctcp_ping }, 
  { "DCCCHAT", ctcp_chat },
  { 0, 0}
};

struct event prty[] =
{
  { "RAW", prty_raw },
  { "SAY", prty_say },
  { "HELP", prty_help },
  { "LINK", prty_link },
  { "UNLINK", prty_unlink },
  { 0, 0 }
};

static struct event *find(struct event table[], register char *item)
{
  int i;
  for (i=0; table[i].name; ++i)
    if (str0_cmp(item, str0_len(table[i].name), table[i].name))
      return &table[i]; /* match */
  return 0;
}

static void ctcp_dispatch(char *bf, unsigned int len, unsigned int k)
{
  char ln[LN_SIZE]; char cmd[COMMLEN]; 
  struct event *e; unsigned int p1, p2; unsigned int n;

  n = len;
  if ( k > 2 ) log_strerr_die(1,"Too many CTCP messages"); /* XXX: must loop */
  
  p1 = byte_ndx(bf, len, '\1'); 
  p2 = byte_ndx(bf + p1 + 1, len, '\1');
  p2 += p1; len = p2 - p1; 

  byte_copy(ln, len, bf + p1 + 1); byte_zero(cmd, COMMLEN); 
  len = scan_word(cmd, COMMLEN, ln);
  if (str0_cmp(cmd,3,"DCC")) 
    scan_word(cmd + len, 4, ln + len + 1);

  e = find(ctcp,cmd); if (!e) return; (*e->f)(bf);

}

static void plugger_tell(struct message m)
{
  static str line; char file[16]; unsigned int n; char *s;
  
  str_REWIND(&line); if (!str_alloc(&line, 512)) _exit(1);
  
  s = &m.text[2]; 

  n = scan_word(file, min(sizeof file,str0_len(s)), s); 
  if (!n) return; file[n] = 0; 

  /* if exceeds, advance until next word */
  if (n == sizeof file) { n = scan_word(s, str0_len(s), s); } 

  s += n;
  
  if (!str_PUTS(&line, file)) _exit(1);
  if (!str_PUTS(&line, " ")) _exit(1);
  if (!str_PUTS(&line, &m.nick)) _exit(1);
  if (!str_PUTS(&line, " ")) _exit(1);
  if (!str_PUTS(&line, &m.user)) _exit(1);
  if (!str_PUTS(&line, " ")) _exit(1);
  if (!str_PUTS(&line, &m.host)) _exit(1);
  if (!str_PUTS(&line, " ")) _exit(1);
  if (!str_PUTS(&line, &m.para)) _exit(1);
  if (!str_PUTS0(&line, s)) _exit(1);
  
  say_2peer_str1(pg, line.bf);
}

int sirc_dispatch(char *bf, unsigned int len)
{
  int i; char *t; struct event *e;
  struct message m; char cmd[COMMLEN]; char ln[LN_SIZE+1];
  unsigned int k; 

  byte_zero(ln,LN_SIZE+1); byte_copy(ln,len,bf); 

  k = byte_cnt(ln, len, '\1');
  if ( k > 0 && k % 2 == 0 ) { ctcp_dispatch(ln,len,k); return 1; }
 
  t = ln; i = 0;
  if( *t == ':') { while (*t && *++t != ' '); if(!*t) return 0; ++t; }

  byte_zero(cmd, sizeof cmd);
  i = scan_word(cmd, sizeof cmd, t);

  if (str0_cmp(cmd,7,"PRIVMSG")) {
    byte_zero(cmd, sizeof cmd); breakup(&m,ln); 
    i = scan_word(cmd,COMMLEN,&m.text[1]); 

    if (str0_cmp(m.para,str0_len(myname),myname)) {
      i = byte_2upper(cmd,i); e = find(priv, cmd); if (e) (*e->f)(ln);
    }

    say_peer_str5(m.nick,"@",m.para," -> ",m.text);

    /* log("sirc_dispatch: +"); log(&m.text[2]); log("+\n"); */
    if ( m.text[1] == '@' ) { plugger_tell(m); }

    io_puts(io1, m.nick); io_puts(io1, "@"); io_puts(io1,m.para); 
    io_puts(io1, " -> "); io_puts(io1, m.text); io_flush(io1);
    return 1;
  }

  e = find(sirc, cmd); if (!e) return 0; (*e->f)(ln); return 1;
}

int bdot_dispatch(struct peer *p, char *ln, unsigned int len)
{
  char cmd[16]; char bf[LN_SIZE+1]; 
  unsigned int n; struct event *e; char *s;

  byte_zero(cmd, sizeof cmd); byte_zero(bf, sizeof bf);

  if (len > LN_SIZE) {
    say_2peer_str1(p, "*** Command too long to make sense\r\n");
    return 0;
  }

  byte_copy(bf, min(len, LN_SIZE), ln); s = bf; 
  n = scan_word(cmd, min(16, len-1), s); s += n + 1; len -= n + 1;
  n = byte_2upper(cmd, n); e = find(prty, cmd);
  if (!e) { prty_nsense(p, bf); return 0; } (*e->f)(p, bf); return 1;
}

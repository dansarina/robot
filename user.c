#include <sys/types.h>
#include <dirent.h>
#include <time.h>
#include "strerr.h"
#include "breakup.h"
#include "str.h"
#include "db.h"
#include "fmt.h"
#include "scan.h"
#include "byte.h"
#include "min.h"
#include "user.h"

int user_has_flag(struct urecord *u, int c)
{
  int r;
  r = byte_ndx(u->flag, NICKLEN, c); if (r < NICKLEN) return 1;
  return 0;
}

unsigned int user_count()
{
  DIR *u; struct dirent *e; unsigned int n;
  u = opendir("./usr/"); if (!u) strerr_sys(1);

  n = 0;
  for (;;) {
    e = readdir(u); if (!e) break;
    if ( e->d_name[0] == '.') continue;
    ++n;
  }

  return n;
}

int user_exists(struct message *m)
{
  static db t; static str key; static str rec; int r;

  str_REWIND(&key); str_REWIND(&rec);

  if (!str_PUTS(&key, m->nick)) strerr_sys(1);
  r = db_open(&t, "./usr/"); if (r == -1) strerr_sys(1);
  r = db_read(&t, &key, &rec); db_close(&t); 

  if (r == -1) return 0; else return 1;
}

int user_new(struct message *m, char *flags)
{
  static db t; static str key; static str dat; int r; char fmt[64];

  str_REWIND(&key); str_REWIND(&dat); 

  /* load key */
  if (!str_PUTS(&key, m->nick)) strerr_sys(1);

  /* load nickname */
  if (! str_put(&dat, fmt_uint32(fmt, str0_len(m->nick)), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_PUTS(&dat, m->nick)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* load hostname */
  if (! str_put(&dat, fmt_uint32(fmt, str0_len(m->host)), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_PUTS(&dat, m->host)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* load flags */
  if (! str_put(&dat, fmt_uint32(fmt, str0_len(flags)), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_PUTS(&dat, flags)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* load password: user will set it later */
  if (! str_PUTS(&dat, "0:,")) strerr_sys(1);

  /* load creation date (UNIX timestamp) */
  if (! str_put(&dat, fmt_uint32(fmt, fmt_uint32(0, time(0))), fmt)) 
    strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_put(&dat, fmt_uint32(fmt, time(0)), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* load last seen date (UNIX timestamp) */
  if (! str_put(&dat, fmt_uint32(fmt, fmt_uint32(0, time(0))), fmt)) 
    strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_put(&dat, fmt_uint32(fmt, time(0)), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  r = db_open(&t, "./usr/"); if (r == -1) strerr_sys(1); 
  r = db_write(&t, &key, &dat); if (r == -1) strerr_sys(1); db_close(&t); 

  return 0;
}

int user_get(struct urecord *r, char *nick)
{
  static str key; static str rec; static db t; int i; char rf[8192]; 
  char *p; uint32_t u; unsigned int n;

  rec.bf = rf; rec.n = 8192; rec.p = 0;
  str_REWIND(&key); /* rec is non-heap memory */ byte_zero(rec.bf, rec.n);

  if (!str_PUTS(&key, nick)) strerr_sys(1);
  i = db_open(&t, "./usr/"); 
  if (i == -1) { 
    /* log: can't open database; broken installation. */
    strerr_sys(1);
  }
  i = db_read(&t, &key, &rec); db_close(&t); 

  if (i == -1) return 0; p = rec.bf;

  byte_zero(r, sizeof (*r));

  /* load nick */
  n = scan_uint32(p, &u); p += n; ++p; r->nn = min(u,NICKLEN-1);
  byte_copy(r->nick, r->nn, p); p += u; ++p;

  /* load host */
  n = scan_uint32(p, &u); p += n; ++p; r->hn = min(u,HOSTLEN-1);
  byte_copy(r->host, r->hn, p); p += u; ++p;

  /* load flags */
  n = scan_uint32(p, &u); p += n; ++p; r->fn = min(u,NICKLEN-1);
  byte_copy(r->flag, r->fn, p); p += u; ++p;

  /* load pass */
  n = scan_uint32(p, &u); p += n; ++p; r->pn = min(u,NICKLEN-1);
  byte_copy(r->pass, r->pn, p); p += u; ++p;

  /* load creation date */
  n = scan_uint32(p, &u); p += n; ++p;
  n = scan_uint32(p, &u); p += n; ++p;
  r->since = u;

  /* load last seen date */
  n = scan_uint32(p, &u); p += n; ++p;
  n = scan_uint32(p, &u); p += n; ++p;
  r->seen = u;

  /* load port */
  n = scan_uint32(p, &u); p += n; ++p;
  n = scan_uint32(p, &u); p += n; ++p;
  r->port = u;

  str_unalloc(&key); return 1;
}

void user_save_pass(char *nick, unsigned int n, char *pass)
{
  struct urecord u;

  if (! user_get(&u, nick) ) 
    strerr_die(1, "Inconsistency in user_get() -> " ERRLINE);

  u.pn = n; byte_copy(u.pass, n, pass); u.pass[n] = 0; 
  user_save(&u);
}

void user_save(struct urecord *u)
{
  static db t; static str key; static str dat; int r; char fmt[32];

  str_REWIND(&key); str_REWIND(&dat); 

  /* put key */
  if (!str_PUTS(&key, u->nick)) strerr_sys(1);

  /* put nickname */
  if (! str_put(&dat, fmt_uint32(fmt,u->nn), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_PUTS(&dat, u->nick)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* put hostname */
  if (! str_put(&dat, fmt_uint32(fmt,u->hn), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_PUTS(&dat, u->host)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* put flags */
  if (! str_put(&dat, fmt_uint32(fmt,u->fn), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_PUTS(&dat, u->flag)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* put pass */
  if (! str_put(&dat, fmt_uint32(fmt,u->pn), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_PUTS(&dat, u->pass)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* put creation date (UNIX timestamp) */
  if (! str_put(&dat, fmt_uint32(fmt, fmt_uint32(0,u->since)), fmt))
    strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_put(&dat, fmt_uint32(fmt,u->since), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* put last seen date (UNIX timestamp) */
  if (! str_put(&dat, fmt_uint32(fmt, fmt_uint32(0,u->seen)), fmt))
    strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_put(&dat, fmt_uint32(fmt,u->seen), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  /* put port */
  if (! str_put(&dat, fmt_uint32(fmt, fmt_uint32(0,u->port)), fmt))
    strerr_sys(1);
  if (! str_PUTS(&dat, ":") ) strerr_sys(1);
  if (! str_put(&dat, fmt_uint32(fmt,u->port), fmt)) strerr_sys(1);
  if (! str_PUTS(&dat, ",")) strerr_sys(1);

  r = db_open(&t, "./usr/"); if (r == -1) strerr_sys(1); 
  r = db_write(&t, &key, &dat); if (r == -1) strerr_sys(1); 

  db_close(&t); str_unalloc(&key); str_unalloc(&dat);
}

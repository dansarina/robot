#include "byte.h"
#include "scan.h"
#include "breakup.h"

void breakup(struct message *m, register char *b) 
{
  register char *p; register int i;

  byte_zero(m,sizeof *m);

  p = b; ++p; /* skip colon */

  /* load nickname */ i = 0;
  while( *p && *p != '!' && i < (NICKLEN - 1))
    m->nick[i++] = *p++;
  m->nick[i] = 0; while (*p && *p++ != '!');

  /* load username */ i = 0;
  while( *p && *p != '@' && i < (USERLEN - 1))
    m->user[i++] = *p++;
  m->user[i] = 0; while (*p && *p++ != '@');

  /* load hostname */ i = 0;
  while( *p && *p != ' ' && i < (HOSTLEN - 1))
    m->host[i++] = *p++;
  m->host[i] = 0; while (*p && *p++ != ' ');

  /* forward 'til parameter */
  while (*p && *p++ != ' ');

  if (*p == '#' || *p == '&') ++p;

  /* load parameter */ i = 0;
  while( *p && *p != ' ' && i < (PARALEN - 1))
    m->para[i++] = *p++;
  m->para[i] = 0;  while (*p && *++p != ':');

  /* load text */ i = 0;
  while( *p && i < (TEXTLEN - 1))
    m->text[i++] = *p++;
  m->text[i] = 0;
}

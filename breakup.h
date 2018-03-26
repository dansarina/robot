#ifndef BREAKUP_H
#define BREAKUP_H

#include "lim.h"

struct message 
{
  char nick[NICKLEN];
  char user[USERLEN];
  char host[HOSTLEN];
  char para[PARALEN];
  char text[TEXTLEN];
};

extern void breakup();

#endif

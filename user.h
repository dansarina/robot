#ifndef USER_H
#define USER_H

#include <inttypes.h>
#include "breakup.h"

struct urecord {
  char nick[NICKLEN]; unsigned int nn;
  char host[HOSTLEN]; unsigned int hn;
  char flag[NICKLEN]; unsigned int fn;
  char pass[NICKLEN]; unsigned int pn;
  uint32_t since; uint32_t seen; uint32_t port;
};

extern unsigned int user_count();
extern int user_exists();
extern void user_save();
extern void user_save_pass();
extern int user_new();
extern int user_get();
extern int user_delete();
extern int user_has_flag();

#endif

#ifndef PEER_H
#define PEER_H

#include "io.h"
#include "breakup.h"
#include "poll.h"

#define LN_SIZE 512
#define PEERMAX 16

#define PEER_CONN_NOT 0x00000001
#define PEER_CONN_PRG 0x00000002
#define PEER_CONN_RDY 0x00000004
#define PEER_AUTH_USR 0x00000008
#define PEER_AUTH_PWD 0x00000010
#define PEER_BNET_LK1 0x00000080
#define PEER_BNET_LK2 0x00000100
#define PEER_INFO_BOT 0x00000200
#define PEER_LINK_US1 0x00000400
#define PEER_LINK_US2 0x00000800

#define PEER_LINK_RIG 0x00001000
#define PEER_LINK_LEF 0x00002000

#define PEER_LRIG_WEL 0x00004000
#define PEER_LRIG_IAM 0x00008000
#define PEER_LRIG_USR 0x00010000

#define peer_STAGE(u,x) ((u)->stage & (x))

struct peer {
  char inb[LN_SIZE]; char oub[LN_SIZE]; struct pollfd *poll;
  io in; io ou; int cont; /* see peer_getln.c */
  char name[NICKLEN]; unsigned long stage; /* see peer_auth.c */
  int version; int nlength;
  char pass[NICKLEN]; /* bots, see peer_auth.c */
  void *net;
};

extern void peer_flags();
extern void peer_set();
extern void peer_zero();
extern int peer_feed();
extern int peer_getln();
extern void peer_detach();
extern struct peer * peer_attach();
extern void peer_kick();
extern void peer_status();
extern void peer_auth();

extern void peer_link_rig();
extern void peer_lrig_wel();
extern void peer_lrig_iam();
extern void peer_lrig_usr();

#endif

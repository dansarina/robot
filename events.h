#ifndef EVENTS_H
#define EVENTS_H

struct event { 
  char *name;
  void (*f)();
};

extern int sirc_dispatch();
extern int bdot_dispatch();

#endif

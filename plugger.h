#ifndef PLUGGER_H
#define PLUGGER_H

#define MAX 4

struct plugin {
  char bf[512]; struct pollfd *poll; io in;
};

struct plugin * plugin_attach(int s);

void plugin_zero();
void plugin_flood();
void plugin_set();
void plugin_detach();
void plugin_doit();

int plugin_emptyslot();
int plugin_exist();
int plugin_argv();
int plugin_path();
int plugin_exec();

#endif

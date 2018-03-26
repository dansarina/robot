#include <errno.h>
#include <stdio.h>
#include "log.h"
#include "strerr.h"

void log(char *s)
{
  FILE *log; 
  log = fopen("./dansarina.log", "a");
  fputs(s, log); fclose(log);
}

void log_strerr_die(int c, char *s)
{
  log("fatal: "); log(s); 
  strerr_die(c, s);
}

void log_strerr_sys(int c, char *s)
{
  log("fatal: "); log(s); 
  log("system: "); log(strerror(errno)); 
  strerr_die(c, s);
}

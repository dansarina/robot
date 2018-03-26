#include "io.h"
#include "exit.h"
#include "strerr.h"

void strerr_die(int e, char *s1) 
{
  if (s1) io_puts(io2,s1);
  io_puts(io2,"\n"); 
  io_flush(io2); _exit(e);
}

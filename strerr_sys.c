#include <errno.h>
#include "io.h"
#include "exit.h"
#include "strerr.h"

void strerr_sys(int e)
{
  io_puts(io2, strerror(errno));
  io_puts(io2, "\n");
  io_flush(io2); 
  _exit(e);
}

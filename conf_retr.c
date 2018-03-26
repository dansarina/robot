#include <sys/types.h>
#include <sys/stat.h>
#include "strerr.h"
#include "open.h"
#include "rw.h"
#include "io.h"
#include "getln.h"
#include "conf.h"

void conf_retr(char *name, unsigned int n, char *buf)
{
  int fd; struct stat sbf; int r; io s;

  fd = open_read(name);
  if (fd == -1) strerr_2sys(1, "conf_retr: ", name);
  
  if ( fstat(fd, &sbf) == -1)
    strerr_2sys(1, "conf_retr: ", name);
  
  if (sbf.st_size > n)
    strerr_3die(1, "conf: ", name, " exceeds maximum size");
  
  if (n > sbf.st_size) n = sbf.st_size;
  
  io_set(&s, read, fd, buf, n);
  
  r = getln(&s, buf, '\n', 80); close(fd);
  
  if (r ==  0) { *buf = 0; return; }
  if (r == -1) { strerr_2sys(1, "conf: io error with ", name); }

  if (buf[r-1] == '\n') buf[r-1] = 0;
  
}

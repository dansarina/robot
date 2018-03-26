#include <unistd.h>
#include "rw.h"
#include "open.h"
#include "strerr.h"
#include "str.h"
#include "conf.h"

void conf_trunc(char *name, char *s)
{
  int fd; int r;

  fd = open_trunc(name);
  if (fd == -1) strerr_2sys(1, "conf_trunc: ", name);

  r = write(fd, s, str0_len(s));
  if (r <= 0) strerr_2sys(1, "conf_trunc: ", name);
  
  close(fd);

}

#include <sys/types.h>
#include <fcntl.h>
#include "open.h"

int open_trunc(char *f) {
  return open(f,O_WRONLY|O_CREAT|O_TRUNC|O_NONBLOCK,0600); 
}

#include <sys/types.h>
#include <fcntl.h>
#include "open.h"

int open_excl(char *f) {
  return open(f,O_WRONLY|O_CREAT|O_EXCL|O_NONBLOCK,0600); 
}

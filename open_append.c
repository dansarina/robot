#include <sys/types.h>
#include <fcntl.h>
#include "open.h"

int open_append(char *f) { 
  return open(f,O_WRONLY|O_NONBLOCK|O_APPEND|O_CREAT,0600);
}

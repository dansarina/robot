#include <sys/types.h>
#include <fcntl.h>
#include "open.h"

int open_read(char *file) {
  return open(file, O_RDONLY | O_NONBLOCK); 
}

#include <sys/types.h>
#include <fcntl.h>
#include "open.h"

int open_write(char *file)
{ 
  return open(file, O_WRONLY | O_NONBLOCK); 
}

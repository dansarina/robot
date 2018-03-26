#include <sys/stat.h>
#include "db.h"

int db_creat(char *path) 
{
  return mkdir(path, 0700);
}

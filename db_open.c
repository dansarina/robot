#include <sys/types.h>
#include <sys/stat.h>
#include "str.h"
#include "db.h"

int db_open(db *p, char *path)
{
  if( !str_PUTS(&p->name, path) ) return -1;
  if ( path[ str0_len(path) - 1] != '/')
    if (! str_PUTS(&p->name, "/") ) return -1;
  if( !str_0(&p->name) ) return -1;

  if ( db_perm(p) == -1) return -1;

  return 0;
}

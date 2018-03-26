#include <unistd.h>
#include "open.h"
#include "str.h"
#include "db.h"

extern int rename();

int db_write(db *p, str *key, str *dat)
{
  static str x; static str y; int fd; int r; 

  if ( db_perm(p) == -1) return -1;
  if ( key->bf[0] == '.') return -1;

  str_REWIND(&x); str_REWIND(&y);

  if (!str_cat(&x,&p->name)) return -1; str_SEEK(&x, 1);
  if (!str_cats(&x,".")) return -1; 
  if (!str_cat(&x,key)) return -1;
  if (!str_0(&x)) return -1;

  if (!str_cat(&y,&p->name)) return -1; str_SEEK(&y, 1);
  if (!str_cat(&y,key)) return -1; 
  if (!str_0(&y)) return -1;

  fd = open_excl(x.bf); if (fd == -1) return -1;
  r = write(fd, dat->bf, dat->p); if (r == -1) goto err;
  r = fsync(fd); if (r == -1) goto err;
  r = close(fd); if (r == -1) goto err;
  r = rename(x.bf, y.bf); if (r == -1) goto err;

  str_unalloc(&x); str_unalloc(&y);

  return 0;

  err:
  unlink(x.bf); return -1;
}

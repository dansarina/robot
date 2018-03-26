#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include "open.h"
#include "str.h"
#include "db.h"

int db_read(db *p, str *key, str *rec)
{
  static str x; int fd; int r; struct stat sbf; uint32_t n; int need;

  if ( db_perm(p) == -1) return -1;
  
  str_REWIND(&x);

  if (!str_cat(&x,&p->name)) return -1; str_SEEK(&x, 1);
  if (!str_cat(&x,key)) return -1; 
  if (! str_0(&x)) return -1;

  if ( db_perm_key(&x) == -1) return -1;

  fd = open_read(x.bf); if (fd == -1) return -1;
  r = fstat(fd, &sbf); if (fd == -1) goto err;
  if (sbf.st_size == 0) goto ok;
  
  need = 0;
  
  n = sbf.st_size; if (sbf.st_size > rec->n) { 
    n = rec->n; need = sbf.st_size - n;
  }

  r = read(fd, rec->bf, n); if (r == -1) goto err;
  rec->p = n; rec->n -= rec->p; close(fd); /* whatever. */

  str_unalloc(&x);

  ok:
  return need;

  err:
  return -1;
}

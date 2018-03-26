#include <unistd.h>
#include "db.h"

int db_unlock(db *p, str *key)
{
  static str x;
  if ( db_perm(p) == -1) return -1;
  
  str_REWIND(&x);

  if (!str_cat(&x,&p->name)) return -1; str_SEEK(&x,1);
  if (!str_cats(&x,".")) return -1; 
  if (!str_cat(&x,key)) return -1;
  if (!str_0(&x)) return -1;

  return unlink(x.bf); 
}

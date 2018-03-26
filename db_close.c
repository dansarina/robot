#include "byte.h"
#include "db.h"

void db_close(db *p)
{
  str_unalloc(&p->name); p->n = 0;
}

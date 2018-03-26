#ifndef DB_H
#define DB_H

#include <inttypes.h>
#include "str.h"

typedef struct {
  str name; uint32_t n;
} db;

extern int db_open();
extern int db_write();
extern int db_read();
extern int db_creat();
extern void db_close();
extern int db_perm();
extern int db_perm_key();

#endif

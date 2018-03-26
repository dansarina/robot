#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "str.h"
#include "db.h"

int db_perm_key(str *key)
{
  struct stat sbf; 

  if ( lstat(key->bf, &sbf) == -1) return -1;
  if ( geteuid() != sbf.st_uid ) { errno = EPERM; return -1; }
  if ( ! S_ISREG(sbf.st_mode)) { errno = EPERM; return -1; }

  if ( ! (S_IRUSR & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( ! (S_IWUSR & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_IXUSR & sbf.st_mode) ) { errno = EPERM; return -1; }

  if ( (S_IRGRP & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_IWGRP & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_IXGRP & sbf.st_mode) ) { errno = EPERM; return -1; }

  if ( (S_IROTH & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_IWOTH & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_IXOTH & sbf.st_mode) ) { errno = EPERM; return -1; }

  if ( (S_ISUID & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_ISGID & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_ISVTX & sbf.st_mode) ) { errno = EPERM; return -1; }

  return 0;
}

int db_perm(db *p)
{
  struct stat sbf; struct stat pbf; static str parent;

  if ( lstat(p->name.bf, &sbf) == -1) return -1;

  if ( geteuid() != sbf.st_uid ) { errno = EPERM; return -1; }

  if (  S_ISLNK(sbf.st_mode)) { errno = ENOTDIR; return -1; }
  if ( !S_ISDIR(sbf.st_mode) ) { errno = ENOTDIR;  return -1; }

  if ( ! (S_IRUSR & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( ! (S_IWUSR & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( ! (S_IXUSR & sbf.st_mode) ) { errno = EPERM; return -1; }

  if ( (S_IRGRP & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_IWGRP & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_IXGRP & sbf.st_mode) ) { errno = EPERM; return -1; }

  if ( (S_IROTH & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_IWOTH & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_IXOTH & sbf.st_mode) ) { errno = EPERM; return -1; }

  if ( (S_ISUID & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_ISGID & sbf.st_mode) ) { errno = EPERM; return -1; }
  if ( (S_ISVTX & sbf.st_mode) ) { errno = EPERM; return -1; }

  if (! str_cat(&parent,&p->name)) return -1; str_SEEK(&parent, 1);
  if (! str_cats(&parent,"..")) return -1; if (! str_0(&parent)) return -1;
  if ( lstat(parent.bf, &pbf) == -1) return -1;
  
  if ( geteuid() != pbf.st_uid ) { errno = EPERM; return -1; }
  str_unalloc(&parent); return 0;
}

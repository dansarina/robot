#include <errno.h>
#include "byte.h"
#include "str.h"

int str_cat(str *to, str *fr)
{
  if (!str_put(to, fr->p, fr->bf)) 
    return 0;
  return 1;
}

int str_cats(str *to, char *fr)
{
  uint32_t len;
  len = str0_len(fr);
  if (!str_put(to, len, fr)) 
    return 0;
  return 1;
}

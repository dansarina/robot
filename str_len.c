#include "str.h"

unsigned int str_len(str *x)
{
  return x->p;
}

unsigned int str0_len(register char *s)
{
  register char *t;
  t = s;
  for (;;) {
    if(!*t) return t - s; ++t; 
    if(!*t) return t - s; ++t;
    if(!*t) return t - s; ++t; 
    if(!*t) return t - s; ++t;
  }
  return t - s;
}

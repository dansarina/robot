#ifndef STR_H
#define STR_H

#include <inttypes.h>

typedef struct {
  char *bf; uint32_t p; uint32_t n;
} str;

extern int str_alloc();
extern void str_unalloc();

extern int str_put();
extern int str_pad();
extern int str_cat();
extern int str_cats();
extern void str_zero();
extern int str_cmp();
extern int str0_cmp();
extern uint32_t str_len();
extern uint32_t str0_len();

#define str_PEEK(x) (x)->bf
#define str_SEEK(x,q) (x)->p -= q; (x)->n += q
#define str_REWIND(x) (x)->n += (x)->p; (x)->p = 0

#define str_0(x) str_put((x), 1, "")
#define str_PUTS0(x,s) str_put((x), str0_len((s)) + 1, (s))
#define str_PUTS(x,s) str_put((x), str0_len((s)), (s))
#define str_PUTC(x,c) str_put((x), 1, c) /* c is pointer to char */

#endif

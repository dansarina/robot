#ifndef STRERR_H
#define STRERR_H

#include "io.h"

extern void strerr_die(); 
extern void strerr_sys();
extern char *strerror();

#define STRINGIFY(x) #x
#define STRING(x) STRINGIFY(x)
#define ERRLINE __FILE__ ":" STRING(__LINE__)

#define strerr_2die(e, s1, s2) \
do { io_puts(io2, (s1)); io_puts(io2,"\n"); strerr_die((e), (s2)); } while(0)

#define strerr_3die(e, s1, s2, s3) \
do { io_puts(io2, (s1)); io_puts(io2, (s2)); io_puts(io2,"\n"); \
strerr_die(e, s3); } while (0)

#define strerr_4die(e, s1, s2, s3, s4) \
do { io_puts(io2, (s1)); io_puts(io2, (s2)); io_puts(io2, (s3)); \
io_puts(io2,"\n"); strerr_die(e, s2); } while (0)

#define strerr_2sys(e, s1, s2) \
do { io_puts(io2, (s1)); io_puts(io2, (s2)); io_puts(io2,"\n"); \
strerr_sys((e)); } while(0)

#define strerr_3sys(e, s1, s2, s3) \
do { io_puts(io2, (s1)); io_puts(io2, (s2)); io_puts(io2, (s3)); \
io_puts(io2,"\n"); strerr_sys(e); } while(0)

#define strerr_4sys(e, s1, s2, s3, s4) \
do { io_puts(io2, (s1)); io_puts(io2, (s2)); io_puts(io2, (s3)); \
io_puts(io2, (s4)); io_puts(io2,"\n"); strerr_sys(e); } while(0)

#endif

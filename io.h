#ifndef IO_H
#define IO_H

#define IO_OUTSIZE 8192
#define IO_INSIZE  8192

typedef struct io {
  char *b;
  unsigned int p;
  unsigned int n;
  int f;
  int (*op)();
} io;

extern void io_set();
extern int io_flush();
extern int io_puts();
extern int io_put();
extern int io_feed();
extern void io_seek();
extern char *io_peek();
extern int io_get();

extern io *io0;
extern io *io1;
extern io *io2;

#define outs(s) do { io_puts(io1,(s)); } while (0)
#define outsflu(s) do { io_puts(io1,(s)); io_flush(io1); } while (0)

#endif


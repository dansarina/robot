#include "rw.h"
#include "io.h"

char io_err[128];
static io t1 = { io_err, 0, 128, 2, write };

io *io2 = &t1;
char io_out[128];

static io t2 = { io_out, 0, 128, 1, write };
io *io1 = &t2;

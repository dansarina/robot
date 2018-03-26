#include "socket.h"
#include "strerr.h"
#include "io.h"
#include "exit.h"
#include "rw.h"
#include "str.h"

#define bf \
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" \
"oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo" \
"ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo\n"

int main() 
{

  int r; int fd;

  r = socket_tcp(); if (r == -1) strerr_sys(1); fd = r;
  r = socket_connect_timeout(fd,"my.free.dom.to",3333,2);
  if (r < 0) strerr_sys(1);

  while (1) {
    r = write(fd,bf,str0_len(bf));
    if (r != str0_len(bf) ) break;
    sleep(15);
  }

  close(fd); _exit(1);
}

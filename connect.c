#include "socket.h"
#include "strerr.h"
#include "io.h"
#include "exit.h"
#include "rw.h"
#include "str.h"

#define message "Rock you like a hurricane...\n"
#define n 16 - 2

extern int printf();

int main() 
{

  int r; int fd[n]; int i;

  for (i=0; i < n; ++i) {
    r = socket_tcp(); if (r == -1) strerr_sys(1); fd[i] = r;
    r = socket_connect_timeout(fd[i],"my.free.dom.to",3333,10);
    if (r < 0) strerr_sys(1);

    printf("+++ connected: %d\n",fd[i]);
  }

  sleep(5);

  for (i=0; i < n; ++i) {
    r = write(fd[i],message,str0_len(message));
    if (r != str0_len(message) ) strerr_die(1,"Wow.\n");
  }

  sleep(5);

  for (i=0; i < n; ++i) {
    r = close(fd[i]);
    printf("+++ closed: %d (%d)\n",fd[i],r);
  }
    
  _exit(0);
}

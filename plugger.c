
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include "strerr.h"
#include "io.h"
#include "poll.h"
#include "byte.h"
#include "sig.h"
#include "getln.h"
#include "scan.h"
#include "min.h"
#include "str.h"
#include "plugger.h"

#include <stdio.h>

extern void free(); extern void* malloc();

int poll_max; struct pollfd fd[MAX]; struct plugin proc[MAX];

static void log(char *s)
{
  FILE *log;
  log = fopen("./plugger.log", "a");
  fputs(s, log);
  fclose(log);
}

static void sigchild(int sig)
{
  pid_t pid; int stat; 

  for (;;) {
    pid = waitpid(-1, &stat, WNOHANG); if (pid <= 0) break; 
  }
}

int main()
{
  int r; int i; int pfd[2]; int nsocks; io in; char path[1024];
  char bf[512*2]; int len; char **argv[7]; char ln[512];

  sig_catch(SIGCHLD, sigchild);
  
  for (i = 0; i < MAX; ++i) { 
    proc[i].poll = &fd[i]; plugin_zero(&proc[i]);
  }

  plugin_attach(0); /* attach standard input: dansarina */

  for (;;) {
    nsocks = polltimeout(fd, poll_max + 1, 120);

    if (nsocks == 0) { continue; } 

    if (fd[0].revents & (POLLIN | POLLERR)) {


      io_set(&in, read, 0, bf, sizeof bf);

      /* do { */
        len = getln(&in, ln, '\n', sizeof ln);

        if (len == -1) {
           log("quitting. getln() -1\n"); _exit(1);
          /* continue; */
        }

        if (len == 0) {
           log("quitting. getln() 0\n"); _exit(1);
          /* continue; */
        }

        ln[len] = 0;

        log("from dansarina: "); log(ln);

        r = plugin_argv(ln, argv, sizeof argv); 
        if (r) { 
          log("plugin_argv failed\n");
          /* continue;  */
        }

        r = plugin_exist(argv[0]);
        if (!r) { log("plugin doesnt exist\n"); /* continue; */ }

        r = plugin_path(argv[0], path, sizeof path);
        if (r) { log("plugin path error"); /* continue; */ }

        r = plugin_exec(path, argv);
        if (r) { log("could not exec"); /* continue; */ }

      /* } while (in.p); */

      if (--nsocks <= 0) continue;
    }


    for (i=1; i <= poll_max; ++i)
      if (fd[i].revents & (POLLIN | POLLERR)) 
        plugin_doit(&proc[i]);

    if (--nsocks <= 0) { continue; }
  }
}


void plugin_doit(struct plugin *p)
{
  char ln[512]; int len;

  /* do { */
    len = getln(&p->in, ln, '\n', sizeof ln);

    if (len == -1) { /* log error? */
      log("plugin_doit: getln -1\n");
      plugin_detach(p); /* break; */
      return;
    }

    if (len == 0) { /* log success? */
      log("plugin_doit: getln 0\n");
      plugin_detach(p); /* break; */
      return;
    }

    ln[len] = 0; log("I read this: "); log(ln); 

    outsflu(ln);
    
  /* } while (p->in.p); */ /* buffer =/= empty */
}


int plugin_argv(char *ln, char **argv, unsigned int size)
{
  char *s; unsigned int n; int i;

  s = ln; 

  for (i = 0; i <= 4; ++i) { 
  
    if (!*s) return -1;

    n = scan_word(s, min(32, str0_len(s)), s); if (!n) return -1;

    argv[i] = malloc(n+1); if (!argv[i]) return -1;
    byte_copy(argv[i], n, s); argv[i][n] = 0;

    /* if it exceeds, advance until next word */
    if (n == 32) { n = scan_word(s, str0_len(s), s); } s += n + 1;
  }

  n = scan_line(s, min(256, str0_len(s)), s); 
  if (!n) { argv[i] = 0; return 0; }

  /* two extra bytes: one for newline, one for 0-terminator */
  argv[i] = malloc(n+2); if (!argv[i]) return -1;
  byte_copy(argv[i], n+1, s); argv[i][n+2] = 0; s += n + 1; 

  argv[i+1] = 0; return 0;
}


int plugin_exist(char *s)
{
  DIR *u; struct dirent *e; unsigned int n;
  u = opendir("./run/"); if (!u) strerr_sys(1);

  for (;;) {
    e = readdir(u); if (!e) return 0; 
   
    if (e->d_name[0] == '.') continue;
    n = str0_len(e->d_name); if (n != str0_len(s)) continue;

    if (str0_cmp(e->d_name, n, s))
      return 1;
  }
}


int plugin_path(char *s, char *path, unsigned int size)
{
  unsigned int n; char file[16]; static str p;

  p.bf = path; p.n = size;
  n = scan_word(file, min(16,str0_len(s)), s); file[n] = 0;

  getcwd(path, size);
  if (str0_len(path) + str0_len("/run/") >= size - n) return -1;

  p.p = str0_len(path); p.n -= p.p;
  
  if (!str_PUTS(&p, "/run/")) return -1;
  if (!str_PUTS(&p, file)) return -1; str_0(&p);

  return 0;
}


int plugin_exec(char *path, char **argv)
{
  int r; int fd[2];

  if (!plugin_emptyslot()) { 
    log("No empty slot.\n");
    return 0; 
  }

  r = pipe(fd); 
  if (r < 0) { return -1; }
      
  r = fork(); 
  if (r < 0) { return -1; }
 
  else if (r > 0) {
    plugin_attach(fd[0]); close(fd[1]); return 0;
  }

  else {
    close(fd[0]);

    if (fd[1] != 1) {
      if (dup2(fd[1], 1) != 1) exit(1);
      close(fd[1]); /* we can close the duplicate */
    }

    execve(path, argv, (void *) 0); _exit(4);
  }
}


void plugin_zero(struct plugin *p)
{
  byte_zero(&p->bf[0], sizeof p->bf);
  io_set(&p->in, read, -1, &p->bf[0], sizeof p->bf);
  p->poll->fd = -1; p->poll->events = 0; p->poll->revents = 0;
}

void plugin_flood(int s)
{
  /* log("plugger: sorry, too many plugins running.\n"); */
}

void plugin_set(struct plugin *p, int fd) 
{
  io_set(&p->in, read, fd, &p->bf[0], sizeof p->bf);
}

int plugin_emptyslot()
{
  int j; struct plugin *p;

  for (j = 0; j < MAX; ++j) {
    p = &proc[j]; if (p->poll->fd == -1) return 1;
  }

  return 0;
}

struct plugin * plugin_attach(int s)
{
  int j; struct plugin *p;

  for (j=0; j < MAX; ++j) {
    p = &proc[j];
    if (p->poll->fd == -1) {
      p->poll->fd = s; p->poll->events = POLLIN; plugin_set(p, s);
      break;
    }
  }

  if (j > poll_max) poll_max = j; 

  return p;
}

void plugin_detach(struct plugin *p)
{
  close(p->poll->fd); plugin_zero(p);
}


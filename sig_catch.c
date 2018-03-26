#include <signal.h>
#include "sig.h"

void sig_catch(int sig,void (*f)())
{
  signal(sig,f);
}

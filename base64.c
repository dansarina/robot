#include "io.h"
#include "byte.h"
#include "b64.h"

/* eggdrop code */
static char tobase64array[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
  'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '[', ']'
};

char *int_to_base64(unsigned int val)
{
  static char buf_base64[12];
  int i = 11;

  buf_base64[11] = 0;
  if (!val) {
    buf_base64[10] = 'A';
    return buf_base64 + 10;
  }
  while (val) {
    i--;
    buf_base64[i] = tobase64array[val & 0x3f];
    val = val >> 6;
  }
  return buf_base64 + i;
}
/* eggdrop code */

int main()
{
  char s[7]; unsigned int v; 

  v = 0;

  for (;;) {
    s [ b64(s, v) ] = 0; io_puts(io1, s); io_puts(io1, " "); 
    io_puts(io1, int_to_base64(v)); io_puts(io1, "\n"); io_flush(io1);
    if (v == 4294967295U) break; ++v;
  }

  _exit(0);
}

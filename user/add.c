#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char format_msg[] = "format error: two space-separated numbers expected\n";
  int bfsize = 20;
  char* bf = malloc(bfsize * sizeof(char));
  char* snd;
  char cur;
  int i, l, r, res;
  int status = 0;

  for (i = 0; i < bfsize; ++i) {
    if (read(0, &cur, 1) < 1 || cur == '\r' || cur == '\n' || cur == '\0') {
      break;
    }

    bf[i] = cur;

    if (cur == ' ') {
      if (status == 0)
        status = 1, snd = bf + i + 1;
      else
        status = -1;
      continue;
    }

    if (cur > '9' || cur < '0') {
      status = -1;
      continue;
    }
  }
  bf[i] = '\0';

  if (status == 1) {

  l = atoi(bf);
  r = atoi(snd);

  add(l, r, &res);

  printf("%u\n", res);
  }
  else {
    fprintf(2, "%s", format_msg);
  }

  printf("|%s|\n", bf);
}
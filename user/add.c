#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char format_msg[] = "format error: two space-separated numbers expected\n";
  char alc_msg[]    = "error: allocation fail\n";
  char buf_of_msg[] = "error: buffer overflow (too long input)\n";
  int bfsize = 20;
  char* bf = malloc(bfsize * sizeof(char));

  if (!bf) {
    fprintf(2, "%s", alc_msg);
    return -1;
  }

  char* snd, *fst = bf;
  char cur;
  int i, l, r, res;
  char flipl = 0, flipr = 0;
  int reading = 1;
  int status = 0;

  for (i = 0; i < bfsize; ++i) {
    if (read(0, &cur, 1) < 1 || cur == '\r' || cur == '\n' || cur == '\0') {
      break;
    }

    bf[i] = cur;

    if (cur == '-') {
      if (reading == 1) {
        reading = 0;
        flipl = 1;
        ++fst;
        continue;
      }
      if (reading == 2) {
        reading = 0;
        flipr = 1;
        ++snd;
        continue;
      }
      status = -1;
      continue;
    }

    if (cur == ' ') {
      if (status == 0)
        status = 1, snd = bf + i + 1, reading = 2;
      else
        status = -1;
      continue;
    }

    if (cur > '9' || cur < '0') {
      status = -1;
      continue;
    }
  }

  if (i == bfsize) {
    fprintf(2, "%s", buf_of_msg);
    free(bf);
    return -3;
  }

  bf[i] = '\0';

  if (status == 1) {

  l = atoi(fst);
  r = atoi(snd);

  if (flipl)
   l = -l;

  if (flipr)
    r = -r;

  add(l, r, &res);

  printf("%d\n", res);
  }
  else {
    fprintf(2, "%s", format_msg);
    free(bf);
    return -2;
  }

  printf("|%s|\n", bf);
  free(bf);
  return 0;
}
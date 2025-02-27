#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char format_msg[]     = "format error: two space-separated numbers expected\n";
  char buf_of_msg[]     = "error: buffer overflow (too long input)\n";
  char read_fail_msg[]  = "error: reading failed\n";

  int bfsize = 24; // maximum 24 symbols (11 * 2 numbers, 1 space, 1 terminal)
  char bf[24];

  char* snd, *fst = bf;
  char cur;
  int i, l, r, res;
  char flipl = 0, flipr = 0;
  int reading = 1;
  int status = 0;
  int termd = 0;

  for (i = 0; i < bfsize; ++i) {
    int r = read(0, &cur, 1);

    if (r < 0) {
      fprintf(2, "%s", read_fail_msg);
      exit(4);
    }

    if (r < 1) {
      break;
    }

    if (cur == '\r' || cur == '\n' || cur == '\0') {
      termd = 1;
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

    reading = 0;

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

  if (i == bfsize && !termd) {
    fprintf(2, "%s", buf_of_msg);
    exit(3);
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
    exit(2);
  }

  printf("|%s|\n", bf);
  exit(0);
}
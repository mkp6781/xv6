#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  printf("HELLO WORLD\n");
  sleep(5);
  printf("Hello after sleep....zzzz\n");
  exit(0);
}

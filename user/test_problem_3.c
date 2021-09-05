#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/processinfo.h"

int
main(int argc, char *argv[])
{
  struct processinfo procinfo;
  get_process_info(&procinfo);
  printf("Process ID -> %d\n", procinfo.pid);
  printf("Process Name -> %s\n", *procinfo.name);
  printf("Process Size -> %d Bytes\n", procinfo.sz);
  exit(0);
}

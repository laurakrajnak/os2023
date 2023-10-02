#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int toParent[2];
  int toChild[2];

  pipe(toParent);
  pipe(toChild);

  int pid = fork();

  if(pid == 0) { //child
    char received;
    read(toChild[0], &received, 1);
    printf("%d: received ping\n", getpid());
    write(toParent[1], "a", 1);
  } else {
    write(toChild[1], "b", 1);
    char received;
    read(toParent[0], &received, 1);
    printf("%d: received pong\n", getpid());
  }
  exit(0);
}

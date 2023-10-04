#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  // Create two pipes for communication: one for parent-to-child and one for child-to-parent.
  int toParent[2];  // Parent writes, child reads
  int toChild[2];   // Child writes, parent reads

  // Create pipes for communication.
  pipe(toParent);
  pipe(toChild);

  // Fork a new process.
  int pid = fork();

  if (pid == 0) { // Child process
    char received;

    // Read a single byte from the pipe connected to the child's end.
    read(toChild[0], &received, 1);

    // Print a message indicating that the child received "ping."
    printf("%d: received ping\n", getpid());

    // Write a single byte to the pipe connected to the parent's end.
    write(toParent[1], "a", 1);
  } else { // Parent process
    // Write a single byte to the pipe connected to the child's end.
    write(toChild[1], "b", 1);

    char received;

    // Read a single byte from the pipe connected to the parent's end.
    read(toParent[0], &received, 1);

    // Print a message indicating that the parent received "pong."
    printf("%d: received pong\n", getpid());
  }

  // Exit the program with a return code of 0 to indicate successful execution.
  exit(0);
}


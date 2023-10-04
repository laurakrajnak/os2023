#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int ticks;

  // Check if the correct number of command-line arguments is provided.
  if (argc != 2) {
    // Print a usage message to the standard error stream (fd 2).
    fprintf(2, "sleep usage: sleep <ticks>\n");
  }

  // Convert the second command-line argument to an integer (number of ticks).
  ticks = atoi(argv[1]);

  // Call the sleep system call to suspend the current process for the specified number of ticks.
  sleep(ticks);

  // Exit the program with a return code of 0 to indicate successful execution.
  exit(0);
}


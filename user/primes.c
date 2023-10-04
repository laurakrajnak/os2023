#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// Recursive function to filter prime numbers.
void exec_pipe(int fd)
{
    int num;

    // Read the next prime number from the pipe.
    read(fd, &num, 4);
    printf("prime %d\n", num);

    int p[2];
    pipe(p);
    int tmp = -1;

    while (1) {
        int n = read(fd, &tmp, 4);

        if (n <= 0) {
            // If there are no more numbers to process, break out of the loop.
            break;
        }

        if (tmp % num != 0) {
            // If the number is not divisible by the current prime number, pass it to the next pipe.
            write(p[1], &tmp, 4);
        }
    }

    if (tmp == -1) {
        // Close file descriptors and return if there are no more numbers to process.
        close(p[1]);
        close(p[0]);
        close(fd);
        return;
    }

    int pid = fork();
    if (pid == 0) {
        // Child process: recursively call exec_pipe with the new pipe.
        close(p[1]);
        close(fd);
        exec_pipe(p[0]);
        close(p[0]);
    } else {
        // Parent process: close file descriptors and wait for the child to finish.
        close(p[1]);
        close(p[0]);
        close(fd);
        wait(0);
    }
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    // Write numbers from 2 to 34 into the pipe.
    for (int i = 2; i < 35; i++) {
        int n = i;
        write(p[1], &n, 4);
    }

    // Close the write end of the pipe.
    close(p[1]);

    // Start the recursive filtering process.
    exec_pipe(p[0]);

    // Close the read end of the pipe.
    close(p[0]);

    // Exit the program with a non-zero return code (1).
    exit(1);
}


// this example shows how to use the ptrace API to
// determine which system calls are being made by the child
// process. Note, this example, only catches the first system
// call.

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#define NUM_SYSCALLS 512  // just to be safe

int main(int argc, char **argv) {
  pid_t child = fork();
  if (child == 0) {
    // do something childish
    ptrace(PTRACE_TRACEME);

    // printf("Hello. I'm the child process.\n");

    // stop myself --allow the parent to get ready to trace me
    kill(getpid(), SIGSTOP);
    system(argv[1]);

  } else {
    int status, syscall_num;
    uint64_t syscall_table[NUM_SYSCALLS] = {0};
    FILE *out = fopen(argv[2], "w");

    // I'm the parent...keep tabs on that child process

    // wait for the child to stop itself
    waitpid(child, &status, 0);

    // this option makes it easier to distinguish normal traps from
    // system calls
    ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

    while (1) {
      do {
        // Request: I want to wait for a system call
        ptrace(PTRACE_SYSCALL, child, 0, 0);

        // actually wait for child status to change
        waitpid(child, &status, 0);

        // there are differented reasons that a child's
        // status might change. Check to see if the child
        // exited
        if (WIFEXITED(status)) {
          // the child exited...let's exit too
          for (int i = 0; i < NUM_SYSCALLS; i++) {
            if (syscall_table[i] == 0) continue;
            fprintf(out, "%d\t%lu\n", i, syscall_table[i]);
          }

          fclose(out);
          exit(1);
        }

        // wait until the process is stopped or bit 7 is set in
        // the status (see man page comment on
        // PTRACE_O_TRACESYSGOOD)
      } while (!(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80));

      // read out the saved value of the RAX register,
      // which contains the system call number
      // For 32-bit machines, you would use EAX.
      syscall_num =
          ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_RAX, NULL);
      syscall_table[syscall_num]++;

      ptrace(PTRACE_SYSCALL, child, 0, 0);
      waitpid(child, &status, 0);
    }
  }
}

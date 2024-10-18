#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
  pid_t pid = fork();
  if (pid < 0) {
    printf("Can't fork process\n");
    exit(-1);
  } else if (pid == 0) {
    printf("Child exiting..\n");
    exit(0);
  }

  sleep(10);

  int status = 0;
  pid_t waited_pid = wait(&status);
  if (waited_pid == -1) {
    printf("Can't wait child process\n");
    return -1;
  }
  if (WIFEXITED(status)) {
    printf("[%d]\tProcess %d exited with code %d\n", getpid(), waited_pid,
           WEXITSTATUS(status));
    return 0;
  }
  return -1;
}
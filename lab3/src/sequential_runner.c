#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/wait.h>

int main() {
  int pid = fork();
  if (pid < 0) {
    printf("cannot fork\n");
  } else if(pid == 0) {
    execl("sequential_min_max", "sequential_min_max", "21", "10", NULL);
  }

  printf("Waiting subprocess..\n");
  int waitedProcess = wait(NULL);
  if (waitedProcess < 0) {
      printf("Can't wait subprocess!\n");
      return -1;
  } else {
    printf("Wait success!\n");
  }

  return 0;
}
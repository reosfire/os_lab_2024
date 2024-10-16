#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/wait.h>

int main() {
  execl("sequential_min_max", "sequential_min_max", "21", "10", NULL);
}
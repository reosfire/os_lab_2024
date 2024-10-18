#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "utils.h"
#include <pthread.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

struct FactorialArgs {
  uint64_t* result;
  pthread_mutex_t* mutex;

  int begin;
  int end;
  int mod;
};

void PartFactorial(const struct FactorialArgs *args) {
  int part = 1;
  for (int i = args->begin; i <= args->end; i++) {
    part *= i;
    part %= args->mod;
  }

  pthread_mutex_lock(args->mutex);
  *(args->result) *= part;
  *(args->result) %= args->mod;
  pthread_mutex_unlock(args->mutex);
}

void* ThreadFactorial(void *args) {
  struct FactorialArgs *sum_args = (struct FactorialArgs *)args;
  PartFactorial(sum_args);
  return NULL;
}

int main(int argc, char **argv) {
  int k = -1;
  int pnum = -1;
  int mod = -1;

  while (true) {
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0:
      switch (option_index) {
      case 0:
        k = atoi(optarg);
        if (k <= 0) {
          printf("k must be a positive number\n");
          return 1;
        }
        break;
      case 1:
        pnum = atoi(optarg);
        if (pnum <= 0) {
          printf("pnum size must be a positive number\n");
          return 1;
        }
        break;
      case 2:
        mod = atoi(optarg);
        if (mod <= 0) {
          printf("mod must be a positive number\n");
          return 1;
        }
        break;

      defalut:
        printf("Index %d is out of options\n", option_index);
      }
      break;

    case '?':
      break;

    default:
      printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (k == -1 || pnum == -1 || mod == -1) {
    printf("Usage: %s -k \"num\" --pnum \"num\" --mod \"num\" \n",
           argv[0]);
    return 1;
  }

  pthread_t threads[pnum];

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  uint64_t result = 1;

  int baseChunkSize = k / pnum;
  int extendedChunksCount = k % pnum;

  int chunkStart = 0;
  struct FactorialArgs args[pnum];
  for (uint32_t i = 0; i < pnum; i++) {
    args[i].mod = mod;
    args[i].mutex = &mutex;
    args[i].result = &result;

    int chunkEnd = chunkStart + baseChunkSize;
    if (i < extendedChunksCount) chunkEnd++;

    args[i].begin = chunkStart + 1;
    args[i].end = chunkEnd;

    chunkStart = chunkEnd;
    if (pthread_create(&threads[i], NULL, ThreadFactorial, (void*)&args[i])) {
      printf("Error: pthread_create failed!\n");
      return 1;
    }
  }

  for (uint32_t i = 0; i < pnum; i++) {
    pthread_join(threads[i], NULL);
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  printf("Factorial: %d\n", result);
  printf("Elapsed time: %fms\n", elapsed_time);

  return 0;
}

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"

int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  bool with_files = false;

  while (true) {
    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
              printf("seed must be a positive number\n");
              return 1;
            }
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
              printf("array size must be a positive number\n");
              return 1;
            }
            break;
          case 2:
            pnum = atoi(optarg);
            if (pnum <= 0) {
              printf("pnum must be a positive number\n");
              return 1;
            }
            break;
          case 3:
            with_files = true;
            break;

          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        with_files = true;
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

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);

  pid_t* pids = malloc(pnum * sizeof(pid_t));
  int* pipes = malloc(pnum * 2 * sizeof(int));

  int baseChunkSize = array_size / pnum;
  int extendedCount = array_size % pnum;
  int j = 0;

  for (int i = 0; i < pnum; i++) {
    int* readDescriptor = pipes + i * 2;
    pipe(readDescriptor);

    int startIndex = j;
    int endIndex = startIndex + baseChunkSize;
    if (i < extendedCount) endIndex++;
    j = endIndex;

    pid_t child_pid = fork();

    if (child_pid >= 0) {
      // successful fork
      pids[i] = child_pid;
      active_child_processes++;
      if (child_pid == 0) {
        struct MinMax chunkResult = GetMinMax(array, startIndex, endIndex);

        if (with_files) {
          char fileName[40];
          sprintf(fileName, "./%d.txt", getpid());
          FILE* temporaryResultsFile = fopen(fileName, "w");
          if (!temporaryResultsFile) {
            printf("Cannot open file %s\n", fileName);
            return 1;
          }

          fprintf(temporaryResultsFile, "%d %d", chunkResult.min, chunkResult.max);
          fclose(temporaryResultsFile);
        } else {
          int* writeDescriptor = readDescriptor + 1;
          write(*writeDescriptor, &chunkResult, sizeof(struct MinMax));

          close(*readDescriptor);
          close(*writeDescriptor);
        }
        return 0;
      }

    } else {
      printf("Fork failed!\n");
      return 1;
    }
  }

  while (active_child_processes > 0) {
    int pid = pids[active_child_processes - 1];
    int code = waitpid(pid, NULL, 0);
    if (!code) {
      printf("Failed to wait for process with id: %d", pid);
      return 1;
    }

    active_child_processes--;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;

  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (with_files) {
      char fileName[40];
      sprintf(fileName, "./%d.txt", pids[i]);
      FILE* temporaryResultsFile = fopen(fileName, "r");

      fscanf(temporaryResultsFile, "%d %d", &min, &max);
      fclose(temporaryResultsFile);
      remove(fileName);
    } else {
      int readDescriptor = pipes[i * 2];
      struct MinMax buff;
      read(readDescriptor, &buff, sizeof(struct MinMax));
      min = buff.min;
      max = buff.max;
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }

  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);
  free(pids);
  free(pipes);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  return 0;
}

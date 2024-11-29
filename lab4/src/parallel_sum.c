#include <err.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "utils.h"
#include "sum.h"

struct SumArgs {
  int *array;
  int begin;
  int end;
};

void *Sum_thread(void *args) {
  struct SumArgs *data = (struct SumArgs *)args;
  int *res = (int *)calloc(1, sizeof(int));
  *res = get_sum(data->array, data->begin, data->end);
  return (void *)res;
}

int main(int argc, char **argv) {

  int *array;
  pthread_t *threads;
  bool flag = false;
  int threads_num = -1;
  int array_size = -1;
  int seed = -1;
  struct timeval start_time, end_time;

  for (;;) {
    int c, option_index;
    struct option options[] = {
      {"threads_num", required_argument, 0, 0},
      {"seed", required_argument, 0, 0},
      {"array_size", required_argument, 0, 0},
      {0, 0, 0, 0}
    };
    option_index = 0;
    c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            threads_num = atoi(optarg);
            if (threads_num <= 0) {
              printf("Invalid value for threads_num\n");
              return 1;
            }
            break;
          case 1:
            seed = atoi(optarg);
            if (seed <= 0) {
              printf("Invalid value for seed\n");
              return 1;
            }
            break;
          case 2:
            array_size = atoi(optarg);
            if (array_size <= 0) {
              printf("Invalid value for array_size\n");
              return 1;
            }
            break;
          default:
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

  if (threads_num == -1 || array_size == -1 || seed == -1) {
    printf("Usage: %s --threads_num \"num\" --seed \"num\" --array_size \"num\" \n", argv[0]);
    return 1;
  }

  gettimeofday(&start_time, NULL);

  array = (int*)malloc(sizeof(int) * array_size);
  threads = (pthread_t*)malloc(sizeof(pthread_t) * threads_num);
  GenerateArray(array, array_size, seed);

  for (int i = 0; i < threads_num; i++) {
    unsigned a, b, coef = array_size / threads_num;
    struct SumArgs *sa = (struct SumArgs*)malloc(sizeof(struct SumArgs));

    a = i * coef;
    b = i == threads_num - 1 ? array_size : (i + 1) * coef;

    sa->array = array;
    sa->begin = a;
    sa->end = b;

    pthread_create(&threads[i], NULL, Sum_thread, sa);
  }

  int sum = 0;
  for (int i = 0; i < threads_num; i++) {
    int *res;
    pthread_join(threads[i], (void **)&res);
    sum += *res;
    free(res);
  }

  gettimeofday(&end_time, NULL);
  double elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (end_time.tv_usec - start_time.tv_usec) / 1000.0;

  printf("> sum: %d\n", sum);
  printf("> elapsed: %lf\n", elapsed_time);

  free(array);
  free(threads);
  return 0;
}

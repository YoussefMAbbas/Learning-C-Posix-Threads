#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_NUM 2
#define ARR_NUM 10
#define HALF_ARR (sizeof(nums) / sizeof(int))

void ErrMsg(const char *msg);
void *Routine(void *args);

int nums[ARR_NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int main(int argc, char *argv[]) {

  pthread_t threads[THREADS_NUM];

  for (int i = 0; i < THREADS_NUM; i++) {
    int *a = malloc(sizeof(int));
    *a = i * HALF_ARR;
    int threadCreated = pthread_create(&threads[i], NULL, &Routine, (void *)a);
    if (threadCreated != 0) {
      ErrMsg("pthread_create()");
    }
  }

  int sum = 0;
  for (int i = 0; i < THREADS_NUM; i++) {
    int *result;
    int threadJoined = pthread_join(threads[i], (void **)&result);
    if (threadJoined != 0) {
      ErrMsg("pthread_join()");
    }
    sum += *result;
    free(result);
  }

  printf("Sum: %d\n", sum);
  return EXIT_SUCCESS;
}

void *Routine(void *args) {
  int *num = (int *)args;
  int sum = 0;

  for (int i = 0; i < (int)HALF_ARR; i++) {
    sum += nums[*num + i];
  }

  *(int *)args = sum;
  pthread_exit(args);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

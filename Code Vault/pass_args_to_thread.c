#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_NUM 10
#define ARR_NUM 10

void ErrMsg(const char *msg);
void *Routine(void *args);

int nums[ARR_NUM] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int main(int argc, char *argv[]) {

  pthread_t threads[THREADS_NUM];

  for (int i = 0; i < THREADS_NUM; i++) {
    int *a = malloc(sizeof(int));
    *a = i;
    int threadCreated = pthread_create(&threads[i], NULL, &Routine, (void *)a);
    if (threadCreated != 0) {
      ErrMsg("pthread_create()");
    }
  }

  for (int i = 0; i < THREADS_NUM; i++) {
    int threadJoined = pthread_join(threads[i], NULL);
    if (threadJoined != 0) {
      ErrMsg("pthread_join()");
    }
  }

  return EXIT_SUCCESS;
}

void *Routine(void *args) {

  int *num = (int *)args;
  printf("Thread num: %d\n", nums[*num]);

  free(args);

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

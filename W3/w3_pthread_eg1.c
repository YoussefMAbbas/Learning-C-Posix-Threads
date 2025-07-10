#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_NUMS 5

void ErrMsg(const char *msg);
void *ListThreads(void *args);

int main(int argc, char *argv[]) {

  pthread_t threads[THREADS_NUMS];
  int threadsNumbers[THREADS_NUMS] = {1, 2, 3, 4, 5};

  for (int i = 0; i < THREADS_NUMS; i++) {
    int threadsCreated = pthread_create(&threads[i], NULL, ListThreads,
                                        (void *)&threadsNumbers[i]);
    if (threadsCreated != 0) {
      ErrMsg("pthread_create()");
    }
  }

  for (int i = 0; i < THREADS_NUMS; i++) {
    int threadsJoined = pthread_join(threads[i], NULL);
    if (threadsJoined != 0) {
      ErrMsg("pthread_join()");
    }
  }

  printf("Complete!\n");

  return EXIT_SUCCESS;
}

void *ListThreads(void *args) {
  int *num = (int *)args;
  printf("Thread:%d finished!\n", *num);
  pthread_exit(NULL); // instead of `return NULL;` looks even better just learned it
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

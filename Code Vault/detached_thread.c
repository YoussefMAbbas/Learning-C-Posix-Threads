#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 5

void *Routine(void *args);
void ErrMsg(const char *msg);

int main(void) {
  pthread_t threads[MAX_THREADS];
  pthread_attr_t detachThreads;

  pthread_attr_init(&detachThreads);
  pthread_attr_setdetachstate(&detachThreads, PTHREAD_CREATE_DETACHED);

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_create(&threads[i], &detachThreads, &Routine, NULL) != 0) {
      ErrMsg("pthread_create()");
    }
  }

  pthread_attr_destroy(&detachThreads);

  pthread_exit(EXIT_SUCCESS);
}

void *Routine(void *args) {

  sleep(1);
  printf("Hello\n");

  pthread_exit((void *)args);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

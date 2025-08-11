#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 9
pthread_barrier_t threadBarrier;

void *Routine();
void ErrMsg(const char *msg);

int main() {

  pthread_t threads[MAX_THREADS];

  if (pthread_barrier_init(&threadBarrier, NULL, 3) != 0) {
    ErrMsg("pthread_barrier_init()");
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, (void *)&Routine, NULL) != 0) {
      ErrMsg("pthread_create()");
    }
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      ErrMsg("pthread_join()");
    }
  }

  if (pthread_barrier_destroy(&threadBarrier) != 0) {
    ErrMsg("pthread_barrier_destroy()");
  }

  return EXIT_SUCCESS;
}

void *Routine() {
  printf("Waiting for barrier to finish!\n");
  sleep(1);
  pthread_barrier_wait(&threadBarrier);
  printf("Passing the barrier!\n");

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

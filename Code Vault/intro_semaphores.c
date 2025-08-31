#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 4

sem_t semaphore;

void *Routine(void *args);
void ErrMsg(const char *msg);

int main(void) {
  pthread_t threads[MAX_THREADS];

  if (sem_init(&semaphore, 0, 2) != EXIT_SUCCESS) {
    ErrMsg("sem_init()");
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, &Routine, NULL) != EXIT_SUCCESS) {
      ErrMsg("pthread_create()");
    }
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != EXIT_SUCCESS) {
      ErrMsg("pthread_join()");
    }
  }

  if (sem_destroy(&semaphore) != EXIT_SUCCESS) {
    ErrMsg("sem_destroy()");
  }

  pthread_exit(EXIT_SUCCESS);
}

void *Routine(void *args) {
  sem_wait(&semaphore);

  sleep(1);
  printf("Hello from thread\n");

  sem_post(&semaphore);

  pthread_exit((void *)args);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

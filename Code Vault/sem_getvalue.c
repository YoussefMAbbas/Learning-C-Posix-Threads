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
  int subscripts[MAX_THREADS];

  if (sem_init(&semaphore, 0, 4) != EXIT_SUCCESS) {
    ErrMsg("sem_init()");
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    subscripts[i] = i + 1;
    if (pthread_create(&threads[i], NULL, &Routine, (void *)&subscripts[i]) !=
        EXIT_SUCCESS) {
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
  int subscript = *(int *)args;
  int semValue;

  sem_wait(&semaphore);
  sleep((unsigned int)subscript + 1);

  sem_getvalue(&semaphore, &semValue);
  printf("After wait, Thread(%d), semaphore value: %d\n", subscript, semValue);

  sem_post(&semaphore);

  sem_getvalue(&semaphore, &semValue);
  printf("After post, Thread(%d), semaphore value: %d\n", subscript, semValue);

  pthread_exit((void *)args);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 1

sem_t semaphore;
int *fuel;

void *Routine(void *args);
void ErrMsg(const char *msg);

int main(void) {
  pthread_t threads[MAX_THREADS];
  fuel = malloc(sizeof(int));
  *fuel = 50;

  if (sem_init(&semaphore, 0, 0) != EXIT_SUCCESS) {
    ErrMsg("sem_init()");
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, &Routine, NULL) != EXIT_SUCCESS) {
      ErrMsg("pthread_create()");
    }
  }

  sem_wait(&semaphore);
  free(fuel);

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

  *fuel += 50;
  printf("Fuel: %d\n", *fuel);

  sem_post(&semaphore);

  pthread_exit((void *)args);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

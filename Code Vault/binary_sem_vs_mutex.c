#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 4

sem_t semaphore;
pthread_mutex_t mLock;
int fuel = 50;

void *Routine1(void *args);
void *Routine2(void *args);
void ErrMsg(const char *msg);

int main(void) {
  pthread_t threads[MAX_THREADS];

  if (sem_init(&semaphore, 0, 0) != EXIT_SUCCESS) {
    ErrMsg("sem_init()");
  }

  /* if (pthread_mutex_init(&mLock, NULL) != EXIT_SUCCESS) { */
  /*   ErrMsg("pthread_mutex_init()"); */
  /* } */

  for (int i = 0; i < MAX_THREADS; i++) {
    if (i % 2 == 0) {
      if (pthread_create(&threads[i], NULL, &Routine1, NULL) != EXIT_SUCCESS) {
        ErrMsg("pthread_create()");
      }
    } else {
      if (pthread_create(&threads[i], NULL, &Routine2, NULL) != EXIT_SUCCESS) {
        ErrMsg("pthread_create()");
      }
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

  /* if (pthread_mutex_destroy(&mLock) != EXIT_SUCCESS) { */
  /*   ErrMsg("pthread_mutex_destroy()"); */
  /* } */

  pthread_exit(EXIT_SUCCESS);
}

void *Routine1(void *args) {

  while (true) {
    sem_wait(&semaphore);
    /* pthread_mutex_lock(&mLock); */
    fuel += 50;
    printf("Fuel: %d\n", fuel);
  }

  pthread_exit((void *)args);
}

void *Routine2(void *args) {

  while (true) {
    sem_post(&semaphore);
    /* pthread_mutex_unlock(&mLock); */
  }

  pthread_exit((void *)args);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

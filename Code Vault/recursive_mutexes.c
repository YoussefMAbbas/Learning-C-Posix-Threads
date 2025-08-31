#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 8

pthread_mutex_t fuelLock;

int fuel = 50;
const int fuelInc = 50;

void *Routine(void *args);
void ErrMsg(const char *msg);

int main(void) {
  pthread_t threads[MAX_THREADS];
  pthread_mutexattr_t recMutexAttr;
  int threadNumber[MAX_THREADS];

  if (pthread_mutexattr_init(&recMutexAttr) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutexattr_init()");
  }
  if (pthread_mutexattr_settype(&recMutexAttr, PTHREAD_MUTEX_RECURSIVE) !=
      EXIT_SUCCESS) {
    ErrMsg("pthread_mutexattr_settype()");
  }

  if (pthread_mutex_init(&fuelLock, &recMutexAttr) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_init()");
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    threadNumber[i] = i + 1;

    if (pthread_create(&threads[i], NULL, &Routine, (void *)&threadNumber[i]) !=
        EXIT_SUCCESS) {
      ErrMsg("pthread_create()");
    }
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != EXIT_SUCCESS) {
      ErrMsg("pthread_join()");
    }
  }

  if (pthread_mutex_destroy(&fuelLock) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_destroy()");
  }
  if (pthread_mutexattr_destroy(&recMutexAttr) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutexattr_destroy()");
  }

  printf("Final Fuel: %d\n", fuel);

  pthread_exit(EXIT_SUCCESS);
}

void *Routine(void *args) {
  int threadNumber = *(int *)args;

  if (pthread_mutex_lock(&fuelLock) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_lock()");
  }
  if (pthread_mutex_lock(&fuelLock) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_lock()");
  }

  fuel += fuelInc;

  printf("Thread:%d, Incremented fuel by %d: %d\n", threadNumber, fuelInc,
         fuel);

  if (pthread_mutex_unlock(&fuelLock) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_unlock()");
  }
  if (pthread_mutex_unlock(&fuelLock) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_unlock()");
  }

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

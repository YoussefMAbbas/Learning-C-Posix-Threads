#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 8

pthread_mutex_t fuelLock;
pthread_mutex_t waterLock;

int fuel = 50;
const int fuelInc = 50;
int water = 50;
const int waterInc = 50;

void *Routine(void *args);
void ErrMsg(const char *msg);

int main(void) {
  pthread_t threads[MAX_THREADS];
  int threadNumber[MAX_THREADS];

  srand((unsigned int)time(NULL));

  if (pthread_mutex_init(&fuelLock, NULL) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_init()");
  }

  if (pthread_mutex_init(&waterLock, NULL) != EXIT_SUCCESS) {
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

  if (pthread_mutex_destroy(&waterLock) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_destroy()");
  }

  printf("Final Fuel: %d\n", fuel);
  printf("Final Water: %d\n", water);

  pthread_exit(EXIT_SUCCESS);
}

void *Routine(void *args) {
  int threadNumber = *(int *)args;

  if (rand() % 2 == 0) {
    if (pthread_mutex_lock(&fuelLock) != EXIT_SUCCESS) {
      ErrMsg("pthread_mutex_lock()");
    }
    sleep(1);
    if (pthread_mutex_lock(&waterLock) != EXIT_SUCCESS) {
      ErrMsg("pthread_mutex_lock()");
    }
  } else {
    if (pthread_mutex_lock(&waterLock) != EXIT_SUCCESS) {
      ErrMsg("pthread_mutex_lock()");
    }
    sleep(1);
    if (pthread_mutex_lock(&fuelLock) != EXIT_SUCCESS) {
      ErrMsg("pthread_mutex_lock()");
    }
  }

  fuel += fuelInc;
  water += waterInc;

  printf("Thread:%d, Incremented fuel by %d: %d, Incremented water by %d: %d\n",
         threadNumber, fuelInc, fuel, waterInc, water);

  if (pthread_mutex_unlock(&fuelLock) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_unlock()");
  }
  if (pthread_mutex_unlock(&waterLock) != EXIT_SUCCESS) {
    ErrMsg("pthread_mutex_unlock()");
  }

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

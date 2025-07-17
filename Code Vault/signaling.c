#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_NUM 2
#define FUEL_MAX 40

pthread_mutex_t threadLocker;
pthread_cond_t threadCond;
unsigned int fuel = 0;

void *FuelStation();
void *CarLine();
void ErrMsg(const char *msg);

int main(int argc, char *argv[]) {

  int mutexInit = pthread_mutex_init(&threadLocker, NULL);
  if (mutexInit != 0) {
    ErrMsg("pthread_mutex_init()");
  }

  int condInit = pthread_cond_init(&threadCond, NULL);
  if (condInit != 0) {
    ErrMsg("pthread_cond_init)");
  }

  pthread_t threads[THREADS_NUM];

  for (int i = 0; i < THREADS_NUM; i++) {

    if (i == 1) {
      int threadCreated = pthread_create(&threads[i], NULL, &CarLine, NULL);
      if (threadCreated != 0) {
        ErrMsg("pthread_create()");
      }
    } else {
      int threadCreated = pthread_create(&threads[i], NULL, &FuelStation, NULL);
      if (threadCreated != 0) {
        ErrMsg("pthread_create()");
      }
    }
  }

  for (int i = 0; i < THREADS_NUM; i++) {
    int threadJoined = pthread_join(threads[i], NULL);
    if (threadJoined != 0) {
      ErrMsg("pthread_join()");
    }
  }

  int mutexDestroy = pthread_mutex_destroy(&threadLocker);
  if (mutexDestroy != 0) {
    ErrMsg("pthread_mutex_destroy()");
  }

  int condDestroy = pthread_cond_destroy(&threadCond);
  if (condDestroy != 0) {
    ErrMsg("pthread_cond_destroy)");
  }

  return EXIT_SUCCESS;
}

void *FuelStation() {

  for (int i = 0; i < 5; i++) {
    int mutexLock = pthread_mutex_lock(&threadLocker);
    if (mutexLock != 0) {
      ErrMsg("pthread_mutex_lock()");
    }

    fuel += 10;
    if (fuel <= FUEL_MAX) {
      printf("Fuel number is %d\n", fuel);
    }

    int mutexUnlock = pthread_mutex_unlock(&threadLocker);
    if (mutexUnlock != 0) {
      ErrMsg("pthread_mutex_unlock()");
    }

    int condBroadCast = pthread_cond_broadcast(&threadCond);
    if (condBroadCast != 0) {
      ErrMsg("pthread_cond_signal()");
    }
    sleep(1);
  }

  pthread_exit(NULL);
}

void *CarLine() {

  int mutexLock = pthread_mutex_lock(&threadLocker);
  if (mutexLock != 0) {
    ErrMsg("pthread_mutex_lock()");
  }

  while (fuel <= FUEL_MAX) {
    printf("Waiting for the car to move!\n");

    int condWait = pthread_cond_wait(&threadCond, &threadLocker);
    if (condWait != 0) {
      ErrMsg("pthread_cond_wait()");
    }
  }

  printf("You are next!\n");
  fuel = 0;

  int mutexUnlock = pthread_mutex_unlock(&threadLocker);
  if (mutexUnlock != 0) {
    ErrMsg("pthread_mutex_unlock()");
  }

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

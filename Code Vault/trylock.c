#include <asm-generic/errno-base.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_NUM 5

void ErrMsg(const char *msg);
void *Routine();

pthread_mutex_t threadLocker;

int main(int argc, char *argv[]) {

  int mutexInit = pthread_mutex_init(&threadLocker, NULL);
  if (mutexInit != 0) {
    ErrMsg("pthread_mutex_init()");
  }

  pthread_t threads[THREADS_NUM];
  for (int i = 0; i < THREADS_NUM; i++) {
    int threadCreated = pthread_create(&threads[i], NULL, &Routine, NULL);
    if (threadCreated != 0) {
      ErrMsg("pthread_create()");
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

  return EXIT_SUCCESS;
}

void *Routine() {
  int mutexTrylock = pthread_mutex_trylock(&threadLocker);
  if (mutexTrylock == 0) {

    printf("Thread got locked!\n");

    int mutexUnlock = pthread_mutex_unlock(&threadLocker);
    if (mutexUnlock != 0) {
      ErrMsg("pthread_mutex_unlock()");
    }

  } else if (mutexTrylock == EBUSY) {
    printf("MUTEX is busy!\n");

  } else {
    ErrMsg("pthread_mutex_trylock()");
  }

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_NUMS 3

void ErrMsg(const char *msg);
void *CounterThreads();

int counter = 0;
pthread_mutex_t threadLocker;

int main(int argc, char *argv[]) {

  if (pthread_mutex_init(&threadLocker, NULL) != 0) {
    ErrMsg("pthread_mutex_init()");
  }

  pthread_t threads[THREADS_NUMS];

  for (int i = 0; i < THREADS_NUMS; i++) {
    int threadsCreated =
        pthread_create(&threads[i], NULL, CounterThreads, NULL);
    if (threadsCreated != 0) {
      ErrMsg("pthread_create()");
    }
  }

  for (int i = 0; i < THREADS_NUMS; i++) {
    int threadsJoined = pthread_join(threads[i], NULL);
    if (threadsJoined != 0) {
      ErrMsg("pthread_join()");
    }
  }

  if (pthread_mutex_destroy(&threadLocker) != 0) {
    ErrMsg("pthread_mutex_destroy()");
  }

  printf("Complete!\n");

  return EXIT_SUCCESS;
}

void *CounterThreads() {

  if (pthread_mutex_lock(&threadLocker) != 0) {
    ErrMsg("pthread_mutex_lock()");
  }

  counter += 1;
  printf("Counter: %d\n", counter);

  if (pthread_mutex_unlock(&threadLocker) != 0) {
    ErrMsg("pthread_mutex_unlock()");
  }
  pthread_exit(NULL);

}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

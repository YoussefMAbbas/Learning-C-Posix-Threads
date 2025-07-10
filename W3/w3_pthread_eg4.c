#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t threadLocker;
pthread_cond_t threadCond;
bool isThreadReady = false;

void ErrMsg(const char *msg);
void *WaitSignal();
void *SendSignal();

int main(int argc, char *argv[]) {

  if (pthread_mutex_init(&threadLocker, NULL) != 0) {
    ErrMsg("pthread_mutex_init()");
  }
  if (pthread_cond_init(&threadCond, NULL) != 0) {
    ErrMsg("pthread_cond_init()");
  }

  pthread_t thread1, thread2;

  int thread1Created = pthread_create(&thread1, NULL, WaitSignal, NULL);
  if (thread1Created != 0) {
    ErrMsg("pthread_create()");
  }
  int thread2Created = pthread_create(&thread2, NULL, SendSignal, NULL);
  if (thread2Created != 0) {
    ErrMsg("pthread_create()");
  }

  int thread1Joined = pthread_join(thread1, NULL);
  if (thread1Joined != 0) {
    ErrMsg("pthread_join()");
  }
  int thread2Joined = pthread_join(thread2, NULL);
  if (thread2Joined != 0) {
    ErrMsg("pthread_join()");
  }

  if (pthread_mutex_destroy(&threadLocker) != 0) {
    ErrMsg("pthread_mutex_destroy()");
  }
  if (pthread_cond_destroy(&threadCond) != 0) {
    ErrMsg("pthread_cond_destroy()");
  }

  printf("Complete!\n");

  return EXIT_SUCCESS;
}

void *WaitSignal() {

  if (pthread_mutex_lock(&threadLocker) != 0) {
    ErrMsg("pthread_mutex_lock()");
  }

  while (!isThreadReady) {
    printf("Waiting for signal\n");
    if (pthread_cond_wait(&threadCond, &threadLocker)) {
      ErrMsg("pthread_cond_wait()");
    }
  }

  printf("Received from signal\n");

  if (pthread_mutex_unlock(&threadLocker) != 0) {
    ErrMsg("pthread_mutex_unlock()");
  }

  pthread_exit(NULL);
}

void *SendSignal() {

  if (pthread_mutex_lock(&threadLocker) != 0) {
    ErrMsg("pthread_mutex_lock()");
  }

  isThreadReady = true;

  if (pthread_cond_signal(&threadCond)) {
    ErrMsg("pthread_cond_signal()");
  }

  if (pthread_mutex_unlock(&threadLocker) != 0) {
    ErrMsg("pthread_mutex_unlock()");
  }

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

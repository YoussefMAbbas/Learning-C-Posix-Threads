#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_NO 4

pthread_mutex_t routineMutex;
int mails = 0;

void ErrMsg(const char *msg);
void *Routine();

int main(int argc, char *argv[]) {

  if (pthread_mutex_init(&routineMutex, NULL) != 0) {
    ErrMsg("pthread_mutex_init()");
  }

  pthread_t threads[THREADS_NO];
  for (int i = 0; i < THREADS_NO; i++) {
    if (pthread_create(&threads[i], NULL, &Routine, NULL) != 0) {
      ErrMsg("pthread_create()");
    }
    printf("Thread:%d started \n", (i + 1));
  }

  for (int i = 0; i < THREADS_NO; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      ErrMsg("pthread_join()");
    }
    printf("Thread:%d ended\n", (i + 1));
  }

  if (pthread_mutex_destroy(&routineMutex) != 0) {
    ErrMsg("pthread_mutex_destroy()");
  }

  printf("Final Mails: %d\n", mails);

  return EXIT_SUCCESS;
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void *Routine() {

  if (pthread_mutex_lock(&routineMutex) != 0) {
    perror("pthread_mutex_lock()");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 1000000; i++) {
    mails++;
  }

  if (pthread_mutex_unlock(&routineMutex) != 0) {
    ErrMsg("pthread_mutex_unlock()");
  }

  return NULL;
}

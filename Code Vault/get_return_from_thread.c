#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void ErrMsg(const char *msg);
void *Routine();

int main(int argc, char *argv[]) {

  srand((unsigned int)time(NULL));
  pthread_t thread;
  int *res;

  int threadCreated = pthread_create(&thread, NULL, &Routine, NULL);
  if (threadCreated != 0) {
    ErrMsg("pthread_create()");
  }

  int threadJoined = pthread_join(thread, (void **)&res);
  if (threadJoined != 0) {
    ErrMsg("pthread_join()");
  }

  printf("MRes addr: %p\n", (void *)res);
  printf("MRes value: %d\n", *res);
  free(res);

  return EXIT_SUCCESS;
}

void *Routine() {
  int num = (rand() % 6) + 1;

  int *res = malloc(sizeof(int));
  if (res == NULL) {
    ErrMsg("malloc()");
  }

  *res = num;
  printf("RRes addr: %p\n", (void *)res);
  printf("RRes value: %d\n", *res);
  pthread_exit(res);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

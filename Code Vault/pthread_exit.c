#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_THREADS 1

void *Routine();
void ErrMsg(const char *msg);

int main() {

  srand((unsigned int)time(NULL));

  pthread_t threads[MAX_THREADS];
  int *res;

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, &Routine, NULL) != 0) {
      ErrMsg("pthread_create()");
    }
  }

  pthread_exit(NULL);

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_join(threads[i], (void **)&res) != 0) {
      ErrMsg("pthread_join()");
    }
  }

  printf("Main result: %d\n", *res);
  free(res);

  return EXIT_SUCCESS;
}

void *Routine() {
  const int randomValue = rand() % 10 + 1;
  int *res = malloc(sizeof(int));
  *res = randomValue;
  printf("Thread result: %d\n", *res);

  pthread_exit((void *)res);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

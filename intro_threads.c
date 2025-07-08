#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *routine() {
  printf("Hello from thread!\n");
  sleep(1);
  printf("Bye from thread!\n");
  sleep(1);
  return NULL;
}

int main(int argc, char *argv[]) {

  pthread_t thread1, thread2;

  if (pthread_create(&thread1, NULL, &routine, NULL) != 0) {
    perror("pthread_create()");
    exit(EXIT_FAILURE);
  }
  if (pthread_join(thread1, NULL) != 0) {
    perror("pthread_join()");
    exit(EXIT_FAILURE);
  }

  if (pthread_create(&thread2, NULL, &routine, NULL) != 0) {
    perror("pthread_create()");
    exit(EXIT_FAILURE);
  }
  if (pthread_join(thread2, NULL) != 0) {
    perror("pthread_join()");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

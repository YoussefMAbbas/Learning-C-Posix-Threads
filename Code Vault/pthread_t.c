#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define MAX_THREADS 2

void *Routine();
void ErrMsg(const char *msg);

int main() {
  pthread_t threads[MAX_THREADS];

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, &Routine, NULL) != 0) {
      ErrMsg("pthread_create()");
    }
  }

  pthread_exit(NULL);

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      ErrMsg("pthread_join()");
    }
  }

  return EXIT_SUCCESS;
}

void *Routine() {

  /* Thread ID at POSIX thread API Level */
  printf("%lu\n", pthread_self());

  /* Thread ID at operating system Level
   * NOTE: This line works with Linux only
   */
  printf("%d\n", (pid_t)syscall(SYS_gettid));

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

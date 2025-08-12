#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_THREADS 9
#define DICE_MAX 12

pthread_barrier_t threadBarrier;

int rolledDice[MAX_THREADS] = {0};
bool isDiceBigger[MAX_THREADS] = {false};

void *Routine(void *args);
void ErrMsg(const char *msg);

int main() {
  srand((unsigned int)time(NULL));

  pthread_t threads[MAX_THREADS];

  if (pthread_barrier_init(&threadBarrier, NULL, MAX_THREADS) != 0) {
    ErrMsg("pthread_barrier_init()");
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    int *index = malloc(sizeof(int));
    *index = i;
    if (pthread_create(&threads[i], NULL, (void *)&Routine, index) != 0) {
      ErrMsg("pthread_create()");
    }
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    if (rolledDice[i] == DICE_MAX) {
      isDiceBigger[i] = true;
    } else {
      isDiceBigger[i] = false;
    }
  }

  for (int i = 0; i < MAX_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      ErrMsg("pthread_join()");
    }
  }

  if (pthread_barrier_destroy(&threadBarrier) != 0) {
    ErrMsg("pthread_barrier_destroy()");
  }

  return EXIT_SUCCESS;
}

void *Routine(void *args) {
  int index = *(int *)args;
  rolledDice[index] = rand() % DICE_MAX + 1;

  /* After setting a random value to each thread we wait and check for the
   * winner in the main function and then we continue */
  pthread_barrier_wait(&threadBarrier);

  if (isDiceBigger[index]) {
    printf("Thread %d won, dice %d\n", (index + 1), rolledDice[index]);
  } else {
    printf("Thread %d lost, dice %d\n", (index + 1), rolledDice[index]);
  }

  free(args);

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS_MAX 3

pthread_mutex_t mutex_shelf_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_shelf_B = PTHREAD_MUTEX_INITIALIZER;

void *moveItem(void *arg);
void ErrMsg(const char *msg);

int main() {
  pthread_t workers[THREADS_MAX];
  long workerNumber = 1;

  if (pthread_mutex_init(&mutex_shelf_A, NULL) != 0) {
    ErrMsg("pthread_mutex_init()");
  }
  if (pthread_mutex_init(&mutex_shelf_B, NULL) != 0) {
    ErrMsg("pthread_mutex_init()");
  }

  for (int i = 0; i < THREADS_MAX; i++) {
    if (pthread_create(&workers[i], NULL, &moveItem, (void *)workerNumber) !=
        0) {
      ErrMsg("pthread_create()");
    }
    workerNumber++;
  }

  for (int i = 0; i < THREADS_MAX; i++) {
    if (pthread_join(workers[i], NULL) != 0) {
      ErrMsg("pthread_join()");
    }
  }

  if (pthread_mutex_destroy(&mutex_shelf_A) != 0) {
    ErrMsg("pthread_mutex_destroy()");
  }
  if (pthread_mutex_destroy(&mutex_shelf_B) != 0) {
    ErrMsg("pthread_mutex_destroy()");
  }

  return EXIT_SUCCESS;
}

void *moveItem(void *arg) {
  long worker_id = (long)arg;

  while (true) {
    if (pthread_mutex_trylock(&mutex_shelf_A) == 0) {
      printf("Worker %ld: locked Shelf A\n", worker_id);

      if (pthread_mutex_trylock(&mutex_shelf_B) == 0) {
        printf("Worker %ld: locked Shelf B\n", worker_id);

        // Both shelves are locked – move item
        printf("Worker %ld: moving item from Shelf A to Shelf B...\n",
               worker_id);
        sleep(1); // simulate item movement

        // Unlock both
        if (pthread_mutex_unlock(&mutex_shelf_B)) {
          ErrMsg("pthread_mutex_unlock()");
        }
        if (pthread_mutex_unlock(&mutex_shelf_A)) {
          ErrMsg("pthread_mutex_unlock()");
        }

        printf("Worker %ld: finished moving item and released shelves\n",
               worker_id);
        break;
      } else {
        // Shelf B is EBUSY – release Shelf A and retry
        printf("Worker %ld: Shelf B busy, releasing Shelf A\n", worker_id);
        if (pthread_mutex_unlock(&mutex_shelf_A)) {
          ErrMsg("pthread_mutex_unlock()");
        }
        sleep(1); // wait and retry
      }
    } else {
      // Shelf A is EBUSY
      sleep(1);
    }
  }

  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

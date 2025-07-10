#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void ErrMsg(const char *msg);
void *UserInfo(void *args);

typedef struct {
  int age;
  char firstName[30];
} User;

int main(int argc, char *argv[]) {

  pthread_t thread;
  User user = {.firstName = "Joe", .age = 20};

  int threadCreated = pthread_create(&thread, NULL, UserInfo, (void *)&user);
  if (threadCreated != 0) {
    ErrMsg("pthread_create()");
  }

  int threadJoined = pthread_join(thread, NULL);
  if (threadJoined != 0) {
    ErrMsg("pthread_join()");
  }

  printf("Complete!\n");

  return EXIT_SUCCESS;
}

void *UserInfo(void *args) {
  User *user = (User *)args;
  printf("First Name: %s\n", user->firstName);
  printf("Age: %d\n", user->age);
  pthread_exit(NULL);
}

void ErrMsg(const char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

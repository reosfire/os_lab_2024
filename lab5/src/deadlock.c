#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

pthread_mutex_t mut1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;

void first_thread_handler(void* _) {
  while (true) {
    printf("Thread 1 tries to aquire mut1 \n");
    pthread_mutex_lock(&mut1);
    printf("Thread 1 aquired mut1 \n");
    pthread_mutex_lock(&mut2);
    printf("Thread 1 aquired mut2 \n");

    pthread_mutex_unlock(&mut1);
    pthread_mutex_unlock(&mut2);
  }
}

void second_thread_handler(void* _) {
  while (true) {
    printf("Thread 2 tries to aquire mut2 \n");
    pthread_mutex_lock(&mut2);
    printf("Thread 2 aquired mut2 \n");
    pthread_mutex_lock(&mut1);
    printf("Thread 2 aquired mut1 \n");

    pthread_mutex_unlock(&mut2);
    pthread_mutex_unlock(&mut1);
  }
}

int main() {
  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, (void *)first_thread_handler, NULL);
  pthread_create(&thread2, NULL, (void *)second_thread_handler, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  
  return 0;
}

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "protected_buffer.h"

#define MAXDURATION 5
#define MAXTHREADS 256
int last_thread = -1;
int first_thread = 0;
pthread_t threads_table[MAXTHREADS];
protected_buffer_t* finished_threads;
// Terminate thread
void process_exit() {
  protected_buffer_put(finished_threads,(void *)pthread_self());
  pthread_exit(NULL);
}

// Find a terminated child process and return its tid
pthread_t process_wait() {
  pthread_t thread = NULL;
  if (first_thread <= last_thread) {
    thread = threads_table[first_thread];
    first_thread++;
  }
  return thread;
}

// Main procedure for a child process
void thread_main(void){
  pthread_t self = pthread_self();
  int       duration = random()%MAXDURATION + MAXDURATION/2;

  printf("thread (%p) sleeping for %d s\n", (void *)self, duration);
  sleep(duration);
  process_exit();
}

int main(int argc, char *argv[]){
  if (argc != 2){ 
    printf("Usage : %s <n_threads>\n", argv[0]);
    exit(1);
  } 
  struct timeval s = {0, 0};
  struct timeval t;
  
  finished_threads = protected_buffer_init(MAXTHREADS);
  srandom(time(NULL));
  gettimeofday(&s, NULL);
  last_thread = atoi(argv[1]) - 1;


  // Creer autant de threads que demandé en ligne de commande
  int n;
  for (n = 0; n <= last_thread; n++) {
     usleep(1000);
     pthread_create(&threads_table[n],NULL,thread_main,NULL);
  }

  // Attendre la terminaison des threads dans un certain ordre
  int counter = last_thread;
  pthread_t thread;
  while(counter >= 0){
     thread = protected_buffer_get(finished_threads); 
     gettimeofday(&t, NULL);
     printf("thread (%p) join after %d s\n", (void *)thread, (int)(t.tv_sec -s.tv_sec));
 	 counter--;
     printf("rest %d threads\n",counter);
  }
    printf("all threads finish\n");
 /* 
  pthread_t thread;
  while ((thread = process_wait()) != NULL) {
    pthread_join(thread,NULL);
	struct timeval t;
    gettimeofday(&t, NULL);
    printf("thread (%p) join after %d s\n", (void *)thread, (int)(t.tv_sec -s.tv_sec));
  }
  */
  return 0;

}






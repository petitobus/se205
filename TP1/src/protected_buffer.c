#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include "circular_buffer.h"
#include "protected_buffer.h"
pthread_mutex_t m1,m2;
pthread_cond_t vide,plein;
protected_buffer_t * protected_buffer_init(int length) {
  protected_buffer_t * b;
  pthread_mutex_init(&m1,NULL);
  pthread_mutex_init(&m2,NULL);
  pthread_cond_init(&vide,NULL);
  pthread_cond_init(&plein,NULL);
  b = (protected_buffer_t *)malloc(sizeof(protected_buffer_t));
  b->buffer = circular_buffer_init(length);
  return b;
}

void * protected_buffer_get(protected_buffer_t * b){
  struct timeval t;
  struct timeval s = {0, 0};
  void * d;
  pthread_mutex_lock(&m1);
  srandom(time(NULL));
  gettimeofday(&s, NULL);
  d = circular_buffer_get(b->buffer);
  while(d==NULL){
    pthread_cond_wait(&vide,&m1);
    d = circular_buffer_get(b->buffer);
    gettimeofday(&t, NULL);
    printf("consomeur waiting en vide for %d s\n", (int)(t.tv_sec -s.tv_sec));
  }
  pthread_cond_broadcast(&plein);
  pthread_mutex_unlock(&m1);
  return d;
}

int protected_buffer_put(protected_buffer_t * b, void * d){
  pthread_mutex_lock(&m2);
  while(circular_buffer_put(b->buffer, d)==0){
    pthread_cond_wait(&plein,&m2);
  }
  pthread_cond_broadcast(&vide);
  pthread_mutex_unlock(&m2);
  return 1;
}
   

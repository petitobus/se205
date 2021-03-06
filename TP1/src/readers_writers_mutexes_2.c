#include "readers_writers_mutexes_2.h"
#define QUEUE_SIZE 20

const char READER_K = 'R';
const char WRITER_K = 'W';
const char UNUSED_K = 'U';

void * READER;
void * WRITER;
void * UNUSED;

void rw_mutex_init (rw_mutex_t * rw_mutex, int n_threads){
  READER = (void *) &READER_K;
  WRITER = (void *) &WRITER_K;
  UNUSED = (void *) &UNUSED_K;
  rw_mutex->n_readers = 0;
  rw_mutex->owner = UNUSED;
  rw_mutex->queue = circular_buffer_init(QUEUE_SIZE);
}

void rw_mutex_read_lock (rw_mutex_t *rw_mutex, thread_conf_t *conf){

  pthread_mutex_lock(&(rw_mutex->mutex));
  while(circular_buffer_read(rw_mutex->queue)!=NULL)pthread_cond_wait(&(rw_mutex->guard), &(rw_mutex->mutex));
  rw_mutex->owner = READER;
  rw_mutex->n_readers++;
  pthread_mutex_unlock(&(rw_mutex->mutex));
  int i;
  for (i=0; i < rw_mutex->n_readers; i++) printf ("  ");
  printf ("reader (%ld) : enter r = %d v = %ld\n",
	  conf->identifier, rw_mutex->n_readers, shared_variable);
}

void rw_mutex_read_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  int i;
  for (i=0; i < rw_mutex->n_readers; i++) printf ("  ");
  printf ("reader (%ld) : leave r = %d v = %ld\n",
	  conf->identifier, rw_mutex->n_readers, shared_variable);
  pthread_mutex_lock(&(rw_mutex->mutex));
  rw_mutex->n_readers--;
  rw_mutex->owner = UNUSED;
  pthread_mutex_unlock(&(rw_mutex->mutex));
}

void rw_mutex_writer_lock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  int r;
  while(!r){
  pthread_mutex_lock(&(rw_mutex->mutex));
  rw_mutex->owner = WRITER;
  r=circular_buffer_put(rw_mutex->queue, &conf->identifier);
  pthread_mutex_unlock(&(rw_mutex->mutex));
  }
  while(rw_mutex->n_readers>0);
  pthread_mutex_lock(&(rw_mutex->mutex));
}

void rw_mutex_writer_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  circular_buffer_get(rw_mutex->queue);  
  //in the condition no writer is waiting in the queue, notify all reader
  if(circular_buffer_read(rw_mutex->queue)==NULL) pthread_cond_broadcast(&(rw_mutex->guard));
  rw_mutex->owner = UNUSED;
  pthread_mutex_unlock(&(rw_mutex->mutex));
}










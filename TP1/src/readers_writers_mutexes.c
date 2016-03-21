#include "readers_writers_mutexes.h"

pthread_mutex_t w_mtx,r_mtx,mtx;
int counter=0;
void rw_mutex_init (rw_mutex_t * rw_mutex){
	pthread_mutex_init(&w_mtx,NULL);	
	pthread_mutex_init(&r_mtx,NULL);	
	pthread_mutex_init(&mtx,NULL);	
}

void rw_mutex_read_lock (rw_mutex_t *rw_mutex, thread_conf_t *conf){
  pthread_mutex_lock(&w_mtx);
  pthread_mutex_unlock(&w_mtx);
  pthread_mutex_lock(&r_mtx);
    counter++;
    if(counter==1) pthread_mutex_lock(&mtx);
  pthread_mutex_unlock(&r_mtx);
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
  pthread_mutex_lock(&r_mtx);
    counter--;
    if(counter==0) pthread_mutex_unlock(&mtx);
  pthread_mutex_unlock(&r_mtx);

}

void rw_mutex_writer_lock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
	pthread_mutex_lock(&w_mtx);
    pthread_mutex_lock(&mtx);
}

void rw_mutex_writer_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
	pthread_mutex_unlock(&w_mtx);
    pthread_mutex_unlock(&mtx);
}

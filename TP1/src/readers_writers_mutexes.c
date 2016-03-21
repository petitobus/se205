#include "readers_writers_mutexes.h"

pthread_mutex_t mtx1,mtx2,mtx3;

void rw_mutex_init (rw_mutex_t * rw_mutex){
	pthread_mutex_init(mtx1,NULL);	
	pthread_mutex_init(mtx2,NULL);	
	pthread_mutex_init(mtx3,NULL);	
}

void rw_mutex_read_lock (rw_mutex_t *rw_mutex, thread_conf_t *conf){
  pthread_mutex_lock(mtx1);
  pthread_mutex_unlock(mtx1);
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
}

void rw_mutex_writer_lock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
	pthread_mutex_lock(mtx1);
}

void rw_mutex_writer_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
	pthread_mutex_unlock(mtx1);
}

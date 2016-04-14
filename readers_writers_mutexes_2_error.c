#include "readers_writers_mutexes_2.h"

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
  //Dans ce modele, le queue est SEULEMENT POUR WRITER, qui n'est pas le même idée le prof 
  rw_mutex->queue = circular_buffer_init(20);
  pthread_mutex_init(&(rw_mutex->mutex),NULL);
  pthread_cond_init(&(rw_mutex->guard),NULL);
}

void rw_mutex_read_lock (rw_mutex_t *rw_mutex, thread_conf_t *conf){
  //prendre le lock est verifié la condition d'attends;
  pthread_mutex_lock(&(rw_mutex->mutex));
  //Si le owner est WRITER, il faut attendre, Sinon(UNUSED ou READER in SC), le READER entre la SC directement.
  while(rw_mutex->owner == WRITER){
  //Dans la condition d'attends,si il y a des WRITER arrive, quitté l'attend et mise à la queue de condition variable.
    if(circular_buffer_size(rw_mutex->queue)!=0){
        printf("some writer is in waiting queue\n");
		//attendre la rappel du WRITER
        pthread_cond_wait(&(rw_mutex->guard),&(rw_mutex->mutex));
    }
  }
  //Entrer la SC
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
  rw_mutex->n_readers--;
  if(rw_mutex->n_readers==0){
    rw_mutex->owner = UNUSED;
  }
}

void rw_mutex_writer_lock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  //registre un WRITER qui attend
  circular_buffer_put(rw_mutex->queue, WRITER);
  pthread_mutex_lock(&(rw_mutex->mutex));
  while(rw_mutex->owner != UNUSED);
  circular_buffer_get(rw_mutex->queue);
  rw_mutex->owner = WRITER;
  pthread_cond_broadcast(&(rw_mutex->guard));
  pthread_mutex_unlock(&(rw_mutex->mutex));
}

void rw_mutex_writer_unlock (rw_mutex_t *rw_mutex, thread_conf_t * conf){
  rw_mutex->owner = UNUSED;
}

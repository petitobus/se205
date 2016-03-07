#include <pthread.h>
#include <stdlib.h>
#include "circular_buffer.h"
#include "protected_buffer.h"

protected_buffer_t * protected_buffer_init(int length) {
  protected_buffer_t * b;
  b = (protected_buffer_t *)malloc(sizeof(protected_buffer_t));
  b->buffer = circular_buffer_init(length);
  return b;
}

void * protected_buffer_get(protected_buffer_t * b){
  void * d;
  d = circular_buffer_get(b->buffer);
  return d;
}

int protected_buffer_put(protected_buffer_t * b, void * d){
  circular_buffer_put(b->buffer, d);
  return 1;
}
   

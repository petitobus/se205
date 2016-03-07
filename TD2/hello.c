#include <stdio.h>
#include <pthread.h>
typedef unsigned long int pthread_t;

void *myThread(void* parameter){
    pthread_t pid = pthread_self();
    printf("Hello Thread %ld!\n",pid);
	return NULL;
}
int main(){
    pthread_t th;//createandstartthread
    pthread_create(&th,NULL,myThread,NULL);
    pthread_join(th,NULL);//waitforthreadtoterminate
    printf("Hello Main!\n");
    return 0;
}

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 8000
#define NUM_HILOS 4

void *funHilo(void*arg);
pthread_mutex_t bloqueo;
int count;
int main(){
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS] , *res;
	count = 0;
	pthread_mutex_init(&bloqueo,NULL);
	register int nh;

	printf("creando hilos \n");
	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL,funHilo,(void*)&nhs[nh]);
	}
	for(nh =0; nh<NUM_HILOS; nh++){

		pthread_join(tids[nh], (void **)&res);
		printf("Hilo %d terminado", *res);
	}
	pthread_mutex_destroy(&bloqueo);
	return 0;
}
void *funHilo(void *arg){

	register int i = 0;
	int nh = *(int*)arg;

	count++;
	printf("Iniciando Hilo %d XD con contador %d\n",nh, count);
	while((--i));
	//sleep(5);
	printf("terminado Hilo %d XD con contador %d\n",nh, count);
		pthread_mutex_lock(&bloqueo);
	pthread_exit(arg);
}
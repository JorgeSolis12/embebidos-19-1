#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define N 64
#define NUM_HILOS 4

void *funHilo(void*arg);
int * memoria();
void llenarArreglo(int * datos);
void imprimir(int *datos);	
void producto(void );
int *A;
int promedio = 0;

pthread_mutex_t bloqueo;

int main(){
	register int nh;
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS] , *res;

	pthread_mutex_init(&bloqueo,NULL);
	A=memoria();
	llenarArreglo(A);
	imprimir(A);

	printf("creando hilos \n");
	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL,funHilo,(void*)&nhs[nh]);
	}
	for(nh =0; nh<NUM_HILOS; nh++){
		
		pthread_join(tids[nh], (void **)&res);
	}

	//promedio = promedio/N;
	printf("promedio en hilo : %d\n",promedio>>6);
	pthread_mutex_destroy(&bloqueo);
	free(A);
	return 0;
}
void *funHilo(void *arg){
	int prom = 0;
	register int i = 0;
	int nh = *(int*)arg;
	int tambloque=N/NUM_HILOS;
	int inicio = nh*tambloque;
	int final= inicio+tambloque;

	for(i=inicio;i<final;i++){
		
		prom += A[i];
	}
		pthread_mutex_lock(&bloqueo);
		promedio += prom;
		pthread_mutex_unlock(&bloqueo);
	pthread_exit(arg);
}
int * memoria(){
	int *mem;
	mem = (int*)malloc(sizeof(int)*N);
	if(!mem){

		perror("error al hacer la memoria");
		exit(EXIT_FAILURE);
	}
	return mem;
}

void llenarArreglo(int * datos){
	srand(getpid());
	register int i;
	for(i=0;i<N;i++){
		datos[i] = rand()%255;
	}
}
void imprimir(int * datos){
	register int i;
	for(i = 0; i<N;i++){
		if(!(i%8)){
			printf("\n");
		}
		printf("%5d",datos[i]);
		printf(",");
	}
	printf("\n");
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define N 32
#define NUM_HILOS 4

void *funHilo(void*arg);
int * memoria();
void llenarArreglo(int * datos);
void imprimir(int *datos);	
void producto(void );

int *A, *B;
int punto = 0;

pthread_mutex_t bloqueo;

int main(){
	register int nh;
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS] , *res;

	pthread_mutex_init(&bloqueo,NULL);
	//srand(time(0));
	A=memoria();
	llenarArreglo(A);
	imprimir(A);
	B=memoria();
	llenarArreglo(B);
	imprimir(B);

	printf("creando hilos \n");
	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL,funHilo,(void*)&nhs[nh]);
	}
	for(nh =0; nh<NUM_HILOS; nh++){
		
		pthread_join(tids[nh], (void **)&res);
	}

	printf("producto punto en hilo : %d\n",punto);
	pthread_mutex_destroy(&bloqueo);
	free(A);
	free(B);
	return 0;
}
void *funHilo(void *arg){
	int point = 0;
	int p = 0;
	register int i = 0;
	int nh = *(int*)arg;
	int tambloque=N/NUM_HILOS;
	int inicio = nh*tambloque;
	int final= inicio+tambloque;

	for(i=inicio;i<final;i++){
		
		p = A[i]*B[i];
		point += p;
	}
		pthread_mutex_lock(&bloqueo);
		punto += point;
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

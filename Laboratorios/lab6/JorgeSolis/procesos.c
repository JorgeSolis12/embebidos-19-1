#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define N 64
#define NUM_HILOS 4

int * memoria();
void llenarArreglo(int * datos);
void imprimir(int *datos);	
void producto(void );
int *A,*B,*C;

int main(){
	srand(getpid());
	A=memoria();
	llenarArreglo(A);
	imprimir(A);
	B=memoria();
	llenarArreglo(B);
	imprimir(B);
	C=memoria();
	llenarArreglo(C);
	imprimir(C);

	free(A);
	free(B);
	free(C);
	return 0;
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
		if(!(i%16)){
			printf("\n");
		}
		printf("%3d",datos[i]);
		printf(",");
	}
	printf("\n");
}

void producto(void){
	register int i =0;
	for(i=0;i<N;i++){
		C[i]= (A[i]) * (B[i]);
	}
	printf("\n");
}
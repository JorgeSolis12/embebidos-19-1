#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "defs.h"

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
	srand(getpid());
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
		printf("%d",datos[i]);
		printf(",");
	}
	printf("\n");
}
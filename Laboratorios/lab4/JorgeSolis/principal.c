#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "defs.h"
#include "datos.h"
#include "procesamiento.h"
#include "procesos.h"

int main(){
	pid_t pid;
	int *datos;
	datos = memoria();
	llenarArreglo(datos);
	imprimir(datos);
	printf("probando procesos \n");
	for(int i = 0; i< NUM_PROC; i++){
    	 pid = fork();
		 if(pid == -1){
		 	perror("No se pudo crear el proceso");
		 	exit(EXIT_FAILURE);
		 }
		 if(!pid){
		  	procesoHijo(i, datos); 
		 }

	}
	procesoPadre();
	free(datos);
	return 0;
}
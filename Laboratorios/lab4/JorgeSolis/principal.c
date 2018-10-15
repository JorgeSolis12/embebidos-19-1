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
	int *datos,pipefd[2],pipe_status;
	datos = memoria();
	llenarArreglo(datos);
	imprimir(datos);
	printf("probando procesos \n");
	pipe_status =pipe(pipefd);
	if(pipe_status==-1){
		perror("Error al crear el pipe");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i< NUM_PROC; i++){
    	 pid = fork();
		 if(pid == -1){
		 	perror("No se pudo crear el proceso");
		 	exit(EXIT_FAILURE);
		 }
		 if(!pid){
		  	procesoHijo(i, datos,pipefd); 
		 }

	}
	procesoPadre(pipefd);
	free(datos);
	return 0;
}
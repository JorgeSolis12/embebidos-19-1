#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "defs.h"

void procesoHijo(int np, int *datos){
	int mayor, menor,*orden,promedio;
	 printf("proseso hijo %d, ejecutando con el PID: %d \n",np, getpid()); 
	 //while(1);
	 if(np == 0){
	 	mayor = Mayor(datos);	
	 	exit(mayor);
	 }
	 else if(np == 1){
	 	menor = Menor(datos);
	 	exit(menor);
	 }
	 else if(np == 2){
	 	promedio = Promedio(datos);
	 	exit(promedio);
	 }
	 else if(np ==3){
	 	orden = Orden(datos);
	 	printf("datos ordenados: ");
	 	imprimir(datos);

	 }
	 exit(np);
}
void procesoPadre(){
	int numero;
	pid_t pid_Hijo;
	printf("proceso padre ejecutado: %d\n", getpid());
	for(int np = 0; np<NUM_PROC; np++){
		pid_Hijo = wait(&numero);
		printf("este es el pid terminado con el pid: %d\n y retorno %d\n", pid_Hijo, numero>>8);
	}
}
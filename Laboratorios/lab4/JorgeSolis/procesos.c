#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "defs.h"

int enviar[N];
int recibir[N];

void procesoHijo(int np, int *datos,int pipefd[]){
	int mayor, menor,*orden,promedio; 
	 if(np == 0){
	 	mayor = Mayor(datos);
	 	close(pipefd[0]);
	 	write(pipefd[1],&mayor,sizeof(int));	
	 	close(pipefd[1]);
	 	printf("proseso hijo %d, ejecutando con el PID: %d \n",np, getpid());
	 	exit(np);
	 }
	 else if(np == 1){
	 	menor = Menor(datos);
	 	close(pipefd[0]);
	 	write(pipefd[1],&menor,sizeof(int));	
	 	close(pipefd[1]);
	 	exit(np);
	 }
	 else if(np == 2){
	 	promedio = Promedio(datos);
	 	close(pipefd[0]);
	 	write(pipefd[1],&Promedio,sizeof(int));	
	 	close(pipefd[1]);
	 }
	 else if(np ==3){
	 	 register int i;
	 	 close(pipefd[0]);
	 	 orden(*datos);
	 	 for (i=0;i<N;i++){
	 	 	enviar[i] = datos[i];
	 	 	write(pipefd[1],&enviar[i],sizeof(int));	
 		 }
	 	close(pipefd[1]);

	 }
}
void procesoPadre(int pipefd[] ){
	int npc;
	int mayor, menor,promedio;
	pid_t pid_Hijo;
	printf("proceso padre ejecutado: %d\n", getpid());
	close(pipefd[1]);
	for(int np = 0; np<NUM_PROC; np++){
		pid_Hijo = wait(&npc);
		npc = npc>>8;
		if(npc ==0){
			read(pipefd[0],&mayor,sizeof(int));
			printf("el proceso %d este es el pid %d terminado con el mayor: %d\n",npc, pid_Hijo,mayor);
		}
		else if(npc ==1){
			read(pipefd[0],&menor,sizeof(int));
			printf("el proceso %d este es el pid %d terminado con el menor: %d\n",npc, pid_Hijo,menor);
		}
		else if(npc ==2){
			read(pipefd[0],&promedio,sizeof(int));
			printf("el proceso %d este es el pid %d terminado con el promedio: %d\n",npc, pid_Hijo,promedio);
		}
		else if(npc ==3){
			printf("Arreglo ordenado");
			for(int i=0; i<N;i++){
				read(pipefd[0],&recibir[i],sizeof(int));
				printf("%d",&recibir[i]);
				if(i<N-1){
					printf(",");
				}
			}

		}
	}
}
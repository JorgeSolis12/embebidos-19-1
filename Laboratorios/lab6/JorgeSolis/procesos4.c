#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_PROC 4
#define N 32

int * memoria( );
void llenarArreglo( int *datos );
void procesoHijo( int np, int pipefd[] );
void procesoPadre( int pipefd[NUM_PROC][2] );
void imprimir( int *datos );
	

int *A;
int promedio = 0;
int suma_parcial = 0;
int main(){

	pid_t pid;
	int  np;
    int pipefd[NUM_PROC][2], pipe_status;
	A=memoria();
	llenarArreglo(A);
	imprimir(A);

	printf("Probando procesos ...");

	for( np = 0; np < NUM_PROC; np++ )
	{
		pipe_status = pipe( &pipefd[np][0] );
		if( pipe_status == -1 )
		{
			perror("Error al crear la tuberia...\n");
			exit( EXIT_FAILURE );
		}
		pid = fork();
		if( pid == -1 )
		{
			perror("No se creo el proceso...\n");
			exit( EXIT_FAILURE );
		}
		if( !pid )
		{
			procesoHijo( np, &pipefd[np][0] );
		}
	}
	procesoPadre( pipefd );
	free(A);
	return 0;
}
void procesoPadre( int pipefd[NUM_PROC][2] )
{
	int np, npc;
	pid_t pid_hijo;
	printf("Proceso padre ejecutado con pid %d\n", getpid());

	for( np = 0; np < NUM_PROC; np++ )		
	{
		pid_hijo = wait( &npc );
		npc = npc >> 8;
		close( pipefd[npc][1] );
		read( pipefd[npc][0], &suma_parcial, sizeof(int));
		promedio +=suma_parcial;
		printf("proceso hijo %d, terminado con el pid %d\n", npc,pid_hijo);
	}
	printf("promedio en procesos : %d\n",promedio>>5);
	close( pipefd[np][0] );
}

void procesoHijo( int np, int pipefd[] )
{
	register int i = 0;
	int tambloque = N/NUM_PROC;
	int inicio = np*tambloque;
	int final = inicio + tambloque;
	close(pipefd[0]);
	for(i=inicio;i<final;i++){
		
		promedio += A[i];
	}
	write(pipefd[1], &promedio, sizeof(int));
	close(pipefd[1]);
	exit(np);
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

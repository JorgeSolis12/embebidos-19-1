#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#define NUM_PROC 4
#define N 32

int * reservarMemoria( );
void llenarArreglo( int *datos );
void procesoHijo( int np, int pipefd[] );
void procesoPadre( int pipefd[NUM_PROC][2] );
void imprimirArreglo( int *datos );

int *A, *B, *C;
int main()
{
	pid_t pid;
	int  np;
        int pipefd[NUM_PROC][2], pipe_status;

        A = reservarMemoria();
        llenarArreglo( A );
        imprimirArreglo( A);
         B = reservarMemoria();
        llenarArreglo( B );
        imprimirArreglo( B);
         C = reservarMemoria();

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
	imprimirArreglo(C);
	free(A);
	free(B);
	free(C);
	return 0;
}

void procesoPadre( int pipefd[NUM_PROC][2] )
{
	int np, npc, tambloque;
	pid_t pid_hijo;
	tambloque = N/NUM_PROC;
	printf("Proceso padre ejecutado con pid %d\n", getpid());

	for( np = 0; np < NUM_PROC; np++ )		
	{
		pid_hijo = wait( &npc );
		npc = npc >> 8;
		close( pipefd[npc][1] );
		int inibloque = tambloque*npc;
		read( pipefd[npc][0], C+inibloque, sizeof(int)*tambloque);
		printf("proceso hijo %d, terminado con el pid %d\n", npc,pid_hijo);
	}
	close( pipefd[np][0] );
}

void procesoHijo( int np, int pipefd[] )
{
	int tambloque = N/NUM_PROC;
	int inicio = np*tambloque;
	int fin = inicio + tambloque;
	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid());
	close(pipefd[0]);
	register int i;
	for(i = inicio; i<fin; i++){
		C[i] = A[i] * B[i];
	}
	write(pipefd[1], C+inicio, sizeof(int)*tambloque);
	close(pipefd[1]);
	exit(np);
}

void llenarArreglo( int *datos )
{
        register int i;

        //srand( getpid() );
        for( i = 0; i < N; i++ )
        {
                datos[i] = rand() % 255;
        }
}

int * reservarMemoria( )
{
        int *mem;

        mem = (int *)malloc( sizeof(int) * N  );
        if( !mem )
        {
                perror("Error al asignar memoria...\n");
                exit(EXIT_FAILURE);
        }
        return mem;
}

void imprimirArreglo( int *datos )
{
        register int i;

        for( i = 0; i < N; i++ )
        {
                if( !(i%16) )
                        printf("\n");
                printf("%3d ", datos[i]);
        }
        printf("\n");
}


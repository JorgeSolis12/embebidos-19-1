#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

#define NUM_PROC 4
#define MUESTRAS 256
#define PI 3.141592


float * reservarMemoria( );
void generar_seno(float seno[]);
void guardar_datos_sin(float datos[]);
void procesoHijo( int np, int pipefd[] );
void procesoPadre( int pipefd[NUM_PROC][2] );
void imprimirArreglo( float datos[]);
void generar_hamming( float *datos );
void guardar_datos_hamming(float datos[]);

float *C,*seno,*hamming;

int main(){
	pid_t pid;
	int  np;
        int pipefd[NUM_PROC][2], pipe_status;

        seno = reservarMemoria();
        generar_seno (seno);
        imprimirArreglo(seno);
		guardar_datos_sin(seno);
		printf("datos Seno: \n");
		imprimirArreglo(seno);

		hamming = reservarMemoria();
        generar_hamming(hamming);
        guardar_datos_hamming(hamming);
        printf("datos hamming: \n");
        imprimirArreglo(hamming);

         C = reservarMemoria();

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
	printf("datos procesados: \n");
	imprimirArreglo(C);
	free(seno);
	free(hamming);
	free(C);
	return 0;
}

void procesoPadre( int pipefd[NUM_PROC][2] )
{
	int np, npc, tambloque;
	pid_t pid_hijo;
	tambloque = MUESTRAS/NUM_PROC;
	printf("Proceso padre ejecutado con pid %d\n", getpid());

	for( np = 0; np < NUM_PROC; np++ ){
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
	int tambloque = MUESTRAS/NUM_PROC;
	int inicio = np*tambloque;
	int fin = inicio + tambloque;
	printf("Proceso hijo %d ejecutado con pid %d\n", np, getpid());
	close(pipefd[0]);
	register int i;
	for(i = inicio; i<fin; i++){
		C[i] = seno[i] * hamming[i];
	}
	write(pipefd[1], C+inicio, sizeof(int)*tambloque);
	close(pipefd[1]);
	exit(np);
}

void generar_hamming( float *datos){
        float a = 0.54;
        float b =0.46;
        int k, n;
        for( k= 0;k < MUESTRAS;k++){ 
        	n = k+1;
        	if(fabs(k)<=n){
	        	if(k==0){
	        		datos[0]=0.0;
	        	}
	        	else{
	        		float c = (PI*k)/n;
	        		double co = 2*b*cos(c);
	        		datos[k] = fabs(co+a);
	        	}
	        }
	        else{
	        	datos[k]=0.0;
	        }
        }
}

float * reservarMemoria( ){
        float *mem;

        mem = (float*)malloc(sizeof(float)*MUESTRAS);
        if( !mem )
        {
                perror("Error al asignar memoria...\n");
                exit(EXIT_FAILURE);
        }
        return mem;
}

void imprimirArreglo( float datos[]){
        register int i;

        for( i = 0; i < MUESTRAS; i++ )
        {
                if( !(i%16) )
                        printf("\n");
                printf("%3f ", datos[i]);
        }
        printf("\n");
}

void guardar_datos_hamming(float datos[]){
	FILE *ap_arch;
	
	ap_arch = fopen("hamming.dat", "w");
	if(!ap_arch){
		perror("error al abrir el archivo");
	}
	fclose(ap_arch);
}

void guardar_datos_sin(float datos[]){
	FILE *ap_arch;
	
	ap_arch = fopen("seno.dat", "w");
	if(!ap_arch){
		perror("error al abrir el archivo");
	}
	fclose(ap_arch);
}

void generar_seno(float seno[]){
	float f = 1000, fs =45000;
	register int n;
	
	for (n = 0; n < MUESTRAS; n++){
		seno[n] = sinf(2*n*M_PI*f/fs);
	}
}
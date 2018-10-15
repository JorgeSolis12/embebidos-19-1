#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 32
#define NUM_HILOS 4

void * Mayor(void * arg);
void * Menor(void * arg);
void * Promedio(void * arg);
void mergeSort(int arr[], int l, int r);
void * Orden(void * arg);
int * memoria();
void llenarArreglo(int * datos);
void imprimir(int *datos);
void merge(int arr[], int l, int m, int r);
pthread_mutex_t bloqueo;

int main(){
	pthread_t tids[NUM_HILOS];
	int *datos;
	int nhs[NUM_HILOS],*res;

	datos = memoria();
	llenarArreglo(datos);
	imprimir(datos);
	pthread_mutex_init(&bloqueo,NULL);

	printf("creando hilos \n");
	register int nh;
	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		if(nhs[0]==0){
			pthread_create(&tids[nh], NULL,Promedio,(void *)&nhs[nh]);
		}
		if(nhs[1]==1){
			pthread_create(&tids[nh], NULL,Mayor,(void *)&nhs[nh]);
		}
		if(nhs[2]==2){
			pthread_create(&tids[nh], NULL,Menor,(void *)&nhs[nh]);
		}
		if(nhs[3]==3){
			pthread_create(&tids[nh], NULL,Orden,(void *)&nhs[nh]);
			
		}
	}
	for(nh =0; nh<NUM_HILOS; nh++){
		if(nh==0){
			pthread_join(tids[nh], (void **)&res);
			printf("El promedio es: %d\n",*res);
		}
		if(nh==1){
			pthread_join(tids[nh], (void **)&res);
			printf("El mayor es: %d\n",*res);
		}
		if(nh==2){
			pthread_join(tids[nh], (void **)&res);
			printf("El menor es: %d\n",*res);
		}
		if(nh==3){
			pthread_join(tids[nh], NULL);
			printf("datos ordenados :");
			imprimir(res);
		}
	}
	pthread_mutex_destroy(&bloqueo);

	return 0;
}
void * Promedio(void *arg){
	register int i;
	int averange = 0;
	int *datos =(int *)arg;
	for(i= 0; i< N; i++){
		averange += datos[i];
	}
	averange = averange/N;
	pthread_mutex_lock(&bloqueo);
	pthread_exit(&averange);
}
void merge(int arr[], int l, int m, int r) {
	int i, j, k; 
	int n1 = m - l + 1; 
	int n2 =  r - m; 
	int L[n1], R[n2]; 
 
	for (i = 0; i < n1; i++){ 
		L[i] = arr[l + i]; 
	}
	for (j = 0; j < n2; j++){ 
		R[j] = arr[m + 1+ j]; 
  	}
	i = 0,j = 0,k = l;  
	while (i < n1 && j < n2){ 
		if (L[i] <= R[j]){ 
			arr[k] = L[i]; 
			i++; 
		} 
		else{ 
			arr[k] = R[j]; 
			j++; 
		} 
		k++; 
	} 
	while (i < n1) { 
		arr[k] = L[i]; 
		i++; 
		k++; 
	} 
	while (j < n2) { 
		arr[k] = R[j]; 
		j++; 
		k++; 
	} 
} 
void * Orden(void * arg){
	int *datos = (int *)arg;
	mergeSort(datos,0,N-1);
}
void mergeSort(int arr[], int l, int r){ 
	if (l < r) {  
		int m = l+(r-l)/2;  
		mergeSort(arr, l, m); 
		mergeSort(arr, m+1, r); 
		merge(arr, l, m, r); 
	} 
} 
void * Mayor(void * arg){
	register int i;
	static int mayor;
	int *datos =(int *)arg;
	mayor = datos[0];
	for(i =1; i<N;i++){
		if(datos[i]>mayor){
			mayor = datos[i];
		}
	}
	pthread_mutex_lock(&bloqueo);
	pthread_exit(&mayor);
}
void * Menor(void * arg){
	register int i;
	static int menor;
	int *datos =(int *)arg;
	menor = datos[0];
	for(i =1; i<N;i++){
		if(datos[i]<menor){
			menor = datos[i];
		}
	}
	pthread_mutex_lock(&bloqueo);
	pthread_exit(&menor);
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
void llenarArreglo(int * datos){
	register int i;
	srand(getpid());
	for(i=0;i<N;i++){
		datos[i] = rand()%255;
	}
}
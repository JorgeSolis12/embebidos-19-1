#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define N 32
#define NUM_PROC 4

void * suma(void* arg);
void * resta(void* arg);
void * multiplicacion(void* arg);
void * division(void* arg);
int num1 = 52, num2 =34;
int main(){
	pthread_t tid_sum, tid_res, tid_div, tid_mult;
	int *res_sum, *res_resta, *res_mult,*res_div;
	printf("creando hilos \n");
	pthread_create(&tid_sum, NULL,suma,NULL);
	pthread_create(&tid_res, NULL,resta,NULL);
	pthread_create(&tid_mult, NULL,multiplicacion,NULL);
	pthread_create(&tid_div, NULL,division,NULL);

	pthread_join(tid_sum, (void **)&res_sum);
	pthread_join(tid_res, (void **)&res_resta);
	pthread_join(tid_mult, (void **)&res_mult);
	pthread_join(tid_div, (void **)&res_div);

	printf("la suma es: %d\n",*res_sum);
	printf("la resta es: %d\n",*res_resta);
	printf("la multiplicacion es: %d\n",*res_mult);
	printf("la division es: %d\n",*res_div);

	return 0;
}

void * suma(void* arg){
	static int sum;
	sum = num1+num2;
	pthread_exit((void*)&sum);

}
void * resta(void* arg){
	static int res;
	res = num1-num2;
	pthread_exit((void*)&res);		
				
}
void * multiplicacion(void* arg){
	static int mult;
	mult = num1*num2;
	pthread_exit((void*)&mult);
}
void * division(void* arg){
	static int div;
	div = num1/num2;
	pthread_exit((void*)&div);
}
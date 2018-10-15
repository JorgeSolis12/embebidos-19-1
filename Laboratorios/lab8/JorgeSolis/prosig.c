#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void ISRsw(int sig);

int main(){

	if(signal(SIGINT,ISRsw) == SIG_ERR){
		perror("Error al crear la signal");
		exit(EXIT_FAILURE); 
	}
	if(signal(SIGTERM,ISRsw) == SIG_ERR){
		perror("Error al crear la signal");
		exit(EXIT_FAILURE); 
	}
	while(1)
		pause();
	return 0;
}

void ISRsw(int sig){
	if(sig == SIGINT){
		printf("Saca las panochas\n");
	}
	else if(sig == SIGTERM){
		printf("Tengo Hambre prrroooo!!!\n");
	}	

}
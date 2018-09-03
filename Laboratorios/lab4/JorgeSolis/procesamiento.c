#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "defs.h"

int Promedio(int *datos){
	register int i, averange = 0;
	for(i= 0; i< N; i++){
		averange += datos[i];
		averange = averange/N;
	}
	return averange;
}
int Orden(int * datos){
	int aux;
	for(int i=0;i<N;i++){
        for(int x=i+1;x<N-1;x++){
	        if(datos[i]>datos[x]){
	            aux=datos[i];
	            datos[i]=datos[x];
	            datos[x]=aux;
	        }
    	}	
	}
	return &datos;
}
int Mayor(int * datos){
	register int i, mayor;
	mayor = datos[0];
	for(i =1; i<N;i++){
		if(datos[i]>mayor){
			mayor = datos[i];
		}
	}
	return mayor;
}
int Menor(int * datos){
	register int i, menor;
	menor = datos[0];
	for(i =1; i<N;i++){
		if(datos[i]<menor){
			menor = datos[i];
		}
	}
	return menor;
}
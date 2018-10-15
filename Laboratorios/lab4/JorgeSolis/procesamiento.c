#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "defs.h"
#include "datos.h"

int Promedio(int *datos){
	int i;
    float suma = 0.0;

    for (i = 0; i < N; ++i){
        suma += datos[i];
    }
    return suma / (float) N;
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
void Orden(int *datos){
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
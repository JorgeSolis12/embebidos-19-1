#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "imagen.h"
#include <stdint.h>
#define DIMASK 3 //especifica las dimensiones de la mascara, aqui especificamos una de 3x3
#define SIGMA 2
#define NUM_HILOS 4
#define FACTOR1 273
#define FACTOR2 115

int * memoria();
void llenarArreglo(int * datos);
void imprimir(int *datos);	
void producto(void );
unsigned char *RGBtoGray( unsigned char *imagenRGB, uint32_t width, uint32_t height);
void GraytoRGB( unsigned char *imagenGray, unsigned char *imagenRGB, uint32_t width, uint32_t height);
void brilloImagen (unsigned char *imagenGray, uint32_t width, uint32_t height);
unsigned char *reservarMemoria(uint32_t width, uint32_t height);
void *filtroImagen(void *arg);

unsigned char *imagenRGB, *imagenGray, *imagenFiltro;
bmpInfoHeader info;
pthread_mutex_t bloqueo;

int main(){
	register int nh;
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS] , *res;
	

	printf("Abriendo la imagen !!! \n");
	imagenRGB = abrirBMP("huella1.bmp", &info);
	displayInfo( &info );

	imagenGray = RGBtoGray( imagenRGB,  info.width, info.height);
	imagenFiltro = reservarMemoria (info.width, info.height);


	printf("creando hilos \n");
	for(nh =0; nh<NUM_HILOS; nh++){
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL,filtroImagen,(void*)&nhs[nh]);
	}
	for(nh =0; nh<NUM_HILOS; nh++){

		pthread_join(tids[nh], (void **)&res);
	}

	pthread_mutex_destroy(&bloqueo);
	GraytoRGB(imagenFiltro, imagenRGB, info.width, info.height) ;
	
	guardarBMP( "Gaussiano_paralelo.bmp", &info, imagenRGB);//filtro pasabajas
	free(imagenRGB);
	free(imagenGray);
	//free(imagenFiltro);
	return 0;
}

void *filtroImagen(void *arg){

	int nh = *(int*)arg;
	int tambloque=info.width/NUM_HILOS;
	int inicio = nh*tambloque;
	int final= inicio+tambloque;
	register int x,y,xm,ym;
	int conv, indice,indicem;
	unsigned char mascara1[] =
							{1, 4, 7, 4, 1,
							 4,16,26,16, 4,
	    					 7,26,41,26, 7,
	    					 4,16,26,16, 4,
	    					 1, 4, 7, 4, 1};
	
	/*unsigned char mascara2[] =
							{2, 4, 5, 4, 2,
							 4,9,12 , 9, 4,
	    					 5,12,15,12, 5,
	    					 4, 9,12, 9, 4,
	    					 2, 4, 5, 4, 2};
	*/
	for( y = 0; y <= (info.height-DIMASK); y++)//Con esta pareja de ciclos se recorre la imagen
		for ( x = inicio; x <= final; x++) //por completo
		{
				for(ym=y;ym< y+DIMASK;ym++)//for (ym = 0; ym < DIMASK; ym++ )//con esta pareja podemos recorrer
				{	
					for(xm=x;xm< x+DIMASK;xm++)//for (xm = 0; xm < DIMASK; xm++ )//la mascara
					{
						indice = ym*info.width+xm;
						indicem = (ym-y)*DIMASK+(xm-x);
						conv += imagenGray[indice]*mascara1[indicem];
					}
				}
					conv /= FACTOR1;
					indice = ((y+1)*info.width + (x+1));
					
					pthread_mutex_lock(&bloqueo);
					imagenFiltro[indice]= conv;
					pthread_mutex_unlock(&bloqueo);
		}
	pthread_exit(arg);
}

unsigned char *reservarMemoria(uint32_t width, uint32_t height) {
	unsigned char *imagen;
	imagen = (unsigned char*) malloc(width*height*sizeof(unsigned char));
	if(imagen == NULL)
	{
		perror("Error al asignar memoria \n");
		exit(EXIT_FAILURE);
	}
	return imagen;
}



unsigned char *RGBtoGray( unsigned char *imagenRGB, uint32_t width, uint32_t height)
{
	register int y,x ;
	int indiceRGB, indiceGray;
	unsigned char grayLevel;
	unsigned char *imagenGray;
	imagenGray = reservarMemoria(width,height);
	if(imagenGray == NULL)
	{
		perror("Error al asignar memoria \n");
		exit(EXIT_FAILURE);
	}

	for ( y = 0; y < height ; y++ )
		for ( x = 0; x < width; x++ )
		{
			indiceGray = (width * y + x);
			//indiceRGB = indiceGray * 3 ;//multiplicar por 3 con corrimientos
			indiceRGB = ((indiceGray << 1) + indiceGray);
			grayLevel = (30*imagenRGB[indiceRGB]+59*imagenRGB[indiceRGB+1]+11*imagenRGB[indiceRGB+2])/100;
			imagenGray[indiceGray] = grayLevel;
		}
	return imagenGray;
}

void brilloImagen (unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	register int p;
	unsigned short int pixel;
	for (p = 0 ; p < width*height ; p++ )
	{
		pixel = imagenGray[p] + 80;
		imagenGray[p] = (pixel>255)? 255 : (unsigned char)pixel;//si el valor de pixel es mayor a 255, devuelve 255 sino se castea el valor de pixel a unsigned char y se guarda en imagenGray.
	}
}

void GraytoRGB( unsigned char *imagenGray, unsigned char *imagenRGB, uint32_t width, uint32_t height){
	register int y,x ;
	int indiceRGB, indiceGray;
	//imagenGray = (unsigned char*) malloc(width*height*sizeof(unsigned char));
        for ( y = 0; y < height ; y++ )
                for ( x = 0; x < width; x++ )
                {
			indiceGray = (y*width+x);
			indiceRGB = indiceGray * 3;

			imagenRGB[indiceRGB+0] = imagenGray [indiceGray];
			imagenRGB[indiceRGB+1] = imagenGray [indiceGray];
			imagenRGB[indiceRGB+2] = imagenGray [indiceGray];

		}
}


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <pthread.h>

void* computePath(void* args);

int terrain[512][128];
int compteur = 0;
int nbThread = 15;
	

int main() {
	
	pthread_t tabT[nbThread];
	int idT[nbThread];
	long posT[nbThread];
	
	for(int i = 0 ; i < 15 ; i++){
		
		long  x = rand() % (512 * 128);
		while( terrain[x % 512][x / 512]) x = rand() % (512 * 128);
		posT[i] = x;
		
		idT[i] = pthread_create(&tabT[i], NULL, computePath,(void*)posT[i]);
	}
    

    while(compteur>0){
    }
    return 0;

}


void* computePath(void* args) {
	long x = ((long)args) % 512 ;
	long y = ((long)args) / 512 ;

}


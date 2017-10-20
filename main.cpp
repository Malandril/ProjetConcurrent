#include <iostream>
#include "Cell.h"

int terrain[512][128];
int compteur=0;
int main() {
    std::cout << "Hello, World!" << std::endl;

    //generation de threads

    while(compteur>0){
    }
    return 0;

}

void changeTerrain(int x, int y, int xDest, int yDest) {
    terrain[x][y] = 0;
    terrain[xDest][yDest] = 1;
}

void computePath() {
    int x = 200;
    int y = 100;
    while (true) {
        //lock
        //calcul de
        //unlock
    }
}


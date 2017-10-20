#include <iostream>
#include <pthread.h>
#include <math.h>
#include "Cell.h"
#include "Display.h"

static const int MAX_X = 512;
static const int MAX_Y = 128;
static const int DEST_X = -1;
static const int DEST_Y = -1;

int terrain[MAX_X][MAX_Y];

int nbThread = 512;
int counter = nbThread;

pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t testC = PTHREAD_MUTEX_INITIALIZER;

bool isValidCell(int x, int y);

double distance(int x, int y, int xDest, int yDest);

void *computePath(void *pVoid);

void bestCell(int &x, int &y);


void printTerrain() {
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            std::cout << terrain[j][i];
        }
        std::cout << "\n";
    }
    std::cout << std::endl;

}

int main() {
    clock_t start = clock();

    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            terrain[j][i] = 0;
        }
    }
    pthread_t tabT[nbThread];
    int idT[nbThread];
    long posT[nbThread];
    srand(2501);
    for (int i = 0; i < nbThread; i++) {

        long x = rand() % (MAX_X * MAX_Y);
        while (terrain[x % MAX_X][x / MAX_X] || x == 0) x = rand() % (MAX_X * MAX_Y);
        posT[i] = x;

        idT[i] = pthread_create(&tabT[i], nullptr, computePath, (void *) posT[i]);
    }
    display(MAX_X, MAX_Y, counter, (int *) (terrain));


    time_t end;
    time(&end);
    std::cout << "AAAAAAA " << (clock() - start) * 1000.0 / CLOCKS_PER_SEC << std::endl;
    return 0;

}


void *computePath(void *args) {
    int x = ((long) args) % MAX_X;
    int x1 = x;
    int y = ((long) args) / MAX_X;
    int y1 = y1;

    int count = 0;
    while (x > 0 || y > 0) {
        pthread_mutex_lock(&arrayMutex); //locking array
        bestCell(x, y);
        pthread_mutex_unlock(&arrayMutex); //unlocking array
        count++;
        displayRefresh();
    }
    pthread_mutex_lock(&arrayMutex);
    terrain[x][y] = 0;
    pthread_mutex_unlock(&arrayMutex);
    pthread_mutex_lock(&testC);
    counter--;
    pthread_mutex_unlock(&testC);
}

void bestCell(int &x, int &y) {
    double minDistance = distance(0, 0, MAX_X, MAX_Y);
    int tmpx = -1;
    int tmpy = -1;
    for (int i = y - 1; i <= y + 1; ++i) {
        for (int j = x - 1; j < x + 1; ++j) {
            if (distance(j, i, DEST_X, DEST_Y) < minDistance && isValidCell(j, i)) {
                minDistance = distance(j, i, DEST_X, DEST_Y);
                tmpx = j;
                tmpy = i;
            }
        }
    }
    if (tmpx >= 0 && tmpy >= 0) {
        terrain[x][y] = 0;
        terrain[tmpx][tmpy] = 1;
        x = tmpx;
        y = tmpy;
    }
}

bool isValidCell(int x, int y) {
    return (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) && (terrain[x][y] == 0);
}

double distance(int x, int y, int xDest, int yDest) {
    return sqrt(pow(xDest - x, 2) + pow(yDest - y, 2));
}
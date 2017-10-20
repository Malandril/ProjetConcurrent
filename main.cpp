#include <iostream>
#include <pthread.h>
#include <math.h>

#include "Cell.h"

static const int MAX_X = 30;
static const int MAX_Y = 12;
int terrain[MAX_X][MAX_Y];

static const int DEST_X = -1;

static const int DEST_Y = -1;

pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;

bool isValidCell(int x, int y);

double distance(int x, int y, int xDest, int yDest);

void computePath();

void bestCell(int &x, int &y);

int main() {
    std::cout << "Hello, World!" << std::endl;

    //generation de threads
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            terrain[j][i] = 0;
        }
    }
    computePath();
    return 0;

}

void printTerrain() {
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            std::cout << terrain[j][i];
        }
        std::cout << "\n";
    }
    std::cout << std::endl;

}

void computePath() {
    int x = 20;
    int y = 10;

    while (x > 0 || y > 0) {
        pthread_mutex_lock(&arrayMutex); //locking array
        bestCell(x, y);
        pthread_mutex_unlock(&arrayMutex); //unlocking array
        printTerrain();

    }
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
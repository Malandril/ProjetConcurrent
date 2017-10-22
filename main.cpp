#include <iostream>
#include <pthread.h>
#include <cmath>
#include <unistd.h>
#include <chrono>

#include "Cell.h"

using std::cout;
using std::endl;
using std::chrono::high_resolution_clock;
using namespace std::chrono;
static const int MAX_X = 60;
static const int MAX_Y = 60;
static const int DEST_X = -1;
static const int DEST_Y = -1;
int terrain[MAX_X][MAX_Y];
int nbThread = 1;
pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t testMut = PTHREAD_MUTEX_INITIALIZER;

bool isValidCell(int x, int y);

double distance(int x, int y, int xDest, int yDest);

void *computePath(void *pVoid);

void bestCell(int &x, int &y);

void initPersonPos(int *idT, long *posT);

void printTerrain() {
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            std::cout << terrain[j][i];
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    bool metric = false;
    int opt;
    while ((opt = getopt(argc, argv, "p::t::m:")) != -1) { //parses arguments and selects relevent ones
        switch (opt) {
            case 'p':
                if (optarg != nullptr) {
                    nbThread = static_cast<int>(pow(2, atoi(optarg)));
                }
                break;
            case 't':
                if (optarg != nullptr) {
                    if (optarg[0] == '1') {
                        std::cerr << "Not implemented yet" << endl;
                        exit(-1);
                    }
                }
                break;
            case 'm':
                metric = true;
                break;
            case '?':
                cout << optopt << " unknown option " << endl;
                break;
            default:
                break;

        }
    }

    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            terrain[j][i] = 0;
        }
    }

    pthread_t tabT[nbThread];
    int idT[nbThread];
    long posT[nbThread];
    srand(2501); //allows to always have the same position of persons
    pthread_mutex_lock(&testMut);
    initPersonPos(idT, posT);
    high_resolution_clock::time_point start;
    if(metric) {
        start = std::chrono::high_resolution_clock::now();
    }
    for (int j = 0; j < nbThread; ++j) {
        idT[j] = pthread_create(&tabT[j], nullptr, computePath, (void *) posT[j]);
    }
    timespec t = {1};
    if(!metric) {
        printTerrain();
        pthread_mutex_unlock(&testMut);
    }
//    while (true) {
//        printTerrain();
//        nanosleep(&t, nullptr);
//    }
    for (int k = 0; k < nbThread; ++k) {
        pthread_join(tabT[k], nullptr);
    }
    if(metric) {
        auto end = high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<milliseconds>(end - start);
        std::cout << "temps d'execution " << elapsed.count() << " ms" << std::endl;
    }
    return 0;

}

void initPersonPos(int *idT, long *posT) {
    for (int i = 0; i < nbThread; i++) {

        long x = rand() % (MAX_X * MAX_Y);
        while (terrain[x % MAX_X][x / MAX_X] || x == 0) x = rand() % (MAX_X * MAX_Y);
        terrain[x % MAX_X][x / MAX_X] = 1;
        posT[i] = x;
    }
}


void *computePath(void *args) {
    int x = ((long) args) % MAX_X;
    int y = ((long) args) / MAX_X;
    pthread_mutex_lock(&testMut);
    pthread_mutex_unlock(&testMut);
    while (x > 0 || y > 0) {
        pthread_mutex_lock(&arrayMutex); //locking array
        bestCell(x, y);
        pthread_mutex_unlock(&arrayMutex); //unlocking array
    }
    pthread_mutex_lock(&arrayMutex);
    terrain[x][y] = 0;
    pthread_mutex_unlock(&arrayMutex);
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
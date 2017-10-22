#include <iostream>
#include <pthread.h>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <sys/resource.h>
#include "Cell.h"
#include "Display.h"

using std::cout;
using std::endl;
using std::chrono::high_resolution_clock;
using namespace std::chrono;

static const int MAX_X = 512;
static const int MAX_Y = 128;
static const int DEST_X = -1;
static const int DEST_Y = -1;
static const int REPEAT = 5;
static const int SEED = 2501;
int terrain[MAX_X][MAX_Y];
int nbThread = 1;
int counter = nbThread;

pthread_mutex_t arrayMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t testC = PTHREAD_MUTEX_INITIALIZER;

bool isValidCell(int x, int y);

double distance(int x, int y, int xDest, int yDest);

void *computePath(void *args);

void bestCell(int &x, int &y);

void initPersonPos(long *posT);

void createAndWaitThreads(pthread_t *tabT, int *idT, const long *posT);

int maxTime(const long timeArray[], int k);

int minTime(const long timeArray[],int k);

double averageTime(const long timeArray[], int k);

int main(int argc, char *argv[]) {
    bool metric = false;
    int opt;
    while ((opt = getopt(argc, argv, "p::t::m")) != -1) { //parses arguments and selects relevent ones
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
    high_resolution_clock::time_point start;
    srand(SEED); //allows the persons to always have the same position
    initPersonPos(posT);
    if (metric) {
        long responseTime[REPEAT];
        long userTime[REPEAT];
        long systemTime[REPEAT];
        for (int i = 0; i < REPEAT; i++) {
            start = std::chrono::high_resolution_clock::now();
            createAndWaitThreads(tabT, idT, posT);
            auto end = high_resolution_clock::now();
            rusage usage = {};
            getrusage(RUSAGE_SELF, &usage);
            userTime[i] = usage.ru_utime.tv_usec;
            systemTime[i] = usage.ru_stime.tv_usec;
            responseTime[i] = std::chrono::duration_cast<milliseconds>(end - start).count();
        }
        double response = averageTime(responseTime, REPEAT);
        double user = averageTime(userTime, REPEAT);
        double system = averageTime(systemTime, REPEAT);
        std::cout << "Temps de réponse moyen: " << response << " ms" << std::endl;
        std::cout << "Temps CPU espace utilisateur moyen: " << user/1000.0 << " ms temps système moyen: " << system/1000.0 << " ms" << std::endl;
    } else {
        createAndWaitThreads(tabT, idT, posT);
    }
    return 0;

}

double averageTime(const long timeArray[], int k) {
    int maxId = maxTime(timeArray, k);
    int minId = minTime(timeArray, k);
    long sum = 0;
    for (int j = 0; j < k; ++j) {
        if (j != maxId && j != minId) {
            sum += timeArray[j];
        }
    }
    double average = sum / 3.0;
    return average;
}

int minTime(const long timeArray[], int k) {
    long min = timeArray[0];
    int minId = 0;
    for (int j = 0; j < k; ++j) {
        if (timeArray[j] < min) {
            minId = j;
        }
    }
    return minId;
}

int maxTime(const long timeArray[], int k) {
    long max = 0;
    int maxId = 0;
    for (int j = 0; j < k; ++j) {
        if (timeArray[j] > max) {
            max = timeArray[j];
            maxId = j;
        }
    }
    return maxId;
}

void createAndWaitThreads(pthread_t *tabT, int *idT, const long *posT) {
    for (int j = 0; j < nbThread; ++j) {
        idT[j] = pthread_create(&tabT[j], nullptr, computePath, (void *) posT[j]);
    }
    for (int k = 0; k < nbThread; ++k) {
        pthread_join(tabT[k], nullptr);
    }
}

void initPersonPos(long *posT) {
    for (int i = 0; i < nbThread; i++) {
        long x = rand() % (MAX_X * MAX_Y);
        while (terrain[x % MAX_X][x / MAX_X] || x == 0) x = rand() % (MAX_X * MAX_Y);
        terrain[x % MAX_X][x / MAX_X] = 1;
        posT[i] = x;
    }
    display(MAX_X, MAX_Y, counter, (int *) (terrain));


    time_t end;
    time(&end);
    std::cout << "AAAAAAA " << (clock() - start) * 1000.0 / CLOCKS_PER_SEC << std::endl;
    return 0;

}

void *computePath(void *args) {
    int x = ((long) args) % MAX_X;
    int y = ((long) args) / MAX_X;
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

void printTerrain() {
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            std::cout << terrain[j][i];
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

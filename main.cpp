#include <iostream>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <semaphore.h>
#include "main.h"
#include "Display.h"

using std::cout;
using std::endl;
using std::chrono::high_resolution_clock;

using namespace std::chrono;

Cell terrain[MAX_X][MAX_Y];
unsigned int nbThread = 1;
sem_t counter ;
bool metric = false;


sem_t arraySemaphore;

void setMetrics(long userTime[], long systemTime[], int i);

#if defined(_WIN32)

void setMetrics(long userTime[], long systemTime[], int i) {
    std::cerr << "Metrics not Working in windows"<<endl;
    exit(4);
}

#else

#include <sys/resource.h>

void setMetrics(long userTime[], long systemTime[], int i) {
    rusage usage = {};
    getrusage(RUSAGE_SELF, &usage);
    userTime[i] = usage.ru_utime.tv_usec;
    systemTime[i] = usage.ru_stime.tv_usec;
}

#endif

bool isValidCell(int x, int y);

double distance(int x, int y, int xDest, int yDest);

void *computePath(void *args);

void bestCell(int &x, int &y);

void initPersonPos(long *posT);

void createAndWaitThreads(pthread_t *tabT, int *idT, const long *posT);

int maxTime(const long timeArray[], int k);

int minTime(const long timeArray[], int k);

double averageTime(const long timeArray[], int k);


void spawnObstacle();

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "p::t::m")) != -1) { //parses arguments and selects relevant ones
        switch (opt) {
            case 'p':
                if (optarg != nullptr) {
                    int y = atoi(optarg);
                    if (y < 0 || y > 9) {
                        std::cerr << " Le nombre doit etre entre 0 et 9" << endl;
                        exit(1);
                    }
                    nbThread = static_cast<unsigned int>(pow(2, y));
                }
                break;
            case 't':
                if (optarg != nullptr) {
                    if (optarg[0] == '1') {
                        std::cerr << "Not implemented yet" << endl;
                        exit(2);
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
    sem_init(&arraySemaphore, 0, 1);
    sem_init(&counter, 0, nbThread);
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            terrain[j][i] = Cell(); //initialise les cellules a zero
        }
    }

    pthread_t tabT[nbThread];
    int idT[nbThread];
    long posT[nbThread];

    srand(SEED); //allows the persons to always have the same position
    for (int k = 0; k < 10; ++k) {
        spawnObstacle();
    }

    initPersonPos(posT);
    high_resolution_clock::time_point start;
    if (metric) {
        cout << "Running program " << REPEAT << " times with " << nbThread << " threads" << endl;
        long responseTime[REPEAT];
        long userTime[REPEAT];
        long systemTime[REPEAT];
        for (int i = 0; i < REPEAT; i++) {
            start = std::chrono::high_resolution_clock::now();
            createAndWaitThreads(tabT, idT, posT);
            auto end = high_resolution_clock::now();
            responseTime[i] = std::chrono::duration_cast<milliseconds>(end - start).count();
            setMetrics(userTime, systemTime, i);
        }
        double response = averageTime(responseTime, REPEAT);
        double user = averageTime(userTime, REPEAT);
        double system = averageTime(systemTime, REPEAT);
        std::cout << "Temps de réponse moyen: " << response << " ms" << std::endl;
        std::cout << "Temps CPU espace utilisateur moyen: " << user / 1000.0 << " ms temps système moyen: "
                  << system / 1000.0 << " ms" << std::endl;
    } else {
        start = std::chrono::high_resolution_clock::now();
        cout << "Running program with " << nbThread << " threads" << endl;
        createAndWaitThreads(tabT, idT, posT);
        auto end = high_resolution_clock::now();
        cout << std::chrono::duration_cast<milliseconds>(end - start).count() << " ms" << endl;
    }
    return 0;

}

/**
 *  Essaie de créer un obstacle tant que les nombres aléatoires donnent des obtacles invalides
 */
void spawnObstacle() {
    int ox = 2 + rand() % (MAX_X - 80);
    int oy = 2 + rand() % (MAX_Y - 80);
    int ow = 2 + rand() % (MAX_X - ox);
    int oh = 2 + rand() % (MAX_Y - oy);
    for (int i = ox - 2; i < ox + ow + 2; ++i) {
        for (int j = oy - 2; j < oy + oh + 2; ++j) {
            if (terrain[i][j].readValue() == OBSTACLE) {
                spawnObstacle();
                return;
            }
        }
    }
    for (int i1 = ox; i1 < ox + ow; ++i1) {
        for (int j2 = oy; j2 < oy + oh; ++j2) {
            terrain[i1][j2] = Cell(OBSTACLE);
        }
    }
}

/**
 * Calcule le temps moyen avec le tableau d'entier donné
 * @param timeArray
 * @param k
 * @return
 */
double averageTime(const long timeArray[], int k) {
    int maxId = maxTime(timeArray, k);
    int minId = minTime(timeArray, k);
    long sum = 0;
    for (int j = 0; j < k; ++j) {
        if (j != maxId && j != minId) {
            sum += timeArray[j];
        }
    }
    double average = sum / (k - 2.0);
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

/**
 * Cette fonction va créer tous les threads et va les attendres
 * @param tabT
 * @param idT
 * @param posT
 */
void createAndWaitThreads(pthread_t *tabT, int *idT, const long *posT) {
    for (int j = 0; j < nbThread; ++j) {
        idT[j] = pthread_create(&tabT[j], nullptr, computePath, (void *) posT[j]);
    }
    if (metric || !canDisplay) {
        for (int k = 0; k < nbThread; ++k) {
            pthread_join(tabT[k], nullptr);
        }
    } else {
        display(counter, (Cell *) terrain);
    };
}

/**
 * Genere aleatoirement des personnes a des endroits valides
 * @param posT
 */
void initPersonPos(long *posT) {
    for (int i = 0; i < nbThread; i++) {
        long x = rand() % (MAX_X * MAX_Y);
        while (terrain[x % MAX_X][x / MAX_X].readValue() || x == 0) x = rand() % (MAX_X * MAX_Y);
        terrain[x % MAX_X][x / MAX_X].moveIn();
        posT[i] = x;
    }
}

/**
 * Fonction éxécuté par les threads et qui tourne jusqu'à ce que le thread arrive à destination
 * @param args
 * @return
 */
void *computePath(void *args) {
    int x = ((long) args) % MAX_X;
    int y = ((long) args) / MAX_X;
    while (x != DEST_X || y != DEST_Y) {
        bestCell(x, y);
        if (!metric && canDisplay) {
            waitDisplayRefresh(); //ralentit le deplacement des threads si il y a affichage pour + de lisibilité
        }
    }
    terrain[x][y].moveOut();
    if (!metric && canDisplay) {// ce compteur permet à l'affichage de verifier quand tous les threads sont finis
        sem_wait(&counter);
    }
}

/**
 * Cette fonction permet de trouver La meilleur cellule vers laquelle se deplacer
 * par rapport à la position courante x, y et s'y deplace
 * @param x
 * @param y
 */
void bestCell(int &x, int &y) {
    double minDistance = distance(DEST_X, DEST_Y, x, y);
    int tmpx = x;
    int tmpy = y;
    double dist;
    for (int i = y - 1; i <= y + 2; ++i) {
        for (int j = x - 1; j < x + 2; ++j) {
            dist = distance(j, i, DEST_X, DEST_Y);
            if (isValidCell(j, i) && dist < minDistance) {
                minDistance = dist;
                tmpx = j;
                tmpy = i;
            }
        }
    }
    if (tmpx != x || tmpy != y) {
        terrain[x][y].move(terrain[tmpx][tmpy]);
        x = tmpx;
        y = tmpy;
    }
}

/**
 * Cette fonction vérifie si la cellule est dans le tableau et si elle n'est pas deja prise
 * @param x
 * @param y
 * @return vrai si cellule valide
 */
bool isValidCell(int x, int y) {
    return (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) && (terrain[x][y].readValue() == 0);
}

/**
 * Distance entre deux cellules
 * @param x
 * @param y
 * @param xDest
 * @param yDest
 * @return
 */
double distance(int x, int y, int xDest, int yDest) {
    return sqrt(pow(xDest - x, 2) + pow(yDest - y, 2));
}

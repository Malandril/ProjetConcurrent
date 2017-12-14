#include <array>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <chrono>
#include <semaphore.h>
#include "Cell/Cell.h"
#include "main.h"
#include "Cell/LockableCell.h"
#include "threads/Threading.h"
#include "threads/ThreadPart.h"
#include "Display.h"

using std::cout;
using std::endl;
using std::chrono::high_resolution_clock;
using std::vector;
using std::array;
using namespace std::chrono;

vector<vector<Cell *>> terrain(MAX_X, vector<Cell *>(MAX_Y));
int nbPerson = 1;
bool metric = false;
array<ThreadPart *, 4> threadParts;
sem_t arraySemaphore;

void setMetrics(array<long, REPEAT> &userTime, array<long, REPEAT> &systemTime, int i);

#if defined(_WIN32)

void setMetrics(long userTime[], long systemTime[], int i) {
    std::cerr << "Metrics not Working in windows"<<endl;
    exit(4);
}

#else

#include <sys/resource.h>
#include <iomanip>

void setMetrics(array<long, REPEAT> &userTime, array<long, REPEAT> &systemTime, int i) {
    rusage usage = {};
    getrusage(RUSAGE_SELF, &usage);
    userTime[i] = usage.ru_utime.tv_usec;
    systemTime[i] = usage.ru_stime.tv_usec;
}

#endif

void initPersonPos(vector<Point> &posVector);

int maxTime(array<long, REPEAT> timeArray, int k);

int minTime(array<long, REPEAT> timeArray, int k);

double averageTime(array<long, REPEAT> timeArray, int k);


void spawnObstacle();

void parseArgs(int argc, char *const *argv, bool &parting);

void createAndWaitThreads(vector<pthread_t> &tabT, vector<Point> &posT);

void partitionTerrain(vector<Point> &persons);

void wakeAllThreadParts() {
    for (int i = 0; i < threadParts.size(); ++i) {
        threadParts[i]->wake();
    }
}

int main(int argc, char *argv[]) {
    bool partitioning = false;
    parseArgs(argc, argv, partitioning);
    sem_init(&arraySemaphore, 0, 1);

    if (partitioning) {
        for (int i = 0; i < MAX_Y; ++i) {
            for (int j = 0; j < MAX_X; ++j) {
                terrain[j][i] = new Cell(0, j, i); //initialise les cellules a zero
            }
        }
    } else {
        for (int i = 0; i < MAX_Y; ++i) {
            for (int j = 0; j < MAX_X; ++j) {
                terrain[j][i] = new LockableCell(0); //initialise les cellules a zero
            }
        }
    }

    vector<pthread_t> tabT(nbPerson);
    vector<Point> posArray(nbPerson);

    srand(SEED); //allows the persons to always have the same position
    for (int k = 0; k < 10; ++k) {
        spawnObstacle();
    }

    initPersonPos(posArray);
    high_resolution_clock::time_point start;

    if (metric) {
        cout << "Running program " << REPEAT << " times with " << nbPerson << " persons" << endl;
        array<long, REPEAT> responseTime{};
        array<long, REPEAT> userTime{};
        array<long, REPEAT> systemTime{};
        for (int i = 0; i < REPEAT; i++) {
            start = std::chrono::high_resolution_clock::now();
            std::cout << "starting" << std::endl;
            if (partitioning) {
                partitionTerrain(posArray);
            } else {
                createAndWaitThreads(tabT, posArray);
            }
            auto end = high_resolution_clock::now();
            responseTime[i] = std::chrono::duration_cast<milliseconds>(end - start).count();
            std::cout << responseTime[i] << " ms " << endl;
            setMetrics(userTime, systemTime, i);
        }
        double response = averageTime(responseTime, REPEAT);
        double user = averageTime(userTime, REPEAT);
        double system = averageTime(systemTime, REPEAT);
        std::cout << "Temps de réponse moyen: " << std::setprecision(10) << response << " ms" << std::endl;
        std::cout << "Temps CPU espace utilisateur moyen: " << user / 1000.0 << " ms temps système moyen: "
                  << system / 1000.0 << " ms" << std::endl;
    } else {
        start = std::chrono::high_resolution_clock::now();
        cout << "Running program with " << nbPerson << " persons" << endl;
        if (partitioning) {
            partitionTerrain(posArray);
        } else {
            createAndWaitThreads(tabT, posArray);
        }
        auto end = high_resolution_clock::now();
        cout << std::chrono::duration_cast<milliseconds>(end - start).count() << " ms" << endl;
    }
    for (int l = 0; l < terrain.size(); ++l) {
        for (int i = 0; i < terrain[0].size(); ++i) {
            Cell *cell = terrain[l][i];
            terrain[l][i] = nullptr;
            delete cell;
        }
    }
    return 0;
}


void createAndWaitThreads(vector<pthread_t> &tabT, vector<Point> &posT) {
    vector<PersonThread *> persons;
    for (int j = 0; j < nbPerson; ++j) {
        Point p = posT[j];
        PersonThread *person = new PersonThread(nbPerson, terrain, metric, p);
        persons.push_back(person);
        pthread_create(&tabT[j], nullptr, Threading::threadFunction,
                       person);
    }
    if (!metric && canDisplay) {
        pthread_t dispThread;
        pthread_create(&dispThread, nullptr, display, &terrain);
    }
    for (int k = 0; k < nbPerson; ++k) {
        pthread_join(tabT[k], nullptr);
    }
    for (int i = 0; i < persons.size(); ++i) {
        delete persons[i];
    }
}

void parseArgs(int argc, char *const *argv, bool &parting) {
    int opt;
    while ((opt = getopt(argc, argv, "p::t::m")) != -1) { //parses arguments and selects relevant ones
        switch (opt) {
            case 'p':
                if (optarg != nullptr) {
                    int y = atoi(optarg);
                    if (y < 0 || y > 12) {
                        std::cerr << " Le nombre doit etre entre 0 et 9" << endl;
                        exit(1);
                    }
                    nbPerson = static_cast<unsigned int>(pow(2, y));
                }
                break;
            case 't':
                if (optarg != nullptr) {
                    if (optarg[0] == '1') {
                        std::cerr << "4 threads version" << endl;
                        parting = true;
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
}

/**
 *  Essaie de créer un obstacle tant que les nombres aléatoires donnent des obtacles invalides
 */
void spawnObstacle() {
    int M = 10;
    int ox = M + rand() % (MAX_X - 2 * M);
    int oy = M + rand() % (MAX_Y - 2 * M);
    int ow = M / 2 + rand() % (MAX_X - ox - M) / 2;
    int oh = M / 2 + rand() % (MAX_Y - oy - M);
    for (int i = ox - M / 4; i < ox + ow + M / 4; ++i) {
        for (int j = oy - M / 4; j < oy + oh + M / 4; ++j) {
            if (terrain[i][j]->readValue() == OBSTACLE) {
                spawnObstacle();
                return;
            }
        }
    }
    for (int i1 = ox; i1 < ox + ow; ++i1) {
        for (int j2 = oy; j2 < oy + oh; ++j2) {
            delete terrain[i1][j2];
            terrain[i1][j2] = new LockableCell(OBSTACLE);
        }
    }
}

/**
 * Calcule le temps moyen avec le tableau d'entier donné
 * @param timeArray
 * @param k
 * @return
 */
double averageTime(const array<long, REPEAT> timeArray, int k) {
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

int minTime(const array<long, REPEAT> timeArray, int k) {
    long min = timeArray[0];
    int minId = 0;
    for (int j = 0; j < k; ++j) {
        if (timeArray[j] < min) {
            minId = j;
        }
    }
    return minId;
}

int maxTime(const array<long, REPEAT> timeArray, int k) {
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
 * Genere aleatoirement des personnes a des endroits valides
 * @param posVector
 */
void initPersonPos(vector<Point> &posVector) {
    for (int i = 0; i < nbPerson; i++) {
        int x = rand() % MAX_X;
        int y = rand() % MAX_Y;
        while (terrain[x][y]->readValue() || (x == 0 && y == 0)) {
            x = rand() % MAX_X;
            y = rand() % MAX_Y;
        }
        terrain[x][y]->spawn();
        posVector[i] = {x, y};
    }
    for (int j = 0; j < posVector.size(); ++j) {
        terrain[posVector[j].x][posVector[j].y]->moveOut();
    }
}

/**
 * fonction qui créé les threads des quatres parties et les objets nécéssaires a leur fonctionnement et attend la fin
 * des threads
 * @param persons liste de position des personnes qui vont etre sur le terrain
 */
void partitionTerrain(vector<Point> &persons) {
    pthread_t t0, t1, t2, t3;
    int xpos = static_cast<int>(terrain.size() / 2);
    int ypos = static_cast<int>(terrain[0].size() / 2);
    Point size = Point(xpos - 1, ypos - 1);
    sem_t counter;
    sem_init(&counter, 0, 1);
    ThreadPart *topLeft = new ThreadPart(nbPerson, terrain, metric, Point(0, 0), size);
    ThreadPart *topRight = new ThreadPart(nbPerson, terrain, metric, Point(xpos, 0), size);
    ThreadPart *bottomLeft = new ThreadPart(nbPerson, terrain, metric, Point(0, ypos), size);
    ThreadPart *bottomRight = new ThreadPart(nbPerson, terrain, metric, Point(xpos, ypos), size);
    threadParts = {topLeft, topRight, bottomLeft, bottomRight};
    for (Point p: persons) {
        for (int k = 0; k < threadParts.size(); ++k) {
            if (threadParts[k]->contains(p)) {
                threadParts[k]->spawnPerson(p);
                break;
            }
        }
        terrain[p.x][p.y]->spawn();
    }

    pthread_create(&t0, nullptr, Threading::threadFunction,
                   topLeft);
    pthread_create(&t1, nullptr, Threading::threadFunction,
                   topRight);
    pthread_create(&t2, nullptr, Threading::threadFunction,
                   bottomLeft);
    pthread_create(&t3, nullptr, Threading::threadFunction,
                   bottomRight);

    if (!metric && canDisplay) {
        pthread_t dispThread;
        pthread_create(&dispThread, nullptr, display, &terrain);
    }
    pthread_join(t0, nullptr);
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);
    for (int i = 0; i < threadParts.size(); ++i) {
        delete threadParts[i];
    }
}


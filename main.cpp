#include <array>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <semaphore.h>
#include <vector>
#include "main.h"
#include "Display.h"
#include "Point.h"
#include "threads/Threading.h"
#include "threads/Person.h"
#include "threads/OptimistPerson.h"

using std::cout;
using std::endl;
using std::chrono::high_resolution_clock;
using std::vector;
using std::array;
using namespace std::chrono;

vector<vector<Cell *>> terrain(MAX_X, vector<Cell *>(MAX_Y));
int nbThread = 1;
sem_t counter;
bool metric = false;

sem_t arraySemaphore;

void setMetrics(array<long, REPEAT> &userTime, array<long, REPEAT> &systemTime, int i);

#if defined(_WIN32)

void setMetrics(long userTime[], long systemTime[], int i) {
    std::cerr << "Metrics not Working in windows"<<endl;
    exit(4);
}

#else

#include <sys/resource.h>

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

void parseArgs(int argc, char *const *argv, int opt);

void createAndWaitThreads(vector<pthread_t> &tabT, vector<int> &idT, vector<Point> &posT);

int main(int argc, char *argv[]) {
    int opt;
    parseArgs(argc, argv, opt);
    sem_init(&arraySemaphore, 0, 1);
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            terrain[j][i] = new Cell(0); //initialise les cellules a zero
        }
    }

    vector<pthread_t> tabT(nbThread);
    vector<int> idT(nbThread);
    vector<Point> posArray(nbThread);

    srand(SEED); //allows the persons to always have the same position
    for (int k = 0; k < 10; ++k) {
        spawnObstacle();
    }

    initPersonPos(posArray);
    high_resolution_clock::time_point start;
//    Person t = Person(nbThread, terrain, metric, counter);
    if (metric) {
        cout << "Running program " << REPEAT << " times with " << nbThread << " threads" << endl;
        array<long, REPEAT> responseTime{};
        array<long, REPEAT> userTime{};
        array<long, REPEAT> systemTime{};
        for (int i = 0; i < REPEAT; i++) {

            start = std::chrono::high_resolution_clock::now();
            createAndWaitThreads(tabT, idT, posArray);
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
        createAndWaitThreads(tabT, idT, posArray);
        auto end = high_resolution_clock::now();
        cout << std::chrono::duration_cast<milliseconds>(end - start).count() << " ms" << endl;
    }
    for (int l = 0; l < terrain.size(); ++l) {
        for (int i = 0; i < terrain[0].size(); ++i) {
            delete terrain[l][i];
        }
    }
    return 0;
}

void createAndWaitThreads(vector<pthread_t> &tabT, vector<int> &idT, vector<Point> &posT) {
    sem_init(&counter, 0, nbThread);
    for (int j = 0; j < nbThread; ++j) {
        Point p = posT[j];
        terrain[p.x][p.y]->spawn();
        idT[j] = pthread_create(&tabT[j], nullptr, Person::salut2,
                                new OptimistPerson(nbThread, terrain, metric, counter, p));
    }
    if (metric || !canDisplay) {
        for (int k = 0; k < nbThread; ++k) {
            pthread_join(tabT[k], nullptr);
        }
    } else {
        display(counter, terrain);
    };
}


void parseArgs(int argc, char *const *argv, int opt) {
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
}

/**
 *  Essaie de créer un obstacle tant que les nombres aléatoires donnent des obtacles invalides
 */
void spawnObstacle() {
    int ox = 2 + rand() % (MAX_X - 80);
    int oy = 2 + rand() % (MAX_Y - 80);
    int ow = 2 + rand() % (MAX_X - ox);
    int oh = 2 + rand() % (MAX_Y - oy - 2);
    for (int i = ox - 2; i < ox + ow + 2; ++i) {
        for (int j = oy - 2; j < oy + oh + 2; ++j) {
            if (terrain[i][j]->readValue() == OBSTACLE) {
                spawnObstacle();
                return;
            }
        }
    }
    for (int i1 = ox; i1 < ox + ow; ++i1) {
        for (int j2 = oy; j2 < oy + oh; ++j2) {
            terrain[i1][j2] = new Cell(OBSTACLE);
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
    for (int i = 0; i < nbThread; i++) {
        int x = rand() % MAX_X;
        int y = rand() % MAX_Y;
        while (terrain[x][y]->readValue() || (x == 0 && y == 0)) {
            x = rand() % MAX_X;
            y = rand() % MAX_Y;
        }
        terrain[x][y]->changeValue(1);
        posVector[i] = {x, y};
    }
    for (int j = 0; j < posVector.size(); ++j) {
        terrain[posVector[j].x][posVector[j].y]->moveOut();
    }
}


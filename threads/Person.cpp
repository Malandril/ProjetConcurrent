//
// Created by user on 18/11/17.
//

#include <array>
#include <semaphore.h>
#include <cmath>
#include <chrono>
#include "Person.h"
#include "../Display.h"
#include "../main.h"
#include "OptimistPerson.h"

/**
 * Cette fonction va créer tous les threads et va les attendres
 * @param tabT
 * @param idT
 * @param posT
 */
//void Person::createAndWaitThreads(vector<pthread_t> &tabT, vector<int> &idT, vector<Point> &posT) {
//    for (int j = 0; j < nbThread; ++j) {
//        idT[j] = pthread_create(&tabT[j], nullptr, salut, (void *) &posT[j]);
//    }
//    if (metric || !canDisplay) {
//        for (int k = 0; k < nbThread; ++k) {
//            pthread_join(tabT[k], nullptr);
//        }
//    } else {
//        display(counter, terrain);
//    };
//}



/**
 * Fonction éxécuté par les threads et qui tourne jusqu'à ce que le thread arrive à destination
 * @param args
 * @return
 */
void *Person::computePath() {
    auto start = std::chrono::high_resolution_clock::now();
    while (pos.x != DEST_X || pos.y != DEST_Y) {
        bestCell();
        if (!metric && canDisplay) {
            waitDisplayRefresh(); //ralentit le deplacement des threads si il y a affichage pour + de lisibilité
        }
    }
    terrain[pos.x][pos.y]->moveOut();
    if (!metric && canDisplay) {// ce compteur permet à l'affichage de verifier quand tous les threads sont finis
        sem_wait(&counter);
    }
//    timespec t;
//    t.tv_sec  = 0;
//    t.tv_nsec = 500000000L;
//    nanosleep(&t, nullptr);
//    auto end = std::chrono::high_resolution_clock::now();
//    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
}

/**
 * Cette fonction permet de trouver La meilleur cellule vers laquelle se deplacer
 * par rapport à la position courante x, y et s'y deplace
 * @param x
 * @param y
 */
void Person::bestCell() {
    double minDistance = distance(DEST_X, DEST_Y, pos.x, pos.y);
    int tmpx = pos.x;
    int tmpy = pos.y;
    double dist;
    for (int i = pos.y - 1; i <= pos.y + 2; ++i) {
        for (int j = pos.x - 1; j < pos.x + 2; ++j) {
            dist = distance(j, i, DEST_X, DEST_Y);
            if (isValidCell(j, i) && dist < minDistance) {
                minDistance = dist;
                tmpx = j;
                tmpy = i;
            }
        }
    }

    if (tmpx != pos.x || tmpy != pos.y) {
        int value = terrain[pos.x][pos.y]->readValue();
        terrain[pos.x][pos.y]->move(*terrain[tmpx][tmpy]);
        pos.x = tmpx;
        pos.y = tmpy;
    }
}

/**
 * Cette fonction vérifie si la cellule est dans le tableau et si elle n'est pas deja prise
 * @param x
 * @param y
 * @return vrai si cellule valide
 */
bool Person::isValidCell(int x, int y) {
    return (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) && (terrain[x][y]->readValue() == 0);
}

/**
 * Distance entre deux cellules
 * @param x
 * @param y
 * @param xDest
 * @param yDest
 * @return
 */
double Person::distance(int x, int y, int xDest, int yDest) {
    return sqrt(pow(xDest - x, 2) + pow(yDest - y, 2));
}

Person::Person(int nbThread, vector<vector<Cell *>> &terrain, bool metric, sem_t &counter, Point pos)
        : Threading(nbThread, terrain, metric, counter), pos(pos) {
}

void *Person::salut(void *args) {
    if (args != nullptr) {
        OptimistPerson t = *((OptimistPerson *) args);
        delete ((OptimistPerson *) args);
        t.computePath();
    }
}

void *Person::salut2(void *args) {
    if (args != nullptr) {
        Person t = *((Person *) args);
        delete ((Person *) args);
        t.computePath();
    }
}
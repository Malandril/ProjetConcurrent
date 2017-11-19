//
// Created by user on 19/11/17.
//

#include <iostream>
#include "OptimistPerson.h"
#include "../main.h"

OptimistPerson::OptimistPerson(int nbThread, vector<vector<Cell *>> &terrain, bool metric, sem_t &counter,
                               const Point pos) : Person(nbThread, terrain, metric, counter, pos) {}

void OptimistPerson::bestCell() {
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
        sem_t &sem = terrain[pos.x][pos.y]->cellSemaphore;
        sem_wait(&sem);
        sem_wait(&terrain[tmpx][tmpy]->cellSemaphore);

        int value = terrain[pos.x][pos.y]->readValue();
        if (value != 1) {
            std::cout << value << std::endl;
        }
        int dest = terrain[tmpx][tmpy]->readValue();
        if (value == 1 && dest == 0) {
            terrain[pos.x][pos.y]->move(*terrain[tmpx][tmpy]);
            pos.x = tmpx;
            pos.y = tmpy;
        } else {
//            std::cout << value << " " << dest << std::endl;

        }
        sem_post(&sem);
        sem_post(&terrain[tmpx][tmpy]->cellSemaphore);
    }
}

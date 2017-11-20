//
// Created by user on 19/11/17.
//

#include <iostream>
#include "OptimistPerson.h"
#include "../main.h"

OptimistPerson::OptimistPerson(int nbThread, vector<vector<Cell *>> &terrain, bool metric, sem_t &counter,
                               const Point pos) : Person(nbThread, terrain, metric, counter, pos) {
    int value = terrain[pos.x][pos.y]->readValue();
    if (value != 1) {
        std::cout << value << " AAAAAAAAAAA" << std::endl;
    }
}

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
        sem_wait(&terrain[tmpx][tmpy]->cellSemaphore);
        int value = terrain[pos.x][pos.y]->readValue();
        int dest = terrain[tmpx][tmpy]->readValue();
        if (value != 1) {
//            std::cout << value <<" b "<<dest<< std::endl;
        }
        if (dest == 0) {
            terrain[pos.x][pos.y]->move(*terrain[tmpx][tmpy]);
            pos.x = tmpx;
            pos.y = tmpy;
        } else {
//            std::cout << terrain[pos.x][pos.y]->readValue() << " SDQDD " <<  terrain[tmpx][tmpy]->readValue()<<std::endl;
        }
        sem_post(&terrain[tmpx][tmpy]->cellSemaphore);
        value = terrain[pos.x][pos.y]->readValue();
        int b = terrain[tmpx][tmpy]->readValue();
        if (value != 1) {
            std::cout << value << "BBBBBBBBBBBBBBBBBBBBBBB " << b << std::endl;
        }
    }
}

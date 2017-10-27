//
// Created by user on 20/10/2017.
//

#include "Cell.h"

Cell::Cell(int value) : value(value) {
    sem_init(&rwMutex, 0, 1);
    sem_init(&inCellMutex, 0, 1);
}

void Cell::moveIn() {
    sem_wait(&inCellMutex);
    changeValue(1);
}

void Cell::moveOut() {
    changeValue(0);
    sem_post(&inCellMutex);
}

int Cell::readValue() {
    sem_wait(&rwMutex);
    int value = this->value;
    sem_post(&rwMutex);
    return value;
}

void Cell::move(Cell &cell) {
    moveOut();
    cell.moveIn();
}

void Cell::changeValue(int value) {
    sem_wait(&rwMutex);
    this->value = value;
    sem_post(&rwMutex);
}

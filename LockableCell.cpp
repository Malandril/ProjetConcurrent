//
// Created by user on 20/10/2017.
//

#include "LockableCell.h"

LockableCell::LockableCell(int value) : Cell(value) {
    sem_init(&rwMutex, 0, 1);
    sem_init(&inCellMutex, 0, 1);
}

void LockableCell::moveIn() {
    sem_wait(&inCellMutex);
    changeValue(1);
}

void LockableCell::moveOut() {
    changeValue(0);
    sem_post(&inCellMutex);
}

int LockableCell::readValue() {
    sem_wait(&rwMutex);
    int value = this->value;
    sem_post(&rwMutex);
    return value;
}

void LockableCell::move(Cell &cell) {
    moveOut();
    cell.moveIn();
}

void LockableCell::changeValue(int value) {
    sem_wait(&rwMutex);
    this->value = value;
    sem_post(&rwMutex);
}

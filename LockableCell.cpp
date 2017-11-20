//
// Created by user on 20/10/2017.
//

#include "LockableCell.h"

LockableCell::LockableCell(int value) : Cell(value) {
    sem_init(&rwMutex, 0, 1);
    pthread_mutex_init(&inCellMutex, nullptr);
    pthread_cond_init(&cond, nullptr);
}

void LockableCell::moveIn() {
    pthread_mutex_lock(&inCellMutex);
    while (this->value == 1)
        pthread_cond_wait(&cond, &inCellMutex);
    changeValue(1);
    pthread_mutex_unlock(&inCellMutex);
}

void LockableCell::moveOut() {
    changeValue(0);
    pthread_cond_broadcast(&cond);
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

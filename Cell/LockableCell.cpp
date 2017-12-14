//
// Created by user on 20/10/2017.
//

#include "LockableCell.h"

LockableCell::LockableCell(int value) : Cell(value) {
    sem_init(&cellSemaphore, 0, 1);
    sem_init(&rwSemaphore, 0, 1);
}

void LockableCell::moveIn() {
    sem_wait(&cellSemaphore);
    changeValue(1);
}

void LockableCell::moveOut() {
    changeValue(0);
    sem_post(&cellSemaphore);
}

int LockableCell::readValue() {
    sem_wait(&rwSemaphore);
    int value = this->value;
    sem_post(&rwSemaphore);
    return value;
}


void LockableCell::move(Cell &cell) {
    this->moveOut();
    cell.moveIn();
}

void LockableCell::changeValue(int value) {
    sem_wait(&rwSemaphore);
    this->value = value;
    sem_post(&rwSemaphore);
}

LockableCell::~LockableCell() {
    sem_destroy(&rwSemaphore);
    sem_destroy(&cellSemaphore);
}

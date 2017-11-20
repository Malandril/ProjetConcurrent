//
// Created by user on 18/11/17.
//

#include "Cell.h"

Cell::Cell(int value) : value(value) {
    sem_init(&cellSemaphore, 0, 1);
}

void Cell::moveIn() {
    changeValue(1);
}

void Cell::moveOut() {
    changeValue(0);
}

void Cell::spawn() {
    sem_wait(&cellSemaphore);
    if (value == 0) {
        changeValue(1);
    } else {
        sem_post(&cellSemaphore);
        spawn();
        return;
    }
    sem_post(&cellSemaphore);
}

int Cell::readValue() {
    return this->value;
}

void Cell::move(Cell &cell) {
    changeValue(0);
    cell.changeValue(1);

}

void Cell::changeValue(int value) {
    this->value = value;

}


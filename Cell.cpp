//
// Created by user on 20/10/2017.
//

#include "Cell.h"

Cell::Cell(int value) : value(value) {
    sem_init(&cellMutex, 0, 1);
}

void Cell::moveIn() {
    changeValue(1);
}

void Cell::moveOut() {
    changeValue(0);

}

int Cell::readValue() {
    sem_wait(&cellMutex);
    int value = this->value;
    sem_post(&cellMutex);
    return value;
}

void Cell::move(Cell &cell) {
    moveOut();
    cell.moveIn();
}

void Cell::changeValue(int value) {
    sem_wait(&cellMutex);
    this->value = value;
    sem_post(&cellMutex);


}

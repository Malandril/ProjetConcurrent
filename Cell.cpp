//
// Created by user on 20/10/2017.
//

#include "Cell.h"

Cell::Cell(int value) : value(value) {}

void Cell::moveIn() {
    changeValue(1);
}

void Cell::moveOut() {
    changeValue(0);

}

int Cell::readValue() {
    pthread_mutex_lock(&cellMutex);
    int value = this->value;
    pthread_mutex_unlock(&cellMutex);
    return value;
}

void Cell::move(Cell &cell) {
    moveOut();
    cell.moveIn();
}

void Cell::changeValue(int value) {
    pthread_mutex_lock(&cellMutex);
    this->value = value;
    pthread_mutex_unlock(&cellMutex);


}

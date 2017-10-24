//
// Created by user on 20/10/2017.
//

#include "Cell.h"

Cell::Cell(int value) : value(value) {}

void Cell::moveIn() {
    pthread_mutex_lock(&cellMutex);
    changeValue(1);


}

void Cell::moveOut() {
    changeValue(0);
    pthread_mutex_unlock(&cellMutex);

}

int Cell::readValue() {

    pthread_mutex_lock(&readMutex);
    readerCount++;
    if (readerCount == 1) {
        pthread_mutex_lock(&writeMutex);
    }
    pthread_mutex_unlock(&readMutex);

    int value = this->value;

    pthread_mutex_lock(&readMutex);
    readerCount--;
    if (readerCount == 0) {
        pthread_mutex_unlock(&writeMutex);
    }
    pthread_mutex_unlock(&readMutex);
    return value;
}

void Cell::move(Cell &cell) {
    moveOut();
    cell.moveIn();
}

void Cell::changeValue(int value) {
    pthread_mutex_lock(&writeMutex);//lock ecriture
    this->value = value;
    pthread_mutex_unlock(&writeMutex);


}

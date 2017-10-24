//
// Created by user on 20/10/2017.
//

#include "Cell.h"

Cell::Cell(int value) : value(value) {}

void Cell::moveIn() {
    //lock ecriture
    pthread_mutex_lock(&writeMutex);
    changeValue(1);
    pthread_mutex_unlock(&writeMutex);


}

void Cell::moveOut() {
    // lock eciture
    pthread_mutex_lock(&writeMutex);
    changeValue(0);
    pthread_mutex_unlock(&writeMutex);

}

int Cell::readValue() {
    return value;
}

void Cell::move() {
    //lock emplacement permet de ne pas avoir de personne qui rentrent après avoir lu une valeur
    pthread_mutex_lock(&cellMute);
    moveIn();
    moveOut();
    pthread_mutex_unlock(&cellMute);
    //unlock emplacement
}

void Cell::changeValue(int value) {
    this->value = value;

}

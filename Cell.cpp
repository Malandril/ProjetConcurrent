//
// Created by user on 20/10/2017.
//

#include "Cell.h"

Cell::Cell(int x, int y) : x(x), y(y) {}

void Cell::moveIn() {
    //lock ecriture

}

void Cell::moveOut() {
    // lock eciture
}

int Cell::readValue() {
    return value;
}

void Cell::move() {
    //lock emplacemnt
    moveIn();
    moveOut();
    //lock
}

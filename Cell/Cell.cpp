//
// Created by user on 18/11/17.
//

#include "Cell.h"

Cell::Cell(int value) : value(value) {
}

void Cell::moveIn() {
    changeValue(1);

}

void Cell::moveOut() {
    changeValue(0);
}

void Cell::spawn() {
    value = 1;
}

int Cell::readValue() {
    return this->value;
}

void Cell::move(Cell &cell) {
    changeValue(0);
    cell.changeValue(1);

}

void Cell::move(Cell &cell, Point from) {
    changeValue(0);
    cell.changeValue(1);
}

void Cell::changeValue(int value) {
    this->value = value;
}

Cell::~Cell() = default;

Cell::Cell(int value, int x, int y) : x(x), y(y), value(value) {}


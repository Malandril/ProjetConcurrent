//
// Created by user on 18/11/17.
//

#ifndef PROJETCONCURRENT_SPECIALCELL_H
#define PROJETCONCURRENT_SPECIALCELL_H


#include "../Point.h"

class ThreadPart;

class Cell {
public:
    int x = -1;
    int y = -1;

    Cell(int x, int y, int value);

    ThreadPart *owner = nullptr;

    explicit Cell(int value = 0);

    virtual void moveIn();

    virtual void moveOut();

    virtual int readValue();

    virtual void move(Cell &cell);

    void move(Cell &cell, Point from);

    void spawn();

    virtual ~Cell();

    virtual void changeValue(int value);

protected:

    int value;
};


#endif //PROJETCONCURRENT_SPECIALCELL_H

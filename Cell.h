//
// Created by user on 18/11/17.
//

#ifndef PROJETCONCURRENT_SPECIALCELL_H
#define PROJETCONCURRENT_SPECIALCELL_H


#include <semaphore.h>

class Cell {
public:
    sem_t cellSemaphore;

    explicit Cell(int value = 0);

    virtual void moveIn();

    virtual void moveOut();

    virtual int readValue();

    virtual void move(Cell &cell);

    void spawn();

    virtual void changeValue(int value);

protected:

    int value;
};


#endif //PROJETCONCURRENT_SPECIALCELL_H

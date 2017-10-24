//
// Created by user on 20/10/2017.
//

#ifndef PROJETCONCURRENT_PERSON_H
#define PROJETCONCURRENT_PERSON_H


#include <pthread.h>

class Cell {
private:
    int value;

    pthread_mutex_t writeMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t cellMute = PTHREAD_MUTEX_INITIALIZER;

    void changeValue(int value);

public:

    explicit Cell(int value=0);

    void moveIn();

    void moveOut();

    void move();

    int readValue();


};


#endif //PROJETCONCURRENT_PERSON_H

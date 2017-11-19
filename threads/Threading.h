//
// Created by user on 19/11/17.
//

#ifndef PROJETCONCURRENT_THREADING_H
#define PROJETCONCURRENT_THREADING_H


#include <pthread.h>
#include <array>
#include <vector>
#include "../Point.h"
#include "../Cell.h"

using std::array;
using std::vector;

class Threading {
protected:
    int nbThread;
    vector<vector<Cell *>> &terrain;
    bool metric;
    sem_t &counter;
public:
    Threading(int nbThread, vector<vector<Cell *>> &terrain, bool metric, sem_t &counter) : nbThread(nbThread),
                                                                                            terrain(terrain),
                                                                                            metric(metric),
                                                                                            counter(counter) {}

//    virtual void createAndWaitThreads(vector<pthread_t> &tabT, vector<int> &idT, vector<Point> &posT)= 0;

    virtual void *computePath()=0;
};


#endif //PROJETCONCURRENT_THREADING_H

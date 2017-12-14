//
// Created by user on 19/11/17.
//

#ifndef PROJETCONCURRENT_THREADING_H
#define PROJETCONCURRENT_THREADING_H


#include "../Cell/Cell.h"
#include <vector>

using std::vector;

class Threading {
protected:
    int nbThread;
    vector<vector<Cell *>> &terrain;
    bool metric;

    virtual bool isValidCell(int x, int y);

    double distance(int x, int y, int xDest, int yDest);

public:
    Threading(int nbThread, vector<vector<Cell *>> &terrain, bool metric) : nbThread(nbThread),
                                                                            terrain(terrain),
                                                                            metric(metric) {}

    virtual ~Threading();

    virtual void *computePath()=0;

    bool finished = false;

    void static *threadFunction(void *args);
};


#endif //PROJETCONCURRENT_THREADING_H

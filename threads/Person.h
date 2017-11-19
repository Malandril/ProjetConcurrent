//
// Created by user on 18/11/17.
//

#ifndef PROJETCONCURRENT_THREADWORKER_H
#define PROJETCONCURRENT_THREADWORKER_H


#include <vector>
#include "Threading.h"
#include "../Cell.h"

class Person : public Threading {

public:
//    void createAndWaitThreads(vector<pthread_t> &tabT, vector<int> &idT, vector<Point> &posT) override;

    void *computePath() override;

    Person(int nbThread, vector<vector<Cell *>> &terrain, bool metric, sem_t &counter, Point pos);

    void static *salut(void *);

    void static *salut2(void *args);

protected:

    Point pos;

    virtual void bestCell();

    bool isValidCell(int x, int y);

    double distance(int x, int y, int xDest, int yDest);
};


#endif //PROJETCONCURRENT_THREADWORKER_H

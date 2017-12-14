//
// Created by user on 18/11/17.
//

#ifndef PROJETCONCURRENT_THREADWORKER_H
#define PROJETCONCURRENT_THREADWORKER_H


#include <list>
#include "Threading.h"
#include "../Point.h"

class PersonThread : public Threading {

public:

    void *computePath() override;

    PersonThread(int nbThread, vector<vector<Cell *>> &terrain, bool metric, Point pos);

protected:

    Point pos;

    virtual void getBestCell(Point &point);

    void moveToBestCell();
};


#endif //PROJETCONCURRENT_THREADWORKER_H

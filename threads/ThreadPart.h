//
// Created by user on 29/11/17.
//

#ifndef PROJETCONCURRENT_THREADPART_H
#define PROJETCONCURRENT_THREADPART_H

#include <list>
#include <stack>
#include <queue>
#include <pthread.h>
#include "PersonThread.h"

using std::list;

class ThreadPart : public PersonThread {
private:
    list<Point> persons;
    list<Point> limitPersons;
protected:
    pthread_mutex_t partLock;
    pthread_cond_t cond;

    void writeColumn(int j);

    void writeLine(int j);

    Point size;

    void moveToBestCell(list<Point> &persons);

    bool isValidCell(int x, int y) override;


public:
    ThreadPart(int nbThread, vector<vector<Cell *>> &terrain, bool metric, Point pos, Point size);

    virtual void spawnPerson(Point pos);

    void *computePath() override;

    void printHistory();

    virtual void wake();

    virtual void mainLoop();

    bool isInLimits(Point pos);

    bool contains(Point point);

    void addPerson(Point point);

    virtual ~ThreadPart();

    std::string strhistory();

    std::string personString();

    Point getPos();
};


#endif //PROJETCONCURRENT_THREADPART_H

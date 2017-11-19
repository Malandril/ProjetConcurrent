//
// Created by user on 19/11/17.
//

#ifndef PROJETCONCURRENT_OPTIMISTPERSON_H
#define PROJETCONCURRENT_OPTIMISTPERSON_H


#include "Person.h"

class OptimistPerson : public Person {
public:
    OptimistPerson(int nbThread, vector<vector<Cell *>> &terrain, bool metric, sem_t &counter, const Point pos);

    void bestCell() override;

private:
};


#endif //PROJETCONCURRENT_OPTIMISTPERSON_H

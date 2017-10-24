//
// Created by user on 20/10/17.
//
#include <pthread.h>
#include "Cell.h"

#ifndef PROJETCONCURRENT_DISPLAYSDL_H
#define PROJETCONCURRENT_DISPLAYSDL_H
bool hasTrueDisplay();

void display(int max_x, int max_y, int &counter, Cell terrain[]);

void displayWaitRefresh();

#endif //PROJETCONCURRENT_DISPLAYSDL_H

//
// Created by user on 20/10/17.
//
#include <pthread.h>

#ifndef PROJETCONCURRENT_DISPLAYSDL_H
#define PROJETCONCURRENT_DISPLAYSDL_H
bool hasTrueDisplay();
void display(int max_x, int max_y, int &counter, int terrain[]);

void displayWaitRefresh();

#endif //PROJETCONCURRENT_DISPLAYSDL_H

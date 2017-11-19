//
// Created by user on 20/10/17.
//
#include <pthread.h>
#include "LockableCell.h"
#include <semaphore.h>

#ifndef PROJETCONCURRENT_DISPLAYSDL_H
#define PROJETCONCURRENT_DISPLAYSDL_H

#include <vector>

using std::vector;
/**
 * vrai si compilé avec la partie graphique
 */
extern bool canDisplay;

/**
 * Affiche le terrain si compilé avec la partie graphique
 * @param counter
 * @param terrain
 */
void display(sem_t &counter, vector<vector<Cell *>> &terrain);

/**
 * Permet de ralentir le deplacement des threads si il y a affichage pour plus de lisibilité
 */
void waitDisplayRefresh();

#endif //PROJETCONCURRENT_DISPLAYSDL_H

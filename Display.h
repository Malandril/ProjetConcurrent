//
// Created by user on 20/10/17.
//
#ifndef PROJETCONCURRENT_DISPLAYSDL_H
#define PROJETCONCURRENT_DISPLAYSDL_H

#include <semaphore.h>
#include <pthread.h>
#include <vector>
#include "Cell/Cell.h"
#include "threads/ThreadPart.h"

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
void *display(void *arg);

/**
 * Permet de ralentir le deplacement des threads si il y a affichage pour plus de lisibilité
 */
void waitDisplayRefresh();

#endif //PROJETCONCURRENT_DISPLAYSDL_H

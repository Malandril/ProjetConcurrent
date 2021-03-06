//
// Created by user on 20/10/2017.
//

#ifndef PROJETCONCURRENT_PERSON_H
#define PROJETCONCURRENT_PERSON_H


#include <pthread.h>
#include <ostream>
#include <semaphore.h>
#include "Cell.h"

/**
 * classe utilisé pour la version 0 (avec autant de threads que de personnes)
 */
class LockableCell : public Cell {
protected:
    sem_t rwSemaphore;
    sem_t cellSemaphore;
public:

    /**
     * Change la valeur de la cellule en entrant dans la section critique controlée par le Mutex writeMutex
     * de ne pas avoir de données incohérentes si il y des modifications concurrentes
     * @param value
     */
    void changeValue(int value) override;

    /**
     * Constructeur de Cell
     * @param value
     */
    explicit LockableCell(int value = 0);

    /**
     * Fait "bouger" une personne dans la cellule et entre dans la section critique du mutex cellMutex(cellmutexc
     * Cela permet de bloquer les accès suivant à moveIn j'usqu'à ceque la cellule soit libéré avec moveOut
     * ceci permet d'empecher que lorsque deux thread lise la valeur d'une cellule en même temps ils souhaitent tout
     * les deux s'y déplacer si il y n'y avait pas cela rien n'empecherait qu'ils bougent l'un après l'autre
     */
    void moveIn() override;

    void moveOut() override;

    virtual ~LockableCell();

    int readValue() override;

    void move(Cell &cell) override;

};


#endif //PROJETCONCURRENT_PERSON_H

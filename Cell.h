//
// Created by user on 20/10/2017.
//

#ifndef PROJETCONCURRENT_PERSON_H
#define PROJETCONCURRENT_PERSON_H


#include <pthread.h>
#include <ostream>

class Cell {
private:
    int value;
    int readerCount = 0;
    pthread_mutex_t cellMutex = PTHREAD_MUTEX_INITIALIZER;

    /**
     * Change la valeur de la cellule en entrant dans la section critique controlée par le Mutex writeMutex
     * de ne pas avoir de données incohérentes si il y des modifications concurrentes
     * @param value
     */
    void changeValue(int value);

public:
    /**
     * Constructeur de Cell
     * @param value
     */
    explicit Cell(int value = 0);

    /**
     * Fait "bouger" une personne dans la cellule et entre dans la section critique du mutex cellMutex(cellmutexc
     * Cela permet de bloquer les accès suivant à moveIn j'usqu'à ceque la cellule soit libéré avec moveOut
     * ceci permet d'empecher que lorsque deux thread lise la valeur d'une cellule en même temps ils souhaitent tout
     * les deux s'y déplacer si il y n'y avait pas cela rien n'empecherait qu'ils bougent l'un après l'autre
     */
    void moveIn();

    void moveOut();

    int readValue();

    void move(Cell &cell);
};


#endif //PROJETCONCURRENT_PERSON_H

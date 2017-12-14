//
// Created by user on 29/11/17.
//

#include <iostream>
#include "ThreadPart.h"
#include "../main.h"
#include "../Display.h"


ThreadPart::ThreadPart(int nbThread, vector<vector<Cell *>> &terrain, bool metric, Point pos,
                       Point size) : PersonThread(nbThread, terrain, metric, pos), size(size) {
    pthread_mutex_init(&partLock, nullptr);
    pthread_cond_init(&cond, nullptr);
}

void *ThreadPart::computePath() {
    if (pos.y > 0)
        writeLine(pos.y);
    if (pos.y + size.y < terrain[0].size() - 1)
        writeLine(pos.y + size.y);

    if (pos.x > 0)
        writeColumn(pos.x);
    if (pos.x + size.x < terrain.size() - 1)
        writeColumn(pos.x + size.x);

    mainLoop();
}

/**
 * Boucle principale qui permet de lancer à chaque fois moveToBestCell
 * si il n'ya plus personne dans cette partie alors la fonction attend un reveil
 * si toutees les autres parties sont vide alors on les reveilles et on s'arrete
 */
void ThreadPart::mainLoop() {
    while (true) {
        if (!metric && canDisplay) {
            waitDisplayRefresh(); //ralentit le deplacement des threads si il y a affichage pour + de lisibilité
        }
        moveToBestCell(persons);
        pthread_mutex_lock(&partLock);
        while (persons.empty() && limitPersons.empty()) {
            int test = 3;
            finished = true;
            for (auto threadPart : threadParts) {
                if (threadPart != this) {
                    if (threadPart->finished) {
                        test--;
                    }
                }
            }
            if (test == 0) {
                pthread_mutex_unlock(&partLock);
                wakeAllThreadParts();
                return;
            }
            pthread_cond_wait(&cond, &partLock);
        }
        moveToBestCell(limitPersons);
        for (auto p = limitPersons.begin(); p != limitPersons.end();) {
            if (!isInLimits(*p)) {
                persons.push_back(*p);
                p = limitPersons.erase(p);
            } else {
                p++;
            }
        }
        pthread_mutex_unlock(&partLock);
    }
}

/**
 * Renvoie vrai si la cellule est valide
 * @param x
 * @param y
 * @return
 */
bool ThreadPart::isValidCell(int x, int y) {
    return Threading::isValidCell(x, y) && terrain[x][y]->readValue() == 0;
}

/**
 * Défini que cette partie possède la ligne j
 * @param j
 */
void ThreadPart::writeLine(int j) {
    for (int i = pos.x; i < pos.x + size.x + 1; ++i) {
        terrain[i][j]->owner = this;
    }
}

/**
 * Défini que cette partie possède la colonne j
 * @param j
 */
void ThreadPart::writeColumn(int j) {
    for (int i = pos.y; i < pos.y + size.y + 1; ++i) {
        terrain[j][i]->owner = this;
    }

}

/**
 * bouge les points de la liste donnée vers une case valide si il y en a une
 * si cela doit faire changer de case alors on lock la partie de destination et on appelle dessus addPerson
 * puis on déplace le point sur le terrain et on supprime le point de la liste et
 *  on libère le verrou
 * @param persons
 */
void ThreadPart::moveToBestCell(list<Point> &persons) {
    for (auto start = persons.begin(); start != persons.end();) {
        Point person = *start;
        Cell *cell = terrain[start->x][start->y];
        bool erased = false;
        getBestCell(person);

        if (start->x == DEST_X && start->y == DEST_Y) {
            terrain[start->x][start->y]->moveOut();
            start = persons.erase(start);
            erased = true;
        } else if (person.x != start->x || person.y != start->y) {
            Cell *dest = terrain[person.x][person.y];
            if (dest->owner != nullptr && dest->owner != this) {
                pthread_mutex_lock(&dest->owner->partLock);
                if (dest->readValue() != 1) {
                    dest->owner->addPerson(person);
                    cell->move(*dest, person);
                    start = persons.erase(start);
                    erased = true;
                }
                pthread_mutex_unlock(&dest->owner->partLock);
            } else {
                cell->move(*dest, person);

                *start = person;
            }
        }
        if (!erased) {
            start++;
        }

    }

}

ThreadPart::~ThreadPart() {
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&partLock);
}

/**
 * renvoie vrai si le point est dans cette partie
 * @param point
 * @return
 */
bool ThreadPart::contains(Point point) {
    return point.x >= pos.x && point.y >= pos.y && point.x <= size.x + pos.x && point.y <= size.y + pos.y;
}

/**
 * ajoute une personne à la liste de personnes qui sont dans la limite et reveille ce thread si il est en attente
 * @param point
 */
void ThreadPart::addPerson(Point point) {
    bool b = limitPersons.empty() && persons.empty();
    this->limitPersons.push_back(point);
    if (b) {
        finished = false;
        pthread_cond_broadcast(&cond);
    }
}

/**
 *Ajoute une personne a la bonne liste fonction utilisée a l'initialisation
 * @param pos
 */
void ThreadPart::spawnPerson(Point pos) {
    if (isInLimits(pos)) {
        this->limitPersons.push_back(pos);
    } else {
        this->persons.push_back(pos);
    }
}

/**
 * reveille cette partie
 */
void ThreadPart::wake() {
    pthread_cond_broadcast(&cond);
}

/**
 * renvoie vrai si le point est dans la limite
 * @param p
 * @return
 */
bool ThreadPart::isInLimits(Point p) {
    return p.x == pos.x + size.x || p.y == pos.y + size.y;
}
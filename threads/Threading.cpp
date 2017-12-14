//
// Created by user on 27/11/17.
//

#include <cmath>
#include "PersonThread.h"
#include "../main.h"

void *Threading::threadFunction(void *args) {
    if (args != nullptr) {
        Threading *threading = (Threading *) args;
        threading->computePath();
    }
}

/**
 * Cette fonction vérifie si la cellule est dans le tableau et si elle n'est pas un obstacle
 * @param x
 * @param y
 * @return vrai si cellule valide
 */
bool Threading::isValidCell(int x, int y) {
    return (x >= 0 && x < MAX_X && y >= 0 && y < MAX_Y) && (terrain[x][y]->readValue() != 7);
}

/**
 * Distance entre deux cellules
 * @param x
 * @param y
 * @param xDest
 * @param yDest
 * @return
 */
double Threading::distance(int x, int y, int xDest, int yDest) {
    return sqrt(pow(xDest - x, 2) + pow(yDest - y, 2));
}

Threading::~Threading() = default;

//
// Created by user on 18/11/17.
//

#include <array>
#include "PersonThread.h"
#include "../Display.h"
#include "../main.h"


/**
 * Fonction éxécuté par les threads et qui tourne jusqu'à ce que le thread arrive à destination
 * @param args
 * @return
 */
void *PersonThread::computePath() {
    terrain[pos.x][pos.y]->moveIn();
    while (pos.x != DEST_X || pos.y != DEST_Y) {
        moveToBestCell();
        if (!metric && canDisplay) {
            waitDisplayRefresh(); //ralentit le deplacement des threads si il y a affichage pour + de lisibilité
        }
    }
    terrain[pos.x][pos.y]->moveOut();
}

/**
 * Cette fonction permet de trouver La meilleur cellule vers laquelle se deplacer
 * par rapport à la position courante x, y et s'y deplace
 * @param x
 * @param y
 */
void PersonThread::moveToBestCell() {
    Point p = pos;
    getBestCell(p);
    if (p.x != pos.x || p.y != pos.y) {
        terrain[pos.x][pos.y]->move(*terrain[p.x][p.y]);
        pos.x = p.x;
        pos.y = p.y;
    }
}

void PersonThread::getBestCell(Point &point) {
    double minDistance = distance(DEST_X, DEST_Y, point.x, point.y);
    double dist;
    Point next = point;
    for (int i = point.y - 1; i < point.y + 1; ++i) {
        for (int j = point.x - 1; j < point.x + 1; ++j) {
            dist = distance(j, i, DEST_X, DEST_Y);
            if (isValidCell(j, i) && dist < minDistance) {
                minDistance = dist;
                next = Point(j, i);
                break;
            }
        }
    }
    point.x = next.x;
    point.y = next.y;
}

PersonThread::PersonThread(int nbThread, vector<vector<Cell *>> &terrain, bool metric, Point pos)
        : Threading(nbThread, terrain, metric), pos(pos) {
}


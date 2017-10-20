//
// Created by user on 20/10/2017.
//

#ifndef PROJETCONCURRENT_PERSON_H
#define PROJETCONCURRENT_PERSON_H


class Cell {
private:
    int value;
public:

    void moveIn();

    void moveOut();

    void move();

    int x;
    int y;

    Cell(int x, int y);

    int readValue();


};


#endif //PROJETCONCURRENT_PERSON_H

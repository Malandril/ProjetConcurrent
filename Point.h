//
// Created by user on 19/11/17.
//

#ifndef PROJETCONCURRENT_POINT_H
#define PROJETCONCURRENT_POINT_H


#include <ostream>

class Point {
public:
    int x;
    int y;

    Point(int x = 0, int y = 0);

    friend bool operator==(const Point &lhs, const Point &rhs);

    friend bool operator!=(const Point &lhs, const Point &rhs);

    friend std::ostream &operator<<(std::ostream &os, const Point &point);
};

#endif //PROJETCONCURRENT_POINT_H

//
// Created by user on 19/11/17.
//

#include "Point.h"

Point::Point(int x, int y) : x(x), y(y) {}


bool operator==(const Point &lhs, const Point &rhs) {
    return lhs.x == rhs.x &&
           lhs.y == rhs.y;
}

std::ostream &operator<<(std::ostream &os, const Point &point) {
    os << "x: " << point.x << " y: " << point.y;
    return os;
}

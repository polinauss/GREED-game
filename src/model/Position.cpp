#include "model/Position.h"
#include <iostream>

Position::Position(int x, int y): _x(x), _y(y) {}

int Position::getX() const {
    return _x;
}

int Position::getY() const {
    return _y;
}

bool Position::operator== (const Position& other) const {
    return (_x == other._x) && (_y == other._y);
}

Position Position::operator+(const Position& other) const {
    return Position(_x + other._x, _y + other._y);
}

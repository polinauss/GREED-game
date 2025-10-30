#include "model/Cell.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>


Cell::Cell(int value, Color color): _value(value), _color(color), _available(true) {}

int Cell::getValue() const {
    return _value;
}

void Cell::setValue(int value) {
    _value = value;
}

Color Cell::getColor() const {
    return _color;
}

bool Cell::isAvailable() const {
    return _available;
}

void Cell::setAvailable(bool available) {
    _available = available;
}
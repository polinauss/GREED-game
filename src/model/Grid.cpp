#include "model/Grid.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

Grid::Grid(int width, int height): _width(width), _height(height) {
    InitializeRandom();
}

void Grid::InitializeRandom() {
    _cells.clear();
    _cells.reserve(_height * _width);
    
    for (int i = 0; i < _height * _width; ++i) {
        int value = rand() % 9 + 1;
        Color color = gameColors[rand() % gameColors.size()];
        _cells.emplace_back(value, color);
    }
}

Cell& Grid::getCell(Position position) {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position out of range | Grid::getCell");
    }
    return _cells[position.getY() * _width + position.getX()];
}

const Cell& Grid::getCell(Position position) const {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position out of range | Grid::getCell");
    }
    return _cells[position.getY() * _width + position.getX()];
}

bool Grid::isValidPosition(Position position) const {
    return 
        position.getX() < _width &&
        position.getX() >= 0 &&
        position.getY() < _height &&
        position.getY() >= 0;
}

void Grid::removeCell(Position position) {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position out of range | Grid::removeCell");
    }
    _cells[position.getY() * _width + position.getX()].setAvailable(false);
}

int Grid::getWidth() const {
    return _width;
}

int Grid::getHeight() const {
    return _height;
}

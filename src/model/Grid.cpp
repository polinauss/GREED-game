#include "model/Grid.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

Grid::Grid(int width, int height): _width(width), _height(height) {
    initializeRandom();
}

Grid::~Grid() {
    clearCells();
}

void Grid::clearCells() {
    for (ICell* cell: _cells) 
        delete cell;

    _cells.clear();
}

void Grid::initializeRandom() {
    clearCells();
    _cells = _generator.generateRandomGrid(_width, _height);
}

ICell& Grid::operator[] (const Position& position) {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position out of range | Grid::operator[]");
    }
    return *_cells[position.getY() * _width + position.getX()];
}

const ICell& Grid::operator[] (const Position& position) const {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position out of range | Grid::operator[]");
    }
    return *_cells[position.getY() * _width + position.getX()];
}

bool Grid::isValidPosition(const Position& position) const {
    return 
        position.getX() < _width &&
        position.getX() >= 0 &&
        position.getY() < _height &&
        position.getY() >= 0;
}

void Grid::removeCell(const Position& position) {
    if (!isValidPosition(position)) {
        throw std::out_of_range("Position out of range | Grid::removeCell()");
    }
    (*this)[position].setAvailable(false);
}

int Grid::getWidth() const {
    return _width;
}

int Grid::getHeight() const {
    return _height;
}

#include "model/Grid.h"
#include "model/cells/BasicCell.h"
#include "model/cells/TeleportCell.h"
#include "model/cells/BombCell.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

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

void Grid::restoreState(const std::vector<int>& cellValues, 
                       const std::vector<int>& cellColors,
                       const std::vector<int>& cellAvailable,
                       const std::vector<int>& cellTypes,
                       const std::vector<int>& teleportTargetsX,
                       const std::vector<int>& teleportTargetsY) {
    clearCells();
    
    int totalCells = _width * _height;
    if (cellValues.size() != static_cast<size_t>(totalCells) ||
        cellColors.size() != static_cast<size_t>(totalCells) ||
        cellAvailable.size() != static_cast<size_t>(totalCells) ||
        cellTypes.size() != static_cast<size_t>(totalCells) ||
        teleportTargetsX.size() != static_cast<size_t>(totalCells) ||
        teleportTargetsY.size() != static_cast<size_t>(totalCells)) {
        throw std::runtime_error("Invalid grid state data");
    }
    
    for (int i = 0; i < totalCells; i++) {
        int type = cellTypes[i];
        bool available = cellAvailable[i] != 0;
        
        if (type == 1) { // TeleportCell
            int targetX = teleportTargetsX[i];
            int targetY = teleportTargetsY[i];
            TeleportCell* teleportCell = new TeleportCell(Position(targetX, targetY));
            teleportCell->setAvailable(available);
            _cells.push_back(teleportCell);
        }
        else if (type == 2) { // BombCell
            BombCell* bombCell = new BombCell();
            bombCell->setAvailable(available);
            _cells.push_back(bombCell);
        }
        else { // BasicCell
            int value = cellValues[i];
            Color color = static_cast<Color>(cellColors[i]);
            BasicCell* cell = new BasicCell(value, color);
            cell->setAvailable(available);
            _cells.push_back(cell);
        }
    }
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

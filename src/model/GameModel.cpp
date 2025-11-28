#include "model/GameModel.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

GameModel::GameModel(int width, int height): 
    _grid(width, height), 
    _player(Position(width / 2, height / 2)),
    _score(0),
    _gameOver(false),
    _interactionHandler(*this) {
    initializeGame();
}

void GameModel::initializeGame() {
    if (_grid.isValidPosition(_player.getPosition())) {
        _grid.removeCell(_player.getPosition());
    }
}

bool GameModel::isValidMove(Position position) const {
    if (!_grid.isValidPosition(position)) {
        return false;
    }
    const ICell& cell = _grid[position];
    return cell.isAvailable();
}

void GameModel::makeMove(Direction direction) {
    if (_gameOver) return;
    
    Position playerPos = _player.getPosition();
    Position cellPos = playerPos;

    switch(direction) {
        case Direction::UP:
            cellPos = _player.getPosition() + Position(0, -1);
            break;
        case Direction::DOWN:
            cellPos = _player.getPosition() + Position(0, 1);
            break;
        case Direction::LEFT:
            cellPos = _player.getPosition() + Position(-1, 0);
            break;
        case Direction::RIGHT:
            cellPos = _player.getPosition() + Position(1, 0);
            break;
    }  

    if (!isValidMove(cellPos)) {
        _gameOver = true;
        return;
    }

    _grid[cellPos].acceptInteractionStepOn(_interactionHandler, cellPos);
}

bool GameModel::isGameOver() const {
    return _gameOver;
}

int GameModel::getScore() const {
    return _score;
}

Grid& GameModel::getGrid() {
    return _grid;
}

const Grid& GameModel::getGrid() const {
    return _grid;
}

Position GameModel::getPlayerPosition() const {
    return _player.getPosition();
}

const std::vector<Position>& GameModel::getAffectedElements() const {
    return _interactionHandler.getAffectedElements();
}

std::vector<std::pair<bool, Position>>& GameModel::getAvailableMoves() {
    _availableMoves.clear();
    
    Position playerPos = _player.getPosition();
    std::vector<Position> directions = {
        Position(0, -1),  
        Position(0, 1),   
        Position(-1, 0),  
        Position(1, 0)    
    };
    
    for (const auto& dir : directions) {
        Position target = playerPos + dir;
        _availableMoves.emplace_back(_grid.isValidPosition(target), target);
    }
    
    return _availableMoves;
}
#include "model/GameModel.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

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

void GameModel::initializeGameFromState(
    const std::vector<int>& cellValues, 
    const std::vector<int>& cellColors,
    const std::vector<int>& cellAvailable,
    const Position& playerPos,
    int score) 
{
    _score = score;
    _player.setPosition(playerPos);
    _gameOver = false;
    
    _grid.restoreState(cellValues, cellColors, cellAvailable);
    
    if (_grid.isValidPosition(playerPos)) {
        _grid.removeCell(playerPos);
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
    Position targetCellPos(0, 0);

    switch(direction) {
        case Direction::UP:
            targetCellPos = _player.getPosition() + Position(0, -1);
            break;
        case Direction::DOWN:
            targetCellPos = _player.getPosition() + Position(0, 1);
            break;
        case Direction::LEFT:
            targetCellPos = _player.getPosition() + Position(-1, 0);
            break;
        case Direction::RIGHT:
            targetCellPos = _player.getPosition() + Position(1, 0);
            break;
        default:
            return;
    }  

    if (!isValidMove(targetCellPos)) {
        _gameOver = true;
        return;
    }

    _grid[targetCellPos].acceptInteractionStepOn(_interactionHandler, targetCellPos);
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

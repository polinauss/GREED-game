#include "model/GameModel.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

GameModel::GameModel(int width, int height): 
    _grid(width, height), 
    _player(Position(width / 2, height / 2)),
    _score(0),
    _gameOver(false) {
    initializeGame();
    updateGameState();
}

// начало игры: определение позиции игрока
void GameModel::initializeGame() {
    if (_grid.isValidPosition(_player.getPosition())) {
        _grid.removeCell(_player.getPosition());
    }
}

// проверка доступности хода: проверка выхода за границы поля и доступности клетки
bool GameModel::isValidMove(Position position) const {
    if (!_grid.isValidPosition(position)) {
        return false;
    }
    const Cell& cell = _grid.getCell(position);
    return cell.isAvailable();
}

// функция перемещения игрока
bool GameModel::makeMove(Direction direction) {
    if (_gameOver) {
        return false;
    }

    Position currentPos = _player.getPosition();
    Position finalPos = calculateNewPosition(currentPos, direction);

    if (!isValidMove(finalPos)) {
        return false;
    }

    _player.setPosition(finalPos);
    for (Position pos: makeOver(currentPos, finalPos)) {
        _grid.getCell(pos).setAvailable(false);
        _score++;
    }                         

    updateGameState();

    return true;
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

const std::vector<Position>& GameModel::getAvailableMoves(Direction direction) const {
    auto it = _availableMoves.find(direction);
    if (it != _availableMoves.end()) {
        return it->second;
    }
    static std::vector<Position> empty;
    return empty;
}

// вычисление позиции по направлению и смещению
Position GameModel::calculateNewPosition(Position current, Direction direction) const {
    int newX = current.getX(), oldX = current.getX();
    int newY = current.getY(), oldY = current.getY();
    
    switch (direction) {
        case Direction::UP:
            newY -= 1;
            break;
        case Direction::DOWN:
            newY += 1;
            break;
        case Direction::LEFT:
            newX -= 1;
            break;
        case Direction::RIGHT:
            newX += 1;
            break;
        case Direction::NONE:
            return current;
    }
    
    Position interPos = Position(newX, newY);

    if (!isValidMove(interPos))
        return interPos;

    int moveValue = _grid.getCell(interPos).getValue();
    Position finalPos = Position(
        newX + (newX - oldX) * moveValue,
        newY + (newY - oldY) * moveValue
    );

    return finalPos;
}

// проверка состояния игры: проверка наличия доступных ходов
void GameModel::updateGameState() {
    Position playerPos = _player.getPosition();
    bool hasValidMoves = false;
    
    Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    
    for (Direction dir : directions) {
        Position newPos = calculateNewPosition(playerPos, dir);
        if (isValidMove(newPos)) {
            hasValidMoves = true;
            _availableMoves[dir] = makeOver(playerPos, newPos);
        } else {
            _availableMoves[dir] = {};
        }
    }

    _availableMoves[Direction::NONE] = {};
    _gameOver = !hasValidMoves;
}

std::vector<Position> GameModel::makeOver(Position current, Position target) const {
    std::vector<Position> jumpedOver;

    int dx = target.getX() - current.getX();
    int dy = target.getY() - current.getY();
    int stepX = (dx > 0 ? 1 : -1) * static_cast<int>(dx != 0);
    int stepY = (dy > 0 ? 1 : -1) * static_cast<int>(dy != 0);

    int distance = std::max(std::abs(dx), std::abs(dy));
    
    for (int i = 1; i <= distance; i++) {
        jumpedOver.emplace_back(Position(
            current.getX() + i * stepX,
            current.getY() + i * stepY
        ));
    }
    
    return jumpedOver;
}
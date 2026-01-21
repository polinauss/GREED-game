#include "model/InteractionHandler.hpp"
#include "model/GameModel.hpp"
#include <cstdlib>

#define FALSE 0
#define TRUE 1
#define BOMB 2
#define TP 3


InteractionHandler::InteractionHandler(GameModel& model): _model(model), _lastFinalPos(0, 0) {};

int InteractionHandler::collideWithBasicCell(BasicCell& cell) {
    if (!cell.isAvailable()) 
        return FALSE;

    _model._score += cell.getValue();
    cell.setAvailable(false);

    return TRUE;
}

Position InteractionHandler::stepOnBasicCell(BasicCell& cell, const Position& cellPos) {
    Position playerPos = _model._player.getPosition();
    int moveValue = cell.getValue();
    int dx = cellPos.getX() - playerPos.getX();
    int dy = cellPos.getY() - playerPos.getY();
    Position finalPos(
        playerPos.getX() + dx * (moveValue),
        playerPos.getY() + dy * (moveValue)
    );

    _lastFinalPos = finalPos;
    return finalPos;
}

void InteractionHandler::handleStepOnBasicCell(const Position& startPos, const Position& finalPos) {
    _prevMoveAffectedElements.clear();
    _prevMoveAffectedElements.emplace_back(_model._player.getPosition());

    std::vector<Position> jumpedOver = makeOver(_model._player.getPosition(), finalPos);
    
    for (size_t i = 1; i < jumpedOver.size(); i++) {
        Position cellPos = jumpedOver[i];
        
        if (!_model.isValidMove(cellPos)) {
            _model._gameOver = true;
            return;
        }

        int canContinue = _model._grid[cellPos].acceptInteractionColission(*this);

        switch (canContinue) {
            case FALSE:
                _model._gameOver = true;
                return;
                break;
            case TRUE:
                _model._player.setPosition(cellPos);
                _prevMoveAffectedElements.emplace_back(cellPos);
                break;
            case BOMB:
                _model._player.setPosition(cellPos);
                _prevMoveAffectedElements.emplace_back(cellPos);
                return;
                break;
            case TP:
                _prevMoveAffectedElements.emplace_back(cellPos);
                return;
                break;
        }
    }
}

int InteractionHandler::collideWithTeleportCell(TeleportCell& cell) {
    Position tpPos = cell.getTPPos();
    int canContinue = _model._grid[tpPos].acceptInteractionColission(*this);
    if (!canContinue) 
        return FALSE;
    
    cell.setAvailable(false);
    _model._player.setPosition(tpPos);
    _prevMoveAffectedElements.emplace_back(tpPos);

    return TP;
}

void InteractionHandler::stepOnTeleportCell(TeleportCell& cell, const Position& cellPos) {
    _prevMoveAffectedElements.clear();
    _prevMoveAffectedElements.emplace_back(_model._player.getPosition());

    Position tpPos = cell.getTPPos();
    if (!_model.isValidMove(cellPos) || !_model.isValidMove(tpPos)) {
        _model._gameOver = true;
        return;
    }
    cell.setAvailable(false);   

    _prevMoveAffectedElements.emplace_back(cellPos);
    _model._player.setPosition(cellPos);

    int canContinue = _model._grid[tpPos].acceptInteractionColission(*this);
    if (!canContinue) {
        _model._gameOver = true;
        return;
    }
    
    _model._player.setPosition(tpPos);
    _prevMoveAffectedElements.emplace_back(tpPos);

}

int InteractionHandler::collideWithBombCell(BombCell& cell) {
    if (!cell.isAvailable())
        return FALSE;

    _model._score -= static_cast<int>(_model._score * 0.2) + 9;
    cell.setAvailable(false);

    if (_model._score <= 0) {
        _model._score = 0;
        return FALSE;
    }

    return BOMB;
}

void InteractionHandler::stepOnBombCell(BombCell& cell, const Position& cellPos) {
    _prevMoveAffectedElements.clear();
    _prevMoveAffectedElements.emplace_back(_model._player.getPosition());

    if (!_model.isValidMove(cellPos) || !cell.isAvailable()) {
        _model._gameOver = true;
        return;
    }

    _model._player.setPosition(cellPos);
    _prevMoveAffectedElements.emplace_back(cellPos);
    
    int canContinue = _model._grid[cellPos].acceptInteractionColission(*this);
    if (!canContinue) {
        _model._gameOver = true;
        return;
    }
}


std::vector<Position> InteractionHandler::makeOver(const Position& current, const Position& target) const {
    std::vector<Position> jumpedOver;

    int dx = target.getX() - current.getX();
    int dy = target.getY() - current.getY();
    
    int stepX = 0;
    if (dx > 0) stepX = 1;
    else if (dx < 0) stepX = -1;
    
    int stepY = 0;
    if (dy > 0) stepY = 1;
    else if (dy < 0) stepY = -1;

    int distance = std::max(std::abs(dx), std::abs(dy));
    
    for (int i = 0; i <= distance; i++) {
        jumpedOver.emplace_back(Position(
            current.getX() + i * stepX,
            current.getY() + i * stepY
        ));
    }
    
    return jumpedOver;
}

const std::vector<Position>& InteractionHandler::getAffectedElements() const{
    return _prevMoveAffectedElements;
}

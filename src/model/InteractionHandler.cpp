#include "model/InteractionHandler.h"
#include "model/GameModel.h"
#include <cstdlib>

InteractionHandler::InteractionHandler(GameModel& model): _model(model), _lastFinalPos(0, 0) {};

bool InteractionHandler::collideWithBasicCell(BasicCell& cell) {
    if (!cell.isAvailable()) 
        return false;

    _model._score += cell.getValue();
    cell.setAvailable(false);

    return true;
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

        bool canContinue = _model._grid[cellPos].acceptInteractionColission(*this);
        if (!canContinue) {
            _model._gameOver = true;
            return;
        } else {
            _model._player.setPosition(cellPos);
            _prevMoveAffectedElements.emplace_back(cellPos);
        }
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

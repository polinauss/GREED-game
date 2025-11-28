#include "model/InteractionHandler.h"
#include "model/GameModel.h"
#include <cstdlib>

#define GAMEOVER 0
#define GAMECONTINUES 1
#define CHANGESTATE 2

InteractionHandler::InteractionHandler(GameModel& model): _model(model) {};

int InteractionHandler::collideWithBasicCell(BasicCell& cell) {
    if (!cell.isAvailable()) 
        return GAMEOVER;

    _model._score += cell.getValue();
    cell.setAvailable(false);

    return GAMECONTINUES;
}

Position InteractionHandler::stepOnBasicCell(BasicCell& cell, const Position& cellPos) {
    Position playerPos = _model._player.getPosition();
    int moveValue = cell.getValue();

    Position finalPos(
        cellPos.getX() + (cellPos.getX() - playerPos.getX()) * moveValue,
        cellPos.getY() + (cellPos.getY() - playerPos.getY()) * moveValue
    );

    return finalPos;
}

void InteractionHandler::handleStepOnBasicCell(const Position& startPos, const Position& finalPos) {
    _prevMoveAffectedElements.clear();
    _prevMoveAffectedElements.emplace_back(_model._player.getPosition());

    for (Position cellPos: makeOver(startPos, finalPos)) {
        if (!_model.isValidMove(cellPos)) {
            _model._gameOver = true;
            return;
        }

        int state = _model._grid[cellPos].acceptInteractionColission(*this);
        switch (state) {
            case GAMEOVER:
                _model._gameOver = true;
                return;
            case GAMECONTINUES:
                _model._player.setPosition(cellPos);
                _prevMoveAffectedElements.emplace_back(cellPos);
                break;
            case CHANGESTATE:
                return;
        }
    }
}

std::vector<Position> InteractionHandler::makeOver(const Position& current, const Position& target) const {
    std::vector<Position> jumpedOver;

    int dx = target.getX() - current.getX();
    int dy = target.getY() - current.getY();
    int stepX = (dx > 0 ? 1 : -1) * static_cast<int>(dx != 0);
    int stepY = (dy > 0 ? 1 : -1) * static_cast<int>(dy != 0);

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
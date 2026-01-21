#include "model/cells/BombCell.hpp"
#include "interfaces/ICellInteractionVisitor.hpp"
#include "interfaces/ICellRenderVisitor.hpp"

BombCell::BombCell(): _isAvailable(true) {};

bool BombCell::isAvailable() const {
    return _isAvailable;
}

void BombCell::setAvailable(bool available) {
    _isAvailable = available;
}

void BombCell::acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor) const {
    visitor.drawBombCell(*this, pos);
}

int BombCell::acceptInteractionColission(ICellInteractionVisitor& visitor) {
    return visitor.collideWithBombCell(*this);
}

void BombCell::acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) {
    visitor.stepOnBombCell(*this, position);
}






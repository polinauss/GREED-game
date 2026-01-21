#include "model/cells/TeleportCell.hpp"
#include "interfaces/ICellInteractionVisitor.hpp"
#include "interfaces/ICellRenderVisitor.hpp"

TeleportCell::TeleportCell(Position teleportPos): _teleportPos(teleportPos), _tpAvailable(true) {}; 

void TeleportCell::setAvailable(bool available) {
    _tpAvailable = available;
}

bool TeleportCell::isAvailable() const {
    return _tpAvailable;
}

Position TeleportCell::getTPPos() const {
    return _teleportPos;
}

void TeleportCell::acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor) const {
    visitor.drawTeleportCell(*this, pos);
}

int TeleportCell::acceptInteractionColission(ICellInteractionVisitor& visitor) {
    return visitor.collideWithTeleportCell(*this);
}

void TeleportCell::acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) {
    visitor.stepOnTeleportCell(*this, position);
}

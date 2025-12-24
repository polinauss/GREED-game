#include "model/cells/BasicCell.h"
#include "interfaces/ICellInteractionVisitor.h"
#include "interfaces/ICellRenderVisitor.h"

BasicCell::BasicCell(int value, Color color): _value(value), _color(color), _isAvailable(true) {};

int BasicCell::getValue() const {
    return _value;
}

Color BasicCell::getColor() const {
    return _color;
}

bool BasicCell::isAvailable() const {
    return _isAvailable;
}

void BasicCell::setAvailable(bool available) {
    _isAvailable = available;
}

void BasicCell::acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor) const {
    visitor.drawBasicCell(*this, pos, highlightColor);
}

int BasicCell::acceptInteractionColission(ICellInteractionVisitor& visitor) {
    return visitor.collideWithBasicCell(*this);
}

void BasicCell::acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) {
    Position finalPos = visitor.stepOnBasicCell(*this, position);
    visitor.handleStepOnBasicCell(position, finalPos);
}


#include "model/cells/BasicCell.h"
#include "interfaces/ICellRenderVisitor.h"
#include "interfaces/ICellInteractionVisitor.h"

BasicCell::BasicCell(int value, Color color): 
    _value(value), 
    _color(color), 
    _available(true) {
}

int BasicCell::getValue() const {
    return _value;
}

Color BasicCell::getColor() const {
    return _color;
}

bool BasicCell::isAvailable() const {
    return _available;
}

void BasicCell::setAvailable(bool available) {
    _available = available;
}

void BasicCell::acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor) const {
    visitor.drawBasicCell(*this, pos, highlightColor);
}

bool BasicCell::acceptInteractionColission(ICellInteractionVisitor& visitor) {
    return visitor.collideWithBasicCell(*this);
}

void BasicCell::acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) {
    visitor.handleStepOnBasicCell(position, visitor.stepOnBasicCell(*this, position));
}

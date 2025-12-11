#include "model/cells/BasicCell.h"
#include "model/InteractionHandler.h"
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

bool BasicCell::acceptInteractionColission(ICellInteractionVisitor& visitor) {
    InteractionHandler& handler = dynamic_cast<InteractionHandler&>(visitor);
    int result = handler.collideWithBasicCell(*this);
    return result != 0;
}

Position BasicCell::acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) {
    InteractionHandler& handler = dynamic_cast<InteractionHandler&>(visitor);
    return handler.stepOnBasicCell(*this, position);
}

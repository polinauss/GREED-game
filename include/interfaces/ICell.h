#ifndef CELL
#define CELL

#include "core/Color.h"
#include "core/Directions.h"
#include "model/Position.h"

class ICellRenderVisitor;
class ICellInteractionVisitor;

class ICell {
public:
    virtual ~ICell() = default;

    virtual int getValue() const = 0;
    virtual Color getColor() const = 0;
    virtual bool isAvailable() const = 0;
    virtual void setAvailable(bool available) = 0;

    virtual void acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor = Color::DEFAULT) const = 0;
    virtual bool acceptInteractionColission(ICellInteractionVisitor& visitor) = 0;
    virtual void acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) = 0;
};

#endif
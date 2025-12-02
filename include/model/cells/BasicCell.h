#ifndef BASICCELL
#define BASICCELL

#include "interfaces/ICell.h"

class BasicCell: public ICell {
private:
    int _value;
    Color _color;
    bool _available;

public:
    BasicCell(int value, Color color);
    ~BasicCell() = default;

    int getValue() const override;
    Color getColor() const override;
    bool isAvailable() const override;
    void setAvailable(bool available) override;

    void acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor = Color::DEFAULT) const override;
    bool acceptInteractionColission(ICellInteractionVisitor& visitor) override;
    void acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) override;
};

#endif

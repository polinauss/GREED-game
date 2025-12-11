#ifndef BASICCELL
#define BASICCELL

#include "interfaces/ICell.h"

class BasicCell : public ICell {
private:
    int _value;
    Color _color;
    bool _isAvailable;

public:
    BasicCell(int value, Color color);
    ~BasicCell() = default;

    void acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor = Color::DEFAULT) const override;
    bool acceptInteractionColission(ICellInteractionVisitor& visitor) override;
    Position acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) override;

    Color getColor() const override;
    int getValue() const override;
    bool isAvailable() const override;
    void setAvailable(bool available) override;
};

#endif

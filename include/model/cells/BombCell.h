#ifndef BOMBCELL
#define BOMBCELL

#include "interfaces/ICell.h"

class BombCell: public ICell {
private:
    bool _isAvailable;

public:
    BombCell();
    ~BombCell() = default;

    void acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor = Color::DEFAULT) const override;
    int acceptInteractionColission(ICellInteractionVisitor& visitor) override;
    void acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) override;

    bool isAvailable() const override;
    void setAvailable(bool available) override;
};

#endif
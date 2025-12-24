#ifndef TELEPORTCELL
#define TELEPORTCELL

#include "interfaces/ICell.h"

class TeleportCell: public ICell {
private:
    Position _teleportPos;
    bool _tpAvailable;

public:
    TeleportCell(Position teleportPos);
    ~TeleportCell() = default;

    void acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor = Color::DEFAULT) const override;
    int acceptInteractionColission(ICellInteractionVisitor& visitor) override;
    void acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) override;

    bool isAvailable() const override;
    void setAvailable(bool available) override;
    Position getTPPos() const;
};


#endif
#ifndef CELLINTERACTIONVISITOR
#define CELLINTERACTIONVISITOR

class Position;
class BasicCell;
class TeleportCell;
class BombCell;

class ICellInteractionVisitor {
public:
    virtual ~ICellInteractionVisitor() = default;

    virtual int collideWithBasicCell(BasicCell& cell) = 0;
    virtual Position stepOnBasicCell(BasicCell& cell, const Position& position) = 0;
    virtual void handleStepOnBasicCell(const Position& startPos, const Position& finalPos) = 0;

    virtual int collideWithTeleportCell(TeleportCell& cell) = 0;
    virtual void stepOnTeleportCell(TeleportCell& cell, const Position& position) = 0;

    virtual int collideWithBombCell(BombCell& cell) = 0;
    virtual void stepOnBombCell(BombCell& cell, const Position& position) = 0;
};

#endif

#ifndef CELLINTERACTIONVISITOR
#define CELLINTERACTIONVISITOR

class Position;
class BasicCell;

class ICellInteractionVisitor {
public:
    virtual ~ICellInteractionVisitor() = default;

    virtual bool collideWithBasicCell(BasicCell& cell) = 0;
    virtual Position stepOnBasicCell(BasicCell& cell, const Position& position) = 0;
    virtual void handleStepOnBasicCell(const Position& startPos, const Position& finalPos) = 0;
    
};

#endif

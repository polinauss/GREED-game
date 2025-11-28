#ifndef INTERACTIONHANDLER
#define INTERACTIONHANDLER

#include "interfaces/ICellInteractionVisitor.h"
#include "model/Position.h"
#include "core/Directions.h"
#include <vector>

class GameModel;

class InteractionHandler: public ICellInteractionVisitor {
private:
    GameModel& _model;
    std::vector<Position> _prevMoveAffectedElements;

public:
    explicit InteractionHandler(GameModel& model);
    ~InteractionHandler() = default;

    int collideWithBasicCell(BasicCell& cell) override;
    Position stepOnBasicCell(BasicCell& cell, const Position& position) override;
    void handleStepOnBasicCell(const Position& startPos, const Position& finalPos) override;

    const std::vector<Position>& getAffectedElements() const;

private:
    std::vector<Position> makeOver(const Position& start, const Position& finish) const;
};

#endif
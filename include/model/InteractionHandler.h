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
    Position _lastFinalPos;

public:
    explicit InteractionHandler(GameModel& model);
    ~InteractionHandler() = default;

    int collideWithBasicCell(BasicCell& cell) override;
    Position stepOnBasicCell(BasicCell& cell, const Position& cellPos) override;
    void handleStepOnBasicCell(const Position& startPos, const Position& finalPos) override;

    int collideWithTeleportCell(TeleportCell& cell) override;
    void stepOnTeleportCell(TeleportCell& cell, const Position& cellPos) override;

    int collideWithBombCell(BombCell& cell) override;
    void stepOnBombCell(BombCell& cell, const Position& cellPos) override;

    const std::vector<Position>& getAffectedElements() const;
    Position getLastFinalPos() const { return _lastFinalPos; }

private:
    std::vector<Position> makeOver(const Position& start, const Position& finish) const;
};

#endif

#ifndef GAMEMODEL
#define GAMEMODEL

#include "model/Grid.h"
#include "model/Player.h"
#include "model/Position.h"
#include "model/InteractionHandler.h"
#include "interfaces/ICell.h"
#include "model/cells/BasicCell.h"
#include "model/cells/TeleportCell.h"
#include "model/cells/BombCell.h"
#include "core/Directions.h"
#include <iostream>
#include <vector>
#include <map>
#include <cstddef>

class GameModel {
private:
    Grid _grid;
    Player _player;
    int _score;
    bool _gameOver;
    std::vector<std::pair<bool, Position>> _availableMoves;
    InteractionHandler _interactionHandler;

public:
    GameModel(int width=25, int height=25);
    ~GameModel() = default;

    friend class InteractionHandler;

    void initializeGame();
    void initializeGameFromState(const std::vector<int>& cellValues, 
                                const std::vector<int>& cellColors,
                                const std::vector<int>& cellAvailable,
                                const Position& playerPos,
                                int score);
    bool isValidMove(Position position) const;
    void makeMove(Direction direction);
    bool isGameOver() const;
    int getScore() const;
    Grid& getGrid();
    const Grid& getGrid() const;
    Position getPlayerPosition() const;
    const std::vector<Position>& getAffectedElements() const;
    std::vector<std::pair<bool, Position>>& getAvailableMoves();

private:
    void updateGameState();
};

#endif

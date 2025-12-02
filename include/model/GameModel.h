#ifndef GAMEMODEL
#define GAMEMODEL

#include "model/Grid.h"
#include "model/Player.h"
#include "model/Position.h"
#include "model/InteractionHandler.h"
#include "interfaces/ICell.h"
#include "model/cells/BasicCell.h"
#include "core/Directions.h"
#include <iostream>
#include <vector>
#include <map>

class GameModel {
private:
    Grid _grid;
    Player _player;
    int _score;
    bool _gameOver;
    std::vector<std::pair<bool, Position>> _availableMoves;
    InteractionHandler _interactionHandler;

public:
    GameModel(int width=70, int height=25);
    ~GameModel() = default;

    friend class InteractionHandler;

    void initializeGame();
    bool isValidMove(Position position) const;
    void makeMove(Direction direction);
    bool isGameOver() const;
    int getScore() const;
    Grid& getGrid();
    const Grid& getGrid() const;
    Position getPlayerPosition() const;
    const std::vector<Position>& getAffectedElements() const;
    std::vector<std::pair<bool, Position>>& getAvailableMoves();

    void setPlayerPosition(Position position);
    void setScore(int score);

private:
    void updateGameState();
};

#endif

#ifndef GAMEMODEL
#define GAMEMODEL

#include "model/Grid.h"
#include "model/Player.h"
#include "model/Position.h"
#include "core/Directions.h"
#include <vector>
#include <map>
#include <string>

class GameModel {
private:
    Grid _grid;
    Player _player;
    int _score;
    bool _gameOver;
    bool _isPaused;
    std::map<Direction, std::vector<Position>> _availableMoves;
    std::string _playerName;

public:
    GameModel(int width=10, int height=10);
    ~GameModel() = default;

    void initializeGame();
    bool isValidMove(Position position) const;
    bool makeMove(Direction direction);
    bool isGameOver() const;
    bool isPaused() const { return _isPaused; }
    void togglePause() { _isPaused = !_isPaused; }
    int getScore() const;
    Grid& getGrid();
    const Grid& getGrid() const;
    const std::vector<Position>& getAvailableMoves(Direction direction) const;
    Position getPlayerPosition() const;
    

    bool saveGame(const std::string& filename) const;
    bool loadGame(const std::string& filename);
    void setPlayerName(const std::string& name);
    std::string getPlayerName() const;

private:
    Position calculateNewPosition(Position current, Direction direction) const;
    void updateGameState();
    std::vector<Position> makeOver(Position start, Position finish) const;
};

#endif

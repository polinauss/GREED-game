#ifndef GAMEVIEW
#define GAMEVIEW

#include "model/GameModel.h"
#include "view/ConsoleRenderer.h"
#include "model/Position.h"
#include <memory>
#include <vector>

class GameView {
private:
    GameModel* _model;
    std::unique_ptr<ConsoleRenderer> _renderer;

public:
    explicit GameView(GameModel* model);
    ~GameView() = default;

    void renderGameState(Direction moveDirection);
    void displayGameOver();
    void displayWelcomeScreen();
    void showHelp();

private:
    void renderGrid(Direction moveDirection);
    void renderScore();
    void renderGameStatus();
};

#endif
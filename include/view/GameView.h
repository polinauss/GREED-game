#ifndef GAMEVIEW
#define GAMEVIEW

#include "model/GameModel.h"
#include "model/Position.h"
#include "view/ConsoleRenderer.h"
#include "view/Settings.h"
#include <memory>
#include <vector>
#include <string>

class GameView {
private:
    GameModel* _model;
    std::unique_ptr<ConsoleRenderer> _renderer;
    std::unique_ptr<Settings> _settings;

public:
    explicit GameView(GameModel* model);
    ~GameView() = default;

    void displayOffset();

    void renderStatringState();
    void renderMove();
    void renderScore();
    void highlightMoveDirection(std::vector<std::pair<bool, Position>>& availableMoves, Direction direction);
    void highlightGameOver();
    void displayGameOver();
    void displayWelcomeScreen();
    void displayMenu(const std::vector<std::string>& menuItems, int selectedIndex);
    void showHelp();

};

#endif

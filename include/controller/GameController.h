#ifndef GAMECONTROLLER
#define GAMECONTROLLER

#include "view/GameView.h"
#include "model/GameModel.h"
#include "controller/InputHandler.h"
#include <vector>
#include <memory>
#include <string>

class GameController {
private:
    GameModel* _model;
    GameView* _view;
    std::unique_ptr<InputHandler> _inputHandler;
    bool _gameRunning;
    std::string _saveFileName;

public:
    GameController(GameModel* model, GameView* view);
    ~GameController() = default;
    
    void startGame();
    void processInput();
    void handleMove(Direction direction);
    void showMenu();
    bool loadGame();
    void saveGame();
    void setPlayerName();
    void showLeaderboard();

private:
    void handleMenuInput(const std::vector<std::string>& menuItems, int& selectedIndex);
};

#endif

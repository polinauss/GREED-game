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
    
    enum class GameState {
        MAIN_MENU,
        PLAYING,
        GAME_OVER,
        RULES,
        LEADERBOARD,
        NAME_INPUT
    };
    
    GameState _currentState;
    int _menuSelection;
    std::string _playerName;

public:
    GameController(GameModel* model, GameView* view);
    ~GameController() = default;
    
    void startGame();

private:
    void showMainMenu();
    void handleMainMenu();
    void startNewGame();
    void saveGame();
    void loadGame();
    void handlePlayingState();
    void handleGameOver();
    void handleRules();
    void showLeaderboard();
    void handleLeaderboard();
    void handleNameInput();
    void cleanup();
};

#endif

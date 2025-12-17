#ifndef GAMECONTROLLER
#define GAMECONTROLLER

#include "view/GameView.h"
#include "model/GameModel.h"
#include "controller/InputHandler.h"
#include <vector>
#include <memory>
#include <functional>
#include <cstddef> // Добавлено для size_t

class GameController {
private:
    GameModel* _model;
    GameView* _view;
    std::unique_ptr<InputHandler> _inputHandler;
    bool _paused;
    bool _shouldReturnToMenu;
    std::function<void()> _saveCallback;
    std::function<bool()> _menuCallback;

public:
    GameController(GameModel* model, GameView* view);
    ~GameController() = default;
    
    void startGame();
    void setSaveCallback(std::function<void()> callback);
    void setMenuCallback(std::function<bool()> callback);
    bool shouldReturnToMenu() const { return _shouldReturnToMenu; }
    void resetReturnToMenu() { _shouldReturnToMenu = false; }
    bool isPaused() const { return _paused; }

private:
    void clearScreen();
};

#endif

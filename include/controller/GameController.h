#ifndef GAMECONTROLLER
#define GAMECONTROLLER

#include "view/GameView.h"
#include "model/GameModel.h"
#include "controller/InputHandler.h"
#include <vector>
#include <memory>

class GameController {
private:
    GameModel* _model;
    GameView* _view;
    std::unique_ptr<InputHandler> _inputHandler;

public:
    GameController(GameModel* model, GameView* view);
    ~GameController() = default;
    
    void startGame();
    void processInput();
    void handleMove(Direction direction);
};

#endif
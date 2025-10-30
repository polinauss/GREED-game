#include "controller/GameController.h"

GameController::GameController(GameModel* model, GameView* view): _model(model), _view(view) {
    _inputHandler = std::make_unique<InputHandler>();
}

void GameController::startGame() {
    _view->displayWelcomeScreen();
    _view->renderGameState(Direction::NONE);

    while (!_model->isGameOver()) {
        processInput();
    }
    
    _view->displayGameOver();
}

void GameController::processInput() {
    Direction temp, direction;
    while(true) {
        direction = _inputHandler->getDirectionFromInput();
        if (direction == Direction::NONE) {
            handleMove(temp);
        }
        _view->renderGameState(direction);
        temp = direction;
    }
}

void GameController::handleMove(Direction direction) {
    if (direction == Direction::NONE) return;

    if (!_model->makeMove(direction)) {
        return;
    }
}
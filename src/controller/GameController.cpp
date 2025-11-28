#include "controller/GameController.h"

GameController::GameController(GameModel* model, GameView* view): _model(model), _view(view) {
    _inputHandler = std::make_unique<InputHandler>();
}

Direction GameController::processInput() {
    Direction prev = Direction::NONE, current;

    do {
        current = _inputHandler->getDirectionFromInput();
        if (current != Direction::NONE) {
            auto& moves = _model->getAvailableMoves();
            _view->highlightMoveDirection(moves, current);
            prev = current;
        }
    } while (prev == Direction::NONE || current != Direction::NONE);

    return prev;
}

void GameController::startGame() {
    _model->initializeGame();
    _view->renderStatringState();

    Direction moveDirection;
    do {
        moveDirection = processInput();
        _model->makeMove(moveDirection);
        _view->renderMove();
    } while(!_model->isGameOver());

    if (_model->isGameOver()) {
        _view->highlightGameOver();
    }
}

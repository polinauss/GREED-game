#include "controller/GameController.h"
#include <unistd.h>

GameController::GameController(GameModel* model, GameView* view): _model(model), _view(view) {
    _inputHandler = std::make_unique<InputHandler>();
}

Direction GameController::processInput() {
    Direction prev = Direction::NONE, current;
    auto& moves = _model->getAvailableMoves();

    do {
        current = _inputHandler->getDirectionFromInput();
        if (current != Direction::NONE) {
            if (prev != Direction::NONE && prev != current) {
                _view->renderMove();
            }
            
            _view->highlightMoveDirection(moves, current);
            prev = current;
        }
    } while (prev == Direction::NONE || current != Direction::NONE);

    _view->renderMove();
    
    return prev;
}

void GameController::startGame() {
    system("clear");
    
    _model->initializeGame();
    _view->renderStatringState();

    Direction moveDirection;
    do {
        moveDirection = processInput();
        if (moveDirection != Direction::NONE) {
            _model->makeMove(moveDirection);
            _view->renderMove();
        }
    } while(!_model->isGameOver());

    if (_model->isGameOver()) {
        _view->highlightGameOver();
        sleep(2);
        system("clear");
    }
}

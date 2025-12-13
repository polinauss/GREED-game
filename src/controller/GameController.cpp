#include "controller/GameController.h"
#include <unistd.h>
#include <iostream>

GameController::GameController(GameModel* model, GameView* view): _model(model), _view(view), _paused(false), _shouldReturnToMenu(false){
    _inputHandler = std::make_unique<InputHandler>();
}

void GameController::setSaveCallback(std::function<void()> callback) {
    _saveCallback = callback;
}

void GameController::setMenuCallback(std::function<bool()> callback) {
    _menuCallback = callback;
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

void GameController::showPauseMenu() {
    system("clear");
    std::cout << "\033[1;33m" << "=== GAME PAUSED ===" << "\033[0m" << std::endl << std::endl;
    std::cout << "Score: \033[1;36m" << _model->getScore() << "\033[0m" << std::endl << std::endl;
    
    std::cout << "\033[32m" << "P" << "\033[0m" << " - Resume Game" << std::endl;
    std::cout << "\033[34m" << "S" << "\033[0m" << " - Save Game" << std::endl;
    std::cout << "\033[35m" << "M" << "\033[0m" << " - Main Menu" << std::endl;
    std::cout << "\033[36m" << "ESC" << "\033[0m" << " - Exit" << std::endl << std::endl;
    
    std::cout << "\033[1;37m" << "Press key to select..." << "\033[0m";
}

void GameController::handleSpecialInput(char input) {
    switch(tolower(input)) {
        case 'p':
            _paused = !_paused;
            if (_paused) {
                showPauseMenu();
            } else {
                _view->refresh();
            }
            break;
        case 's':
            if (_saveCallback) {
                _saveCallback();
                if (_paused) {
                    showPauseMenu();
                }
            }
            break;
        case 'm':
            if (_menuCallback && _menuCallback()) {
                _shouldReturnToMenu = true;
            }
            break;
        case 27: // ESC
            if (_paused) {
                exit(0);
            }
            break;
    }
}

void GameController::startGame() {
    system("clear");
    
    _model->initializeGame();
    _view->renderStatringState();

    Direction moveDirection;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    do {
        char c;
        if (read(STDIN_FILENO, &c, 1) > 0) {
            handleSpecialInput(c);
        }
        
        if (!_paused && !_shouldReturnToMenu) {
            moveDirection = processInput();
            if (moveDirection != Direction::NONE) {
                _model->makeMove(moveDirection);
                _view->renderMove();
            }
        } else if (_paused) {
            usleep(100000);
        }
        
    } while(!_model->isGameOver() && !_shouldReturnToMenu);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    if (_model->isGameOver() && !_shouldReturnToMenu) {
        _view->highlightGameOver();
        sleep(2);
        system("clear");
    }
}

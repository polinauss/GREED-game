#include "controller/GameController.h"
#include <iostream>
#include <unistd.h>

GameController::GameController(GameModel* model, GameView* view): 
    _model(model), _view(view), _gameRunning(false), _saveFileName("savegame.dat") {
    _inputHandler = std::make_unique<InputHandler>();
}

void GameController::startGame() {
    _gameRunning = true;
    showMenu();
}

void GameController::showMenu() {
    std::vector<std::string> menuItems = {
        "New Game",
        "Load Game", 
        "Save Game",
        "Set Player Name",
        "Exit"
    };
    
    int selectedIndex = 0;
    
    while (_gameRunning) {
        _view->displayMenu(menuItems, selectedIndex);
        handleMenuInput(menuItems, selectedIndex);
    }
}

void GameController::handleMenuInput(const std::vector<std::string>& menuItems, int& selectedIndex) {
    char input = _inputHandler->getCharInput();
    
    if (input == '\033') {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) > 0 && seq[0] == '[') {
            if (read(STDIN_FILENO, &seq[1], 1) > 0) {
                if (seq[1] == 'A') {
                    selectedIndex = (selectedIndex - 1 + menuItems.size()) % menuItems.size();
                } else if (seq[1] == 'B') {
                    selectedIndex = (selectedIndex + 1) % menuItems.size();
                }
            }
        }
    } else if (input == '\n') {
        switch (selectedIndex) {
            case 0:
                _model->initializeGame();
                _view->renderGameState(Direction::NONE);
                processInput();
                break;
            case 1:
                if (loadGame()) {
                    _view->renderGameState(Direction::NONE);
                    processInput();
                }
                break;
            case 2:
                saveGame();
                sleep(2);
                break;
            case 3:
                setPlayerName();
                break;
            case 4:
                _gameRunning = false;
                break;
        }
    }
}

void GameController::setPlayerName() {
    std::cout << "Enter your name: ";
    _inputHandler->disableCanonicalMode();
    std::string name;
    std::getline(std::cin, name);
    _inputHandler->enableCanonicalMode();
    
    if (!name.empty()) {
        _model->setPlayerName(name);
        std::cout << "Hello, " << name << "!" << std::endl;
        sleep(2);
    }
}

void GameController::saveGame() {
    if (_model->saveGame(_saveFileName)) {
        std::cout << "Game saved successfully!" << std::endl;
    } else {
        std::cout << "Error saving game!" << std::endl;
    }
}

bool GameController::loadGame() {
    if (_model->loadGame(_saveFileName)) {
        std::cout << "Game loaded successfully!" << std::endl;
        std::cout << "Welcome back, " << _model->getPlayerName() << "!" << std::endl;
        sleep(2);
        return true;
    } else {
        std::cout << "No saved game found!" << std::endl;
        sleep(2);
        return false;
    }
}

void GameController::processInput() {
    Direction previewDirection = Direction::NONE;
    
    while (!_model->isGameOver() && _gameRunning) {
        if (!_model->isPaused()) {
            _view->renderGameState(previewDirection);
            
            char input = _inputHandler->getCharInput();
            
            if (input == '\033') {
                char seq[2];
                if (read(STDIN_FILENO, &seq[0], 1) > 0 && seq[0] == '[') {
                    if (read(STDIN_FILENO, &seq[1], 1) > 0) {
                        switch(seq[1]) {
                            case 'A': previewDirection = Direction::UP; break;
                            case 'B': previewDirection = Direction::DOWN; break;
                            case 'C': previewDirection = Direction::RIGHT; break;
                            case 'D': previewDirection = Direction::LEFT; break;
                        }
                    }
                }
            } 
            else if (input == '\n') {
                if (previewDirection != Direction::NONE) {
                    handleMove(previewDirection);
                    previewDirection = Direction::NONE;
                    
                    saveGame();
                }
            }
            else if (input == ' ') {
                _model->togglePause();
            }
            else if (input == 'q' || input == 'Q') {

                std::cout << "Saving game and returning to menu..." << std::endl;
                saveGame();
                sleep(1);
                break;
            }
            else {
                switch(input) {
                    case 'w': case 'W': previewDirection = Direction::UP; break;
                    case 's': case 'S': previewDirection = Direction::DOWN; break;
                    case 'a': case 'A': previewDirection = Direction::LEFT; break;
                    case 'd': case 'D': previewDirection = Direction::RIGHT; break;
                }
            }
        } else {
            char input = _inputHandler->getCharInput();
            if (input == ' ') {
                _model->togglePause();
            }
        }
        
        if (_model->isGameOver()) {
            _view->displayGameOver();
            
            saveGame();
            sleep(3);
            break;
        }
    }
}

void GameController::handleMove(Direction direction) {
    if (!_model->makeMove(direction)) {
        std::cout << "Invalid move!" << std::endl;
        sleep(1);
    }
}

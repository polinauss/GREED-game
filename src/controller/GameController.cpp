#include "controller/GameController.h"
#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <fstream>

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
        "Leaderboard",
        "Force Update Leaderboard",
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
                _model->resetGame();
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
                showLeaderboard();
                break;
            case 5:
                std::cout << "Forcing leaderboard update..." << std::endl;
                _model->updatePlayerScore();
                sleep(2);
                break;
            case 6:
                _gameRunning = false;
                break;
        }
    }
}

void GameController::showLeaderboard() {
    std::cout << "=== SHOWING LEADERBOARD ===" << std::endl;
    
    _model->loadLeaderboard();
    auto leaderboard = _model->getLeaderboard();
    
    _inputHandler->disableCanonicalMode();
    
    std::cout << "\033[2J\033[1;1H";
    std::cout << "\033[1;36m" << "=== LEADERBOARD ===" << "\033[0m" << std::endl;
    std::cout << std::endl;
    
    if (leaderboard.empty()) {
        std::cout << "No records yet! Be the first to play!" << std::endl;
        std::cout << "DEBUG: leaderboard vector size = " << leaderboard.size() << std::endl;
        
        std::ifstream file("leaderboard.dat");
        if (file.is_open()) {
            std::cout << "leaderboard.dat exists but is empty or cannot be read" << std::endl;
            file.close();
        } else {
            std::cout << "leaderboard.dat does not exist" << std::endl;
        }
    } else {
        std::cout << std::setw(4) << "Rank" << std::setw(20) << "Player" << std::setw(10) << "Score" << std::endl;
        std::cout << std::string(34, '-') << std::endl;
        
        for (size_t i = 0; i < leaderboard.size() && i < 10; ++i) {
            const auto& record = leaderboard[i];
            std::cout << std::setw(4) << (i + 1) 
                      << std::setw(20) << record.name 
                      << std::setw(10) << record.bestScore << std::endl;
        }
    }
    
    std::cout << std::endl;
    std::cout << "Press any key to return to menu..." << std::endl;
    
    _inputHandler->getCharInput();
    _inputHandler->enableCanonicalMode();
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
                        
                    if (_model->isGameOver()) {
                        std::cout << "=== GAME OVER AFTER MOVE ===" << std::endl;
                        _view->displayGameOver();
                        _model->updatePlayerScore();
                        sleep(3);
                        break;
                    }
                }
            }
            else if (input == ' ') {
                _model->togglePause();
            }
            else if (input == 'q' || input == 'Q') {

                std::cout << "Quitting to menu..." << std::endl;
                saveGame();
                std::cout << "Updating leaderboard before quit..." << std::endl;
                _model->updatePlayerScore();
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
            std::cout << "=== GAME OVER DETECTED IN MAIN LOOP ===" << std::endl;
            _view->displayGameOver();
            
            std::cout << "Final score: " << _model->getScore() << std::endl;
            
            _model->updatePlayerScore();
            
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
        return;
    }
    std::cout << "Move successful. Current score: " << _model->getScore() << std::endl;
}

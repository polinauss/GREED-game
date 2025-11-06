#include "view/GameView.h"
#include "model/GameModel.h"
#include "core/Directions.h"
#include <iostream>
#include <iomanip>

GameView::GameView(GameModel* model): _model(model) {
    _renderer = std::make_unique<ConsoleRenderer>();
}

void GameView::renderGrid(Direction moveDirection) {
    _renderer->drawGrid(
        _model->getGrid(),
        _model->getPlayerPosition(),
        _model->getAvailableMoves(moveDirection)
    );
}

void GameView::renderScore() {
    _renderer->drawScore(_model->getScore());
}

void GameView::displayGameOver() {
    _renderer->displayGameOver();
}

void GameView::displayWelcomeScreen() {
    _renderer->clearScreen();
    _renderer->displayWelcomeScreen();
}

void GameView::displayMenu(const std::vector<std::string>& menuItems, int selectedIndex) {
    _renderer->displayMenu(menuItems, selectedIndex, _model->getPlayerName());
}

void GameView::showHelp() {}

void GameView::renderGameState(Direction moveDirection) {
    _renderer->clearScreen();
    
    std::cout << "Player: " << _model->getPlayerName();
    if (_model->isPaused()) {
        std::cout << " | *** PAUSED ***";
    }
    std::cout << std::endl;
    
    renderScore();
    renderGrid(moveDirection);

    if (moveDirection != Direction::NONE) {
        std::string directionStr;
        switch (moveDirection) {
            case Direction::UP: directionStr = "UP"; break;
            case Direction::DOWN: directionStr = "DOWN"; break;
            case Direction::LEFT: directionStr = "LEFT"; break;
            case Direction::RIGHT: directionStr = "RIGHT"; break;
            default: directionStr = "NONE";
        }
        std::cout << "Previewing: " << directionStr << " - Press ENTER to move" << std::endl;
    } else {
        std::cout << "Select direction with WASD/Arrows" << std::endl;
    }
    
    std::cout << "Controls: WASD/Arrows - Select direction, ENTER - Move, SPACE - Pause, Q - Menu" << std::endl;
}

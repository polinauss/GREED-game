#include "view/GameView.h"
#include "model/GameModel.h"
#include "core/Directions.h"
#include <iostream>
#include <iomanip>
#include <signal.h>

GameView::GameView(GameModel* model): _model(model) {
    _settings = std::make_unique<Settings>();
    _renderer = std::make_unique<ConsoleRenderer>(_settings->calculateCenteringOffsets(
        _model->getGrid().getWidth(),
        _model->getGrid().getHeight()
    ));
}

void GameView::updateRenderer() {
    Position offset = _settings->calculateCenteringOffsets(
        _model->getGrid().getWidth(),
        _model->getGrid().getHeight()
    );
    _renderer = std::make_unique<ConsoleRenderer>(offset);
}

void GameView::renderStatringState() {
    _renderer->clearScreen();
    _renderer->drawStartingState(_model->getGrid());
    _renderer->drawPlayer(_model->getPlayerPosition());
    renderScore();
}

void GameView::renderMove() {
    _renderer->drawMove(_model->getGrid(), _model->getAffectedElements());
    _renderer->drawPlayer(_model->getPlayerPosition());
    renderScore();
}

void GameView::highlightMoveDirection(std::vector<std::pair<bool, Position>>& availableMoves, Direction direction) {
    _renderer->highlightMoveDirection(_model->getGrid(), availableMoves, direction);
    renderScore();
}

void GameView::highlightGameOver() {
    _renderer->highlightGameOverState(_model->getGrid());
    _renderer->drawPlayer(_model->getPlayerPosition());
    
    int gridWidth = _model->getGrid().getWidth() * 2;
    Position fieldOffset = _settings->calculateCenteringOffsets(
        _model->getGrid().getWidth(),
        _model->getGrid().getHeight()
    );
    
    int scoreX = fieldOffset.getX() + (gridWidth / 2) - 3;
    int scoreY = fieldOffset.getY() - 2;
    
    Position scorePos(scoreX, scoreY);
    _renderer->drawScoreAtPosition(_model->getScore(), scorePos);
}

void GameView::renderScore() {
    int gridWidth = _model->getGrid().getWidth() * 2;
    Position fieldOffset = _settings->calculateCenteringOffsets(
        _model->getGrid().getWidth(),
        _model->getGrid().getHeight()
    );
    
    int scoreX = fieldOffset.getX() + (gridWidth / 2) - 3;
    int scoreY = fieldOffset.getY() - 2;
    
    Position scorePos(scoreX, scoreY);
    _renderer->drawScoreAtPosition(_model->getScore(), scorePos);
}

void GameView::displayGameOver() {
    _renderer->displayGameOver();
}

void GameView::displayWelcomeScreen() {
    _renderer->clearScreen();
    _renderer->displayWelcomeScreen();
}

void GameView::displayMenu(const std::vector<std::string>& menuItems, int selectedIndex) {}

void GameView::refresh() {
    try {
        _settings->updateTerminalSize();
        updateRenderer();
        _renderer->clearScreen(); 
        renderStatringState();
        renderMove();
        renderScore();
    } catch (const std::exception& e) {
        _renderer->clearScreen();
        std::cout << "\033[31mTerminal too small! Please resize.\033[0m" << std::endl;
    }
}

void GameView::showHelp() {}

#include "view/GameView.h"
#include "model/GameModel.h"
#include "core/Directions.h"
#include <iostream>
#include <iomanip>

GameView::GameView(GameModel* model): _model(model) {
    _settings = std::make_unique<Settings>();
    _renderer = std::make_unique<ConsoleRenderer>(_settings->calculateCenteringOffsets(
        _model->getGrid().getWidth(),
        _model->getGrid().getHeight()
    ));
}

void GameView::renderStatringState() {
    _renderer->drawStartingState(_model->getGrid());
    _renderer->drawPlayer(_model->getPlayerPosition());
    _renderer->drawScore(_model->getScore());
}

void GameView::renderMove() {
    _renderer->drawMove(_model->getGrid(), _model->getAffectedElements());
    _renderer->drawPlayer(_model->getPlayerPosition());
    _renderer->drawScore(_model->getScore());
}

void GameView::highlightMoveDirection(std::vector<std::pair<bool, Position>>& availableMoves, Direction direction) {
    _renderer->highlightMoveDirection(_model->getGrid(), availableMoves, direction);
}

void GameView::highlightGameOver() {
    _renderer->highlightGameOverState(_model->getGrid());
    _renderer->drawPlayer(_model->getPlayerPosition());
    _renderer->resetCursor();
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


void GameView::clearScreen() {
    _renderer->clearScreen();
}

void GameView::displayMenu(const std::vector<std::string>& menuItems, int selectedIndex, const std::string& playerName) {
    _renderer->displayMenu(menuItems, selectedIndex, playerName);
}

void GameView::showHelp() {
    _renderer->showRules();
}

void GameView::showLeaderboard(const std::vector<std::pair<std::string, int>>& leaders) {
    _renderer->showLeaderboard(leaders);
}

void GameView::showNameInput() {
    _renderer->showNameInput();
}

void GameView::displayOffset() {
    
}

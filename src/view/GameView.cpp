#include "view/GameView.h"
#include "model/GameModel.h"
#include "core/Directions.h"
#include <iostream>
#include <iomanip>

GameView::GameView(GameModel* model): _model(model) {
    _renderer = std::make_unique<ConsoleRenderer>();
}

// отображение игрового поля
void GameView::renderGrid(Direction moveDirection) {
    _renderer->drawGrid(
        _model->getGrid(),
        _model->getPlayerPosition(),
        _model->getAvailableMoves(moveDirection)
    );
}

// отображение счёта
void GameView::renderScore() {
    _renderer->drawScore(_model->getScore());
}

// отображение финального состояния и очситка терминала
void GameView::displayGameOver() {
    _renderer->displayGameOver();
    _renderer->clearScreen();
}

// отображение начального состояния и установка начальных параметров терминала
void GameView::displayWelcomeScreen() {
    _renderer->clearScreen();
    _renderer->displayWelcomeScreen();
}

void showHelp() {}

// отображение промежуточного игрового состояния
void GameView::renderGameState(Direction moveDirection) {
    _renderer->clearScreen();
    renderScore();
    renderGrid(moveDirection);
}


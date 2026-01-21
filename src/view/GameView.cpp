#include "view/GameView.hpp"
#include "model/GameModel.hpp"
#include "core/Directions.hpp"
#include <iostream>
#include <iomanip>
#include <signal.h>
#include <chrono>
#include <sys/ioctl.h>
#include <unistd.h>

static GameView* globalGameView = nullptr;

void handleResize(int sig) {
    if (globalGameView && sig == SIGWINCH) {
        globalGameView->refresh();
    }
}

GameView::GameView(GameModel* model): _model(model) {
    _settings = std::make_unique<Settings>();
    
    Position offset = _settings->calculateCenteringOffsets(
        _model->getGrid().getWidth(),
        _model->getGrid().getHeight()
    );
    
    _renderer = std::make_unique<ConsoleRenderer>(offset);
    
    std::cout << "\033[?7l";
    std::cout << "\033[?1049h";
    
    struct sigaction sa;
    sa.sa_handler = handleResize;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, nullptr);
    
    globalGameView = this;
}

GameView::~GameView() {
    std::cout << "\033[?7h";
    std::cout << "\033[?1049l";
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

void GameView::highlightGameOver() {
    _renderer->highlightGameOverState(_model->getGrid());
    _renderer->drawPlayer(_model->getPlayerPosition());
    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminalWidth = w.ws_col;
    
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
struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminalWidth = w.ws_col;
    
    int gridWidth = _model->getGrid().getWidth() * 2;
    Position fieldOffset = _settings->calculateCenteringOffsets(
        _model->getGrid().getWidth(),
        _model->getGrid().getHeight()
    );
    
    int scoreX = fieldOffset.getX() + (gridWidth / 2) - 4;
    int scoreY = fieldOffset.getY() - 2;
    if (scoreY < 0) scoreY = 0;
    
    Position scorePos(scoreX, scoreY);
    
    _renderer->drawScoreAtPosition(_model->getScore(), scorePos);

}

void GameView::highlightMoveDirection(std::vector<std::pair<bool, Position>>& availableMoves, Direction direction) {
    _renderer->highlightMoveDirection(_model->getGrid(), availableMoves, direction);
//    renderScore();
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
    std::cout << "\033[2J\033[1;1H";
    
    try {
        _settings->updateTerminalSize();
        updateRenderer();
        
        _renderer->drawStartingState(_model->getGrid());
        _renderer->drawPlayer(_model->getPlayerPosition());
        
        renderScore();
        
    } catch (...) {
        system("clear");
        std::cout << "Terminal size too small! Please resize." << std::endl;
    }
    
    std::cout.flush();
}
//void GameView::showHelp() {}

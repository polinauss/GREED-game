#include "controller/GameController.h"
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <sys/ioctl.h>
#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <csignal>

GameController::GameController(GameModel* model, GameView* view): _model(model), _view(view), _paused(false), _shouldReturnToMenu(false){
    _inputHandler = std::make_unique<InputHandler>();
}

void GameController::setSaveCallback(std::function<void()> callback) {
    _saveCallback = callback;
}

void GameController::setMenuCallback(std::function<bool()> callback) {
    _menuCallback = callback;
}

void GameController::startGame() {

    std::signal(SIGINT, [](int sig) {
        std::cout << "\033[?1049l";
        system("clear");
        std::cout << "\033[1;36m" << "Game interrupted. Goodbye!" << "\033[0m" << std::endl;
        std::exit(0);
    });
    
    std::cout << "\033[?1049h\033[2J\033[1;1H";
    std::cout.flush();
    
    _model->initializeGame();
    _view->renderStatringState();

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 0;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    bool waitingForSpace = false;
    Direction currentDirection = Direction::NONE;
    
    while (!_model->isGameOver() && !_shouldReturnToMenu) {
        char input;
        int bytesRead = read(STDIN_FILENO, &input, 1);
        
        if (bytesRead > 0) {
            if (_paused) {
                if (tolower(input) == 'p') {
                    _paused = false;
                    _view->refresh();
                }
                else if (tolower(input) == 'm') {
                    _shouldReturnToMenu = true;
                }
                else if (tolower(input) == 's') {
                    if (_saveCallback) _saveCallback();
                }
                else if (input == 27) {
                    char next;
                    if (read(STDIN_FILENO, &next, 1) > 0) {
                        if (next == '[') {
                            char arrow;
                            read(STDIN_FILENO, &arrow, 1);
                        } else {
                            _shouldReturnToMenu = true;
                        }
                    } else {
                        _shouldReturnToMenu = true;
                    }
                }
            } else {
                if (waitingForSpace) {
                    Direction newDir = Direction::NONE;
                    
                    if (input == 27) {
                        char next;
                        if (read(STDIN_FILENO, &next, 1) > 0) {
                            if (next == '[') {
                                char arrow;
                                if (read(STDIN_FILENO, &arrow, 1) > 0) {
                                    switch(arrow) {
                                        case 'A': newDir = Direction::UP; break;
                                        case 'B': newDir = Direction::DOWN; break;
                                        case 'C': newDir = Direction::RIGHT; break;
                                        case 'D': newDir = Direction::LEFT; break;
                                    }
                                }
                            }
                        }
                    }
                    else if (input == 'w' || input == 'W') {
                        newDir = Direction::UP;
                    }
                    else if (input == 'a' || input == 'A') {
                        newDir = Direction::LEFT;
                    }
                    else if (input == 's' || input == 'S') {
                        newDir = Direction::DOWN;
                    }
                    else if (input == 'd' || input == 'D') {
                        newDir = Direction::RIGHT;
                    }
                    
                    if (newDir != Direction::NONE) {
                        currentDirection = newDir;
                        auto& availableMoves = _model->getAvailableMoves();
                        _view->highlightMoveDirection(availableMoves, currentDirection);
                    }
                    else if (input == ' ') {
                        _model->makeMove(currentDirection);
                        _view->renderMove();
                        waitingForSpace = false;
                        currentDirection = Direction::NONE;
                    }
                    else if (input == 27) { 
                        _view->refresh();
                        waitingForSpace = false;
                        currentDirection = Direction::NONE;
                    }
                    else if (tolower(input) == 'p') {
                        _paused = true;
                        waitingForSpace = false;
                        currentDirection = Direction::NONE;
                    }
                    else if (tolower(input) == 's') {
                        if (_saveCallback) _saveCallback();
                
                    }
                    else if (tolower(input) == 'm') {
                        _shouldReturnToMenu = true;
                        waitingForSpace = false;
                        currentDirection = Direction::NONE;
                    }
                } else {
                    Direction arrowDir = Direction::NONE;
                    
                    if (input == 27) {
                        char next;
                        if (read(STDIN_FILENO, &next, 1) > 0) {
                            if (next == '[') {
                                char arrow;
                                if (read(STDIN_FILENO, &arrow, 1) > 0) {
                                    switch(arrow) {
                                        case 'A': arrowDir = Direction::UP; break;
                                        case 'B': arrowDir = Direction::DOWN; break;
                                        case 'C': arrowDir = Direction::RIGHT; break;
                                        case 'D': arrowDir = Direction::LEFT; break;
                                    }
                                }
                            } else {
                                if (_menuCallback && _menuCallback()) {
                                    _shouldReturnToMenu = true;
                                }
                            }
                        } else {
                            if (_menuCallback && _menuCallback()) {
                                _shouldReturnToMenu = true;
                            }
                        }
                    }
                    else if (tolower(input) == 'p') {
                        _paused = true;
                    }
                    else if (tolower(input) == 's') {
                        if (_saveCallback) _saveCallback();
                    }
                    else if (tolower(input) == 'm') {
                        if (_menuCallback && _menuCallback()) {
                            _shouldReturnToMenu = true;
                        }
                    }
                    else if (input == 'w' || input == 'W' || input == 'a' || input == 'A' || 
                             input == 's' || input == 'S' || input == 'd' || input == 'D') {
                        
                        switch(tolower(input)) {
                            case 'w': arrowDir = Direction::UP; break;
                            case 'a': arrowDir = Direction::LEFT; break;
                            case 's': arrowDir = Direction::DOWN; break;
                            case 'd': arrowDir = Direction::RIGHT; break;
                        }
                    }
                    
                    if (arrowDir != Direction::NONE) {
                        currentDirection = arrowDir;
                        auto& availableMoves = _model->getAvailableMoves();
                        _view->highlightMoveDirection(availableMoves, currentDirection);
                        waitingForSpace = true;
                    }
                }
            }
        }
        
        if (_paused && !waitingForSpace) {
            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            int terminalWidth = w.ws_col;
            int terminalHeight = w.ws_row;
            
            std::cout << "\033[2J\033[1;1H";
            
            int verticalPadding = terminalHeight / 4;
            
            int pauseWidth = 40;
            int pauseHeight = 13;
            
            int horizontalPadding = (terminalWidth - pauseWidth) / 2;
            if (horizontalPadding < 0) horizontalPadding = 0;
            
            int score = _model->getScore();
            std::string scoreColorCode;
            if (score >= 0 && score <= 100) {
                scoreColorCode = "\033[1;31m";
            } else if (score >= 101 && score <= 200) {
                scoreColorCode = "\033[1;33m";
            } else {
                scoreColorCode = "\033[1;32m";
            }
            
            for (int y = 0; y < pauseHeight; y++) {
                std::cout << "\033[" << (verticalPadding + y) << ";" << horizontalPadding << "H";
                
                for (int x = 0; x < pauseWidth; x++) {
                    if (y == 0 || y == pauseHeight - 1 || x == 0 || x == pauseWidth - 1) {
                        std::cout << "\033[40m \033[0m";
                    } else {
                        std::cout << " ";
                    }
                }
            }
            
            std::string title = "GAME PAUSED";
            int titleX = horizontalPadding + (pauseWidth - title.length()) / 2;
            int titleY = verticalPadding + 2;
            std::cout << "\033[" << titleY << ";" << titleX << "H";
            std::cout << "\033[1;35m" << title << "\033[0m";
            
            std::string scoreText = "Score: " + std::to_string(score);
            int scoreX = horizontalPadding + (pauseWidth - scoreText.length()) / 2;
            int scoreY = verticalPadding + 4;
            std::cout << "\033[" << scoreY << ";" << scoreX << "H";
            std::cout << scoreColorCode << scoreText << "\033[0m";
            
            std::vector<std::pair<std::string, std::string>> buttons = {
                {"P", "Resume Game"},
                {"S", "Save Game"}, 
                {"M", "Main Menu"},
                {"ESC", "Exit"}
            };
            
            std::vector<std::string> buttonColors = {
                "\033[1;32m",
                "\033[1;34m", 
                "\033[1;35m",
                "\033[1;36m"
            };
            
            int buttonStartY = verticalPadding + 6;
            for (size_t i = 0; i < buttons.size(); i++) {
                std::string buttonText = buttons[i].first + " - " + buttons[i].second;
                int buttonX = horizontalPadding + (pauseWidth - buttonText.length()) / 2;
                int buttonY = buttonStartY + i;
                
                std::cout << "\033[" << buttonY << ";" << buttonX << "H";
                std::cout << buttonColors[i] << buttonText << "\033[0m";
            }
            
            std::string hint = "Press key to select...";
            int hintX = horizontalPadding + (pauseWidth - hint.length()) / 2;
            int hintY = verticalPadding + 11;
            std::cout << "\033[" << hintY << ";" << hintX << "H";
            std::cout << "\033[3;90m" << hint << "\033[0m";
            
            std::cout.flush();
        }
        
        usleep(10000);
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::signal(SIGINT, SIG_DFL);
    std::cout << "\033[?1049l";
    
    if (_model->isGameOver() && !_shouldReturnToMenu) {
        _view->highlightGameOver();
        sleep(2);
    }
}

void GameController::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
    std::cout.flush();
}

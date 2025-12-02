#include "controller/GameController.h"
#include "model/Leaderboard.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <cstdlib>

static Leaderboard g_leaderboard;

GameController::GameController(GameModel* model, GameView* view): 
    _model(model), 
    _view(view),
    _inputHandler(std::make_unique<InputHandler>()),
    _currentState(GameState::MAIN_MENU),
    _menuSelection(0),
    _playerName("Игрок") {
}

void GameController::startGame() {
    system("clear");
    showMainMenu();
    
    while (true) {
        switch (_currentState) {
            case GameState::MAIN_MENU:
                handleMainMenu();
                break;
            case GameState::PLAYING:
                handlePlayingState();
                break;
            case GameState::GAME_OVER:
                handleGameOver();
                break;
            case GameState::RULES:
                handleRules();
                break;
            case GameState::LEADERBOARD:
                handleLeaderboard();
                break;
            case GameState::NAME_INPUT:
                handleNameInput();
                break;
        }
    }
}

void GameController::showMainMenu() {
    std::vector<std::string> menuItems = {
        "1. НАЧАТЬ НОВУЮ ИГРУ",
        "2. ЗАГРУЗИТЬ ИГРУ", 
        "3. ПРАВИЛА ИГРЫ",
        "4. ТАБЛИЦА ЛИДЕРОВ",
        "5. СМЕНИТЬ ИМЯ",
        "6. ВЫХОД"
    };
    _view->displayMenu(menuItems, 0, _playerName);
}

void GameController::handleMainMenu() {
    char input = _inputHandler->getMenuInput();
    
    switch (input) {
        case '1':
            startNewGame();
            break;
        case '2':
            loadGame();
            break;
        case '3':
            _currentState = GameState::RULES;
            _view->showHelp();
            break;
        case '4':
            _currentState = GameState::LEADERBOARD;
            showLeaderboard();
            break;
        case '5':
            _currentState = GameState::NAME_INPUT;
            _view->showNameInput();
            break;
        case '6':
        case 'q':
        case 'Q':
            exit(0);
            break;
        default:
            showMainMenu();
            break;
    }
}

void GameController::startNewGame() {
    _model = new GameModel();
    _view = new GameView(_model);
    
    _currentState = GameState::PLAYING;
    system("clear");
    _view->renderStatringState();
}
void GameController::saveGame() {
    if (!_model) return;
    
    std::ofstream saveFile("savegame.dat");
    if (saveFile.is_open()) {
        saveFile << _playerName << std::endl;
        saveFile << _model->getScore() << std::endl;
        Position playerPos = _model->getPlayerPosition();
        saveFile << playerPos.getX() << " " << playerPos.getY() << std::endl;
        saveFile.close();
    }
}

void GameController::loadGame() {
    std::ifstream saveFile("savegame.dat");
    if (!saveFile.is_open()) {
        startNewGame();
        return;
    }
    
    std::string loadedName;
    int loadedScore;
    int playerX, playerY;
    
    saveFile >> loadedName;
    saveFile >> loadedScore;
    saveFile >> playerX >> playerY;
    
    saveFile.close();
    
    startNewGame();
    
    if (!loadedName.empty()) {
        _playerName = loadedName;
    }
    
    if (_model) {
        _model->setScore(loadedScore);
        _model->setPlayerPosition(Position(playerX, playerY));
        _model->getGrid().removeCell(Position(playerX, playerY));
    }
    
    system("clear");
    _view->renderStatringState();
}

void GameController::handlePlayingState() {
    Direction dir = _inputHandler->getDirectionFromInput();
    
    if (dir == Direction::NONE) {
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~ICANON;
        newt.c_lflag &= ~ECHO;
        newt.c_cc[VMIN] = 0;
        newt.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        char c;
        if (read(STDIN_FILENO, &c, 1) > 0) {
            if (c == 'p' || c == 'P' || c == 'm' || c == 'M') {
                saveGame();
                _currentState = GameState::MAIN_MENU;
                system("clear");
                showMainMenu();
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return;
            } else if (c == 's' || c == 'S') {
                saveGame();
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                return;
            } else if (c == 'q' || c == 'Q') {
                saveGame();
                exit(0);
            }
        }
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
    
    if (dir != Direction::NONE && _model) {
        _model->makeMove(dir);
        
        if (_model->isGameOver()) {
            g_leaderboard.addScore(_playerName, _model->getScore());
            _currentState = GameState::GAME_OVER;
            _view->highlightGameOver();
            sleep(2);
            _view->displayGameOver();
        } else {
            _view->renderMove();
            _view->renderScore();
        }
    }
}

void GameController::handleGameOver() {
    _inputHandler->setupMenuMode();
    char input = _inputHandler->getAnyKey();
    _currentState = GameState::MAIN_MENU;
    system("clear");
    showMainMenu();
}

void GameController::handleRules() {
    _inputHandler->setupMenuMode();
    char input = _inputHandler->getAnyKey();
    _currentState = GameState::MAIN_MENU;
    system("clear");
    showMainMenu();
}

void GameController::showLeaderboard() {
    auto leaders = g_leaderboard.getTopScores(10);
    _view->showLeaderboard(leaders);
}

void GameController::handleLeaderboard() {
    _inputHandler->setupMenuMode();
    char input = _inputHandler->getAnyKey();
    _currentState = GameState::MAIN_MENU;
    system("clear");
    showMainMenu();
}

void GameController::handleNameInput() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    std::cout << "\033[?25h";
    
    system("clear");
    
    std::cout << "\033[10;35HВведите ваше имя (без пробелов): ";
    std::cout.flush();
    
    std::string newName;
    std::cin >> newName;
    
    if (!newName.empty()) {
        _playerName = newName;
    }
    
    std::cout << "\033[?25l";
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    _currentState = GameState::MAIN_MENU;
    system("clear");
    showMainMenu();
}

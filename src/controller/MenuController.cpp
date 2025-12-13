#include "controller/MenuController.h"
#include "controller/InputHandler.h"
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <sys/ioctl.h>
#include <termios.h>

void GameState::serialize(std::ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&playerPosition), sizeof(Position));
    file.write(reinterpret_cast<const char*>(&score), sizeof(int));
    file.write(reinterpret_cast<const char*>(&width), sizeof(int));
    file.write(reinterpret_cast<const char*>(&height), sizeof(int));
    
    size_t size = cellValues.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(cellValues.data()), size * sizeof(int));
    
    size = cellColors.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(cellColors.data()), size * sizeof(int));
    
    size = cellAvailable.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    file.write(reinterpret_cast<const char*>(cellAvailable.data()), size * sizeof(int));
}

void GameState::deserialize(std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&playerPosition), sizeof(Position));
    file.read(reinterpret_cast<char*>(&score), sizeof(int));
    file.read(reinterpret_cast<char*>(&width), sizeof(int));
    file.read(reinterpret_cast<char*>(&height), sizeof(int));
    
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    cellValues.resize(size);
    file.read(reinterpret_cast<char*>(cellValues.data()), size * sizeof(int));
    
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    cellColors.resize(size);
    file.read(reinterpret_cast<char*>(cellColors.data()), size * sizeof(int));
    
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    cellAvailable.resize(size);
    file.read(reinterpret_cast<char*>(cellAvailable.data()), size * sizeof(int));
}

MenuController::MenuController() : _playerName("Player"), _hasSavedGame(false) {
    loadLeaderboard();
    
    std::ifstream file(SAVE_FILE, std::ios::binary);
    _hasSavedGame = file.good();
    file.close();
}

void MenuController::drawAsciiTitle() {
    system("clear");
    
    std::cout << "\033[1;35m";
    
    std::cout << R"(
 ######   ########  ########  ########  ######  
##    ##  ##     #  ##        ##        ##    ##
##        #######   #######   #######   ##    ##
##        ####      ##        ##        ##    ##
##  ####  ##  ##    ##        ##        ##    ##
 ##### #  ##   ###  #######   ########  ######  
    )" << std::endl;
    
    std::cout << "\033[1;36m" << "A Colorful Number Jumping Adventure!" << "\033[0m" << std::endl << std::endl;
}

void MenuController::showWelcomeScreen() {
    system("clear");
    drawAsciiTitle();
    
    std::cout << "\033[1;33m" << "Controls:" << "\033[0m" << std::endl;
    std::cout << "\033[32m" << "W/↑ A/← S/↓ D/→" << "\033[0m" << " - Move" << std::endl;
    std::cout << "\033[33m" << "P" << "\033[0m" << " - Pause Game" << std::endl;
    std::cout << "\033[34m" << "S" << "\033[0m" << " - Save Game" << std::endl;
    std::cout << "\033[35m" << "M" << "\033[0m" << " - Menu" << std::endl;
    std::cout << "\033[36m" << "ESC" << "\033[0m" << " - Exit" << std::endl << std::endl;
    
    std::cout << "\033[1;37m" << "Press any key to continue..." << "\033[0m";
    
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    char input;
    read(STDIN_FILENO, &input, 1);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void MenuController::displayMenuItems(const std::vector<std::string>& items, int selectedIndex) {
    system("clear");
    drawAsciiTitle();
    
    std::cout << "\033[1;37m" << "Player: \033[1;33m" << _playerName << "\033[0m" << std::endl << std::endl;
    
    for (size_t i = 0; i < items.size(); i++) {
        if (i == selectedIndex) {
            std::cout << "\033[1;32m" << "  > " << items[i] << " <" << "\033[0m" << std::endl;
        } else {
            std::cout << "\033[37m" << "    " << items[i] << "\033[0m" << std::endl;
        }
    }
    
    std::cout << std::endl << "\033[3;90m" << "Use UP/DOWN arrows to navigate, ENTER to select" << "\033[0m" << std::endl;
}

void MenuController::setPlayerName() {
    system("clear");
    std::cout << "\033[1;36m" << "=== SET PLAYER NAME ===" << "\033[0m" << std::endl << std::endl;
    std::cout << "Current name: \033[1;33m" << _playerName << "\033[0m" << std::endl;
    std::cout << "Enter new name: ";
    
    std::string newName;
    std::getline(std::cin, newName);
    
    if (!newName.empty()) {
        _playerName = newName;
        std::cout << "\033[1;32m" << "Name changed to: " << _playerName << "\033[0m" << std::endl;
    }
    
    sleep(1);
}

void MenuController::showRules() {
    system("clear");
    std::cout << "\033[1;36m" << "=== GAME RULES ===" << "\033[0m" << std::endl << std::endl;
    
    std::cout << "\033[1;33m" << "Objective:" << "\033[0m" << std::endl;
    std::cout << "Jump on colored cells to score points!" << std::endl << std::endl;
    
    std::cout << "\033[1;33m" << "How to Play:" << "\033[0m" << std::endl;
    std::cout << "1. Move using W/A/S/D or arrow keys" << std::endl;
    std::cout << "2. When you step on a cell:" << std::endl;
    std::cout << "   - You jump forward by its value" << std::endl;
    std::cout << "   - You earn points equal to the value" << std::endl;
    std::cout << "3. If you land outside the grid or on unavailable cell, game over!" << std::endl << std::endl;
    
    std::cout << "\033[1;33m" << "Cell Colors:" << "\033[0m" << std::endl;
    std::cout << "\033[31mRed: 1 point\033[0m" << std::endl;
    std::cout << "\033[32mGreen: 2 points\033[0m" << std::endl;
    std::cout << "\033[34mBlue: 3 points\033[0m" << std::endl;
    std::cout << "\033[33mYellow: 4 points\033[0m" << std::endl;
    std::cout << "\033[35mMagenta: 5 points\033[0m" << std::endl << std::endl;
    
    std::cout << "\033[1;37m" << "Press any key to return to menu..." << "\033[0m";
    
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    char input;
    read(STDIN_FILENO, &input, 1);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void MenuController::loadLeaderboard() {
    _leaderboard.clear();
    std::ifstream file(LEADERBOARD_FILE);
    
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            LeaderboardEntry entry;
            if (iss >> entry.playerName >> entry.score >> entry.date) {
                _leaderboard.push_back(entry);
            }
        }
        file.close();
        std::sort(_leaderboard.begin(), _leaderboard.end(), 
                 [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
                     return a.score > b.score;
                 });
    }
}

void MenuController::saveLeaderboard() {
    std::ofstream file(LEADERBOARD_FILE);
    if (file.is_open()) {
        for (const auto& entry : _leaderboard) {
            file << entry.playerName << " " << entry.score << " " << entry.date << std::endl;
        }
        file.close();
    }
}

void MenuController::addToLeaderboard(int score) {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    
    std::ostringstream dateStream;
    dateStream << std::setfill('0') << std::setw(2) << localTime->tm_mday << "/"
               << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "/"
               << (localTime->tm_year + 1900);
    
    LeaderboardEntry newEntry{_playerName, score, dateStream.str()};
    _leaderboard.push_back(newEntry);
    
    std::sort(_leaderboard.begin(), _leaderboard.end(),
             [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
                 return a.score > b.score;
             });
    
    if (_leaderboard.size() > 10) {
        _leaderboard.resize(10);
    }
    
    saveLeaderboard();
}

void MenuController::showLeaderboard() {
    system("clear");
    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminalWidth = w.ws_col;
    
    std::cout << "\033[1;36m";
    int titlePadding = (terminalWidth - 18) / 2;
    std::cout << std::string(titlePadding, ' ') << "=== LEADERBOARD ===" << "\033[0m" << std::endl << std::endl;
    
    if (_leaderboard.empty()) {
        int msgPadding = (terminalWidth - 31) / 2;
        std::cout << std::string(msgPadding, ' ') << "\033[33m" << "No records yet. Be the first!" << "\033[0m" << std::endl;
    } else {
        int tableWidth = 46;
        int tablePadding = (terminalWidth - tableWidth) / 2;
        
        std::cout << std::string(tablePadding, ' ') 
                  << std::left << std::setw(4) << "#" 
                  << std::setw(20) << "Player" 
                  << std::setw(10) << "Score" 
                  << "Date" << std::endl;
        std::cout << std::string(tablePadding, ' ') << std::string(46, '-') << std::endl;
        
        for (size_t i = 0; i < _leaderboard.size(); i++) {
            const auto& entry = _leaderboard[i];
            
            std::string colorCode;
            if (i == 0) colorCode = "\033[1;33m";
            else if (i == 1) colorCode = "\033[1;37m";
            else if (i == 2) colorCode = "\033[1;31m";
            else colorCode = "\033[0m";
            
            std::cout << std::string(tablePadding, ' ') 
                      << colorCode
                      << std::left << std::setw(4) << (i + 1)
                      << std::setw(20) << entry.playerName
                      << std::setw(10) << entry.score
                      << entry.date
                      << "\033[0m" << std::endl;
        }
    }
    
    std::cout << std::endl;
    int hintPadding = (terminalWidth - 37) / 2;
    std::cout << std::string(hintPadding, ' ') << "\033[1;37m" << "Press any key to return to menu..." << "\033[0m";
    
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    char input;
    read(STDIN_FILENO, &input, 1);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void MenuController::saveGame(GameModel* model) {
    GameState state;
    state.playerPosition = model->getPlayerPosition();
    state.score = model->getScore();
    
    const Grid& grid = model->getGrid();
    state.width = grid.getWidth();
    state.height = grid.getHeight();
    
    int totalCells = state.width * state.height;
    state.cellValues.reserve(totalCells);
    state.cellColors.reserve(totalCells);
    state.cellAvailable.reserve(totalCells);
    
    for (int y = 0; y < state.height; y++) {
        for (int x = 0; x < state.width; x++) {
            Position pos(x, y);
            const ICell& cell = grid[pos];
            
            state.cellValues.push_back(cell.getValue());
            state.cellColors.push_back(static_cast<int>(cell.getColor()));
            state.cellAvailable.push_back(cell.isAvailable() ? 1 : 0);
        }
    }
    
    std::ofstream file(SAVE_FILE, std::ios::binary);
    if (file.is_open()) {
        state.serialize(file);
        file.close();
        _hasSavedGame = true;
        
        std::cout << "\033[1;32m" << "Game saved successfully!" << "\033[0m" << std::endl;
        sleep(1);
    } else {
        std::cout << "\033[1;31m" << "Failed to save game!" << "\033[0m" << std::endl;
        sleep(1);
    }
}

bool MenuController::loadGame(GameModel* model) {
    std::ifstream file(SAVE_FILE, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "\033[1;31m" << "No saved game found!" << "\033[0m" << std::endl;
        sleep(1);
        return false;
    }
    
    GameState state;
    state.deserialize(file);
    file.close();
    
    _hasSavedGame = true;
    return true;
}

bool MenuController::runMainMenu() {
    std::vector<std::string> menuItems = {
        "Set Player Name",
        "Start New Game",
        "Load Saved Game",
        "Game Rules",
        "Leaderboard",
        "Exit"
    };
    
    std::ifstream file(SAVE_FILE, std::ios::binary);
    _hasSavedGame = file.good();
    file.close();
    if (!_hasSavedGame) {
        menuItems[2] = "Load Saved Game (No save)";
    } else {
        menuItems[2] = "Load Saved Game";
    }
    
    int selectedIndex = 0;
    bool menuRunning = true;
    
    InputHandler inputHandler;
    
    while (menuRunning) {
        displayMenuItems(menuItems, selectedIndex);
        
        inputHandler.enableCanonicalMode();
        char input;
        read(STDIN_FILENO, &input, 1);
        
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
        } else if (input == '\n' || input == ' ') {
            inputHandler.disableCanonicalMode();
            
            switch(selectedIndex) {
                case 0:
                    setPlayerName();
                    break;
                case 1:
                    return true;
                case 2:
                    if (_hasSavedGame) {
                        std::cout << "\033[1;33m" << "Loading saved game..." << "\033[0m" << std::endl;
                        sleep(1);
                    } else {
                        std::cout << "\033[1;33m" << "No saved game available!" << "\033[0m" << std::endl;
                        sleep(1);
                    }
                    break;
                case 3:
                    showRules();
                    break;
                case 4:
                    showLeaderboard();
                    break;
                case 5:
                    std::cout << "\033[1;36m" << "Goodbye!" << "\033[0m" << std::endl;
                    exit(0);
            }
            
            file.open(SAVE_FILE, std::ios::binary);
            _hasSavedGame = file.good();
            file.close();
            
            if (!_hasSavedGame) {
                menuItems[2] = "Load Saved Game (No save)";
            } else {
                menuItems[2] = "Load Saved Game";
            }
        }
        inputHandler.disableCanonicalMode();
    }
    
    return false;
}

#include "controller/MenuController.h"
#include "controller/InputHandler.h"
#include <iostream>
#include <unistd.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <csignal>
#include <sys/ioctl.h>
#include <termios.h>

MenuController* MenuController::globalMenuController = nullptr;

void MenuController::handleResize(int sig) {
    if (globalMenuController && sig == SIGWINCH) {
        globalMenuController->refreshMenu();
    }
}

void GameState::serialize(std::ofstream& file) const {
    file.write(reinterpret_cast<const char*>(&playerPosition), sizeof(Position));
    file.write(reinterpret_cast<const char*>(&score), sizeof(int));
    file.write(reinterpret_cast<const char*>(&width), sizeof(int));
    file.write(reinterpret_cast<const char*>(&height), sizeof(int));
    
    size_t size = cellValues.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(cellValues.data()), size * sizeof(int));
    }
    
    size = cellColors.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(cellColors.data()), size * sizeof(int));
    }
    
    size = cellAvailable.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(cellAvailable.data()), size * sizeof(int));
    }
    
    size = cellTypes.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(cellTypes.data()), size * sizeof(int));
    }
    
    size = teleportTargetsX.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(teleportTargetsX.data()), size * sizeof(int));
    }
    
    size = teleportTargetsY.size();
    file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
    if (size > 0) {
        file.write(reinterpret_cast<const char*>(teleportTargetsY.data()), size * sizeof(int));
    }
}

void GameState::deserialize(std::ifstream& file) {
    file.read(reinterpret_cast<char*>(&playerPosition), sizeof(Position));
    file.read(reinterpret_cast<char*>(&score), sizeof(int));
    file.read(reinterpret_cast<char*>(&width), sizeof(int));
    file.read(reinterpret_cast<char*>(&height), sizeof(int));
    
    size_t size;
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    cellValues.resize(size);
    if (size > 0) {
        file.read(reinterpret_cast<char*>(cellValues.data()), size * sizeof(int));
    }
    
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    cellColors.resize(size);
    if (size > 0) {
        file.read(reinterpret_cast<char*>(cellColors.data()), size * sizeof(int));
    }
    
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    cellAvailable.resize(size);
    if (size > 0) {
        file.read(reinterpret_cast<char*>(cellAvailable.data()), size * sizeof(int));
    }
    
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    cellTypes.resize(size);
    if (size > 0) {
        file.read(reinterpret_cast<char*>(cellTypes.data()), size * sizeof(int));
    }
    
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    teleportTargetsX.resize(size);
    if (size > 0) {
        file.read(reinterpret_cast<char*>(teleportTargetsX.data()), size * sizeof(int));
    }
    
    file.read(reinterpret_cast<char*>(&size), sizeof(size_t));
    teleportTargetsY.resize(size);
    if (size > 0) {
        file.read(reinterpret_cast<char*>(teleportTargetsY.data()), size * sizeof(int));
    }
}

MenuController::MenuController() : _playerName("Player"), _hasSavedGame(false), 
                                  _lastSelectedOption(-1), _currentSelectedIndex(0) {
    loadLeaderboard();
    
    std::ifstream file(SAVE_FILE, std::ios::binary);
    _hasSavedGame = file.good();
    file.close();
    
    struct sigaction sa;
    sa.sa_handler = handleResize;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, nullptr);
    
    globalMenuController = this;
}

bool MenuController::checkTerminalSize() const {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return (w.ws_col >= 80 && w.ws_row >= 24);
}

void MenuController::displaySizeError() const {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    std::cout << "\033[2J\033[1;1H";
    std::string errorMsg = "Terminal size too small! Minimum: 80x24, Current: " + 
                          std::to_string(w.ws_col) + "x" + std::to_string(w.ws_row);
    
    int errorX = (w.ws_col - errorMsg.length()) / 2;
    int errorY = w.ws_row / 2;
    
    if (errorX < 0) errorX = 0;
    if (errorY < 0) errorY = 0;
    
    std::cout << "\033[" << errorY << ";" << errorX << "H";
    std::cout << "\033[1;31m" << errorMsg << "\033[0m";
    std::cout.flush();
}

void MenuController::refreshMenu() {
    if (!checkTerminalSize()) {
        displaySizeError();
        return;
    }
    
    if (!_currentMenuItems.empty()) {
        displayMenuItems(_currentMenuItems, _currentSelectedIndex);
    }
}

void MenuController::drawAsciiTitle() {
    std::cout << "\033[2J\033[1;1H";
    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminalWidth = w.ws_col;
    int terminalHeight = w.ws_row;
    
    std::vector<std::string> titleLines = {
        " ######   ########  ########  ########  ######  ",
        "##    ##  ##    ##  ##        ##        ##    ##",
        "##        #######   #######   #######   ##    ##", 
        "##        ####      ##        ##        ##    ##",
        "##  ####  ##  ##    ##        ##        ##    ##",
        " ##### #  ##   ###  #######   ########  ######  "
    };
    
    int verticalPadding = terminalHeight / 4;
    for (int i = 0; i < verticalPadding; i++) {
        std::cout << std::endl;
    }
    
    for (const auto& line : titleLines) {
        int padding = (terminalWidth - line.length()) / 2;
        if (padding < 0) padding = 0;
        std::cout << std::string(padding, ' ') << "\033[1;35m" << line << "\033[0m" << std::endl;
    }
    
    std::string subtitle = "A Colorful Number Jumping Adventure!";
    int subtitlePadding = (terminalWidth - subtitle.length()) / 2;
    std::cout << std::string(subtitlePadding, ' ') << "\033[1;36m" << subtitle << "\033[0m" << std::endl << std::endl;
}

void MenuController::displayMenuItems(const std::vector<std::string>& items, int selectedIndex) {
    _currentMenuItems = items;
    _currentSelectedIndex = selectedIndex;
    
    if (!checkTerminalSize()) {
        displaySizeError();
        return;
    }

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminalWidth = w.ws_col;
    int terminalHeight = w.ws_row;
    
    std::cout << "\033[2J\033[3J\033[1;1H";
    std::cout.flush();
    
    std::cout << "\033[?1049h";
    std::cout << "\033[?7l";
    
    std::vector<std::string> titleLines = {
        " ######   ########  ########  ########  #######  ",
        "##    ##  ##     ## ##        ##        ##     ##",
        "##        ##     ## ##        ##        ##     ##", 
        "##   #### ########  #######   #######   ##     ##",
        "##    ##  ##   ##   ##        ##        ##     ##",
        " ######   ##     ## ########  ########  #######  " 
    };
    
    int verticalPadding = terminalHeight / 4;
    for (int i = 0; i < verticalPadding; i++) {
        std::cout << std::endl;
    }
    
    for (const auto& line : titleLines) {
        int padding = (terminalWidth - line.length()) / 2;
        if (padding < 0) padding = 0;
        std::cout << std::string(padding, ' ') << "\033[1;35m" << line << "\033[0m" << std::endl;
    }
    
    std::cout << std::endl;
    
    std::string playerInfo = "\033[1;37mPlayer: \033[1;33m" + _playerName + "\033[0m";
    int playerPadding = (terminalWidth - playerInfo.length()) / 2;
    std::cout << std::string(playerPadding, ' ') << playerInfo << std::endl << std::endl;
    
    std::vector<std::vector<std::string>> numbers = {
        {
            " ##",
            "###", 
            " ##",
            " ##",
            " ##",
            " ##",
            "####"
        },
        {
            "#### ",
            "   ##",
            "  ## ",
            " ##  ",
            "##   ",
            "##   ",
            "#####"
        },
        {
            "#### ",
            "    #",
            "    #",
            " ### ",
            "    #",
            "    #",
            "#### "
        },
        {
            "   ##",
            "  ###",
            " ## #", 
            "##  #",
            "######",
            "    #",
            "    #"
        },
        {
            "######",
            "##    ",
            "##    ",
            "##### ",
            "    ##",
            "    ##",
            "##### "
        },
        {
            "  ### ",
            " ##   ",
            "##    ",
            "##### ",
            "##  ##",
            "##  ##",
            " #### "
        }
    };
    
    int menuStartY = verticalPadding + titleLines.size() + 4;
    int menuStartX = (terminalWidth - 30) / 2;
    
    for (size_t i = 0; i < items.size(); i++) {
        int itemX = menuStartX;
        int itemY = menuStartY + i;
        
        if (itemY >= terminalHeight) break;
        
        std::cout << "\033[" << itemY << ";" << itemX << "H";
        
        if (i == selectedIndex) {
            std::cout << "\033[1;32m" << "  > " << items[i] << " <" << "\033[0m";
        } else {
            std::cout << "\033[37m" << "    " << items[i] << "\033[0m";
        }
        
        std::cout << "\033[K";
    }
    
    std::string colors[6] = {
        "\033[1;31m",
        "\033[1;32m",
        "\033[1;34m",
        "\033[1;33m",
        "\033[1;35m",
        "\033[1;36m"
    };
    
    int num1X = menuStartX - 10;
    int num1Y = menuStartY;
    
    if (num1X > 0) {
        for (size_t row = 0; row < numbers[0].size(); row++) {
            int currentY = num1Y + row;
            if (currentY < terminalHeight) {
                std::cout << "\033[" << currentY << ";" << num1X << "H";
                std::cout << colors[0] << numbers[0][row] << "\033[0m";
            }
        }
    }
    
    int num2X = menuStartX + 35;
    int num2Y = menuStartY;
    
    if (num2X + 5 < terminalWidth) {
        for (size_t row = 0; row < numbers[1].size(); row++) {
            int currentY = num2Y + row;
            if (currentY < terminalHeight) {
                std::cout << "\033[" << currentY << ";" << num2X << "H";
                std::cout << colors[1] << numbers[1][row] << "\033[0m";
            }
        }
    }
    
    int num3X = menuStartX - 12;
    int num3Y = menuStartY + items.size()/2;
    
    if (num3X > 0) {
        for (size_t row = 0; row < numbers[2].size(); row++) {
            int currentY = num3Y + row;
            if (currentY < terminalHeight) {
                std::cout << "\033[" << currentY << ";" << num3X << "H";
                std::cout << colors[2] << numbers[2][row] << "\033[0m";
            }
        }
    }
    
    int num4X = menuStartX + 37;
    int num4Y = menuStartY + items.size()/2;
    
    if (num4X + 5 < terminalWidth) {
        for (size_t row = 0; row < numbers[3].size(); row++) {
            int currentY = num4Y + row;
            if (currentY < terminalHeight) {
                std::cout << "\033[" << currentY << ";" << num4X << "H";
                std::cout << colors[3] << numbers[3][row] << "\033[0m";
            }
        }
    }
    
    int num5X = menuStartX - 10;
    int num5Y = menuStartY + items.size() + 2;
    
    if (num5X > 0 && num5Y + 7 < terminalHeight) {
        for (size_t row = 0; row < numbers[4].size(); row++) {
            int currentY = num5Y + row;
            if (currentY < terminalHeight) {
                std::cout << "\033[" << currentY << ";" << num5X << "H";
                std::cout << colors[4] << numbers[4][row] << "\033[0m";
            }
        }
    }
    
    int num6X = menuStartX + 35;
    int num6Y = menuStartY + items.size() + 2;
    
    if (num6X + 6 < terminalWidth && num6Y + 7 < terminalHeight) {
        for (size_t row = 0; row < numbers[5].size(); row++) {
            int currentY = num6Y + row;
            if (currentY < terminalHeight) {
                std::cout << "\033[" << currentY << ";" << num6X << "H";
                std::cout << colors[5] << numbers[5][row] << "\033[0m";
            }
        }
    }
    
    std::cout << "\033[" << (terminalHeight - 2) << ";1H";
    std::string hint = "\033[3;90mUse UP/DOWN arrows to navigate, ENTER to select\033[0m";
    int hintPadding = (terminalWidth - hint.length()) / 2;
    std::cout << std::string(hintPadding, ' ') << hint;
    std::cout << "\033[K";
    
    std::cout.flush();
}

void MenuController::setPlayerName() {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, nullptr);
    
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    
    std::cout << "\033[?1049l";
    std::cout << "\033[2J\033[3J\033[1;1H";
    std::cout.flush();
    
    struct termios newt = oldt;
    newt.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    while (true) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int terminalWidth = w.ws_col;
        int terminalHeight = w.ws_row;
        
        if (terminalWidth < 80 || terminalHeight < 24) {
            std::cout << "\033[2J\033[3J\033[1;1H";
            std::string errorMsg = "Terminal size too small! Minimum: 80x24, Current: " + 
                                  std::to_string(terminalWidth) + "x" + std::to_string(terminalHeight);
            
            int errorX = (terminalWidth - errorMsg.length()) / 2;
            int errorY = terminalHeight / 2;
            
            if (errorX < 0) errorX = 0;
            if (errorY < 0) errorY = 0;
            
            std::cout << "\033[" << errorY << ";" << errorX << "H";
            std::cout << "\033[1;31m" << errorMsg << "\033[0m";
            std::cout << "\033[" << (terminalHeight - 1) << ";1H";
            std::cout << "\033[33mResize terminal or press Enter to return...\033[0m";
            std::cout.flush();
            
            fd_set readfds;
            struct timeval timeout;
            
            while (true) {
                FD_ZERO(&readfds);
                FD_SET(STDIN_FILENO, &readfds);
                timeout.tv_sec = 0;
                timeout.tv_usec = 100000;
                
                int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
                
                if (ready > 0) {
                    char buf[10];
                    ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
                    if (n > 0 && (buf[0] == '\n' || buf[0] == '\r')) {
                        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                        sa.sa_handler = handleResize;
                        sigaction(SIGWINCH, &sa, nullptr);
                        return;
                    }
                }
                
                struct winsize w2;
                ioctl(STDOUT_FILENO, TIOCGWINSZ, &w2);
                if (w2.ws_col != w.ws_col || w2.ws_row != w.ws_row) {
                    break;
                }
            }
            continue;
        }
        
        std::cout << "\033[2J\033[3J\033[1;1H";
        std::cout.flush();
        
        int verticalCenter = terminalHeight / 2;
        
        std::string title = "=== SET PLAYER NAME ===";
        int titleX = (terminalWidth - title.length()) / 2;
        std::cout << "\033[" << (verticalCenter - 2) << ";" << titleX << "H";
        std::cout << "\033[1;36m" << title << "\033[0m";
        
        std::string currentName = "Current: \033[1;33m" + _playerName + "\033[0m";
        int currentNameX = (terminalWidth - currentName.length()) / 2;
        std::cout << "\033[" << (verticalCenter - 1) << ";" << currentNameX << "H";
        std::cout << currentName;
        
        std::string prompt = "New name: ";
        int promptX = (terminalWidth - prompt.length()) / 2;
        std::cout << "\033[" << verticalCenter << ";" << promptX << "H";
        std::cout << "\033[1;37m" << prompt << "\033[0m";
        
        std::cout << "\033[?25h";
        std::cout.flush();
        
        int inputX = promptX + prompt.length();
        std::cout << "\033[" << verticalCenter << ";" << inputX << "H";
        
        std::string newName;
        std::getline(std::cin, newName);
        
        if (!newName.empty()) {
            _playerName = newName;
            std::string successMsg = "\033[1;32m✓ Name changed!\033[0m";
            int successX = (terminalWidth - successMsg.length()) / 2;
            std::cout << "\033[" << (verticalCenter + 1) << ";" << successX << "H";
            std::cout << successMsg;
        } else {
            std::string unchangedMsg = "\033[33m✗ Name unchanged\033[0m";
            int unchangedX = (terminalWidth - unchangedMsg.length()) / 2;
            std::cout << "\033[" << (verticalCenter + 1) << ";" << unchangedX << "H";
            std::cout << unchangedMsg;
        }
        
        std::cout.flush();
        sleep(1);
        break;
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    sa.sa_handler = handleResize;
    sigaction(SIGWINCH, &sa, nullptr);
    
    std::cout << "\033[2J\033[3J\033[1;1H";
    std::cout.flush();
}


void MenuController::showRules() {
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, nullptr);
    
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    
    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    std::cout << "\033[?1049l";
    std::cout.flush();
    
    bool shouldReturn = false;
    
    while (!shouldReturn) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int terminalWidth = w.ws_col;
        int terminalHeight = w.ws_row;
        
        if (terminalWidth < 80 || terminalHeight < 24) {
            std::cout << "\033[2J\033[3J\033[1;1H";
            std::string errorMsg = "Terminal size too small! Minimum: 80x24, Current: " + 
                                  std::to_string(terminalWidth) + "x" + std::to_string(terminalHeight);
            
            int errorX = (terminalWidth - errorMsg.length()) / 2;
            int errorY = terminalHeight / 2;
            
            if (errorX < 0) errorX = 0;
            if (errorY < 0) errorY = 0;
            
            std::cout << "\033[" << errorY << ";" << errorX << "H";
            std::cout << "\033[1;31m" << errorMsg << "\033[0m";
            std::cout << "\033[" << (terminalHeight - 1) << ";1H";
            std::cout << "\033[33mResize terminal or press any key to return...\033[0m";
            std::cout.flush();
            
            char input;
            read(STDIN_FILENO, &input, 1);
            shouldReturn = true;
            break;
        }
        
        std::cout << "\033[2J\033[3J\033[1;1H";
        std::cout.flush();
        
        int verticalPadding = terminalHeight / 6;
        
        std::string title = "=== GAME RULES ===";
        int titlePadding = (terminalWidth - title.length()) / 2;
        std::cout << "\033[" << verticalPadding << ";" << titlePadding << "H";
        std::cout << "\033[1;36m" << title << "\033[0m";
        
        std::vector<std::string> rules = {
            "Jump on colored cells to score points!",
            "",
            "\033[1;33mObjective:\033[0m",
            "Score 200+ points to WIN the game!",
            "After winning, you can continue playing to set new records.",
            "",
            "\033[1;33mHow to Play:\033[0m",
            "1. Move using W/A/S/D or arrow keys",
            "2. When stepping on a cell:",
            "   - Jump forward by the cell's value",
            "   - Earn points equal to the cell's value",
            "3. Game over if you land outside grid or on unavailable cell!",
            "",
            "\033[1;33mCell Types:\033[0m",
            "\033[31mRed (1): 1 point\033[0m",
            "\033[32mGreen (2): 2 points\033[0m", 
            "\033[34mBlue (3): 3 points\033[0m",
            "\033[33mYellow (4): 4 points\033[0m",
            "\033[35mPurple (5): 5 points\033[0m",
            "",
            "\033[1;33mSpecial Cells:\033[0m",
            "\033[31;1m Bomb (B):\033[0m deducts 20% of score",
            "\033[32;1m Teleport (T):\033[0m teleports to random location",
            "",
            "\033[1;33mSkill Levels:\033[0m",
            "\033[31mBEGINNER: 0-99 points\033[0m",
            "\033[33mADVANCED: 100-199 points\033[0m", 
            "\033[32mWINNER: 200+ points (VICTORY!)\033[0m"
        };
        
        int currentY = verticalPadding + 2;
        for (size_t i = 0; i < rules.size() && currentY < terminalHeight - 2; i++) {
            int padding = (terminalWidth - rules[i].length()) / 2;
            if (padding < 0) padding = 0;
            std::cout << "\033[" << currentY << ";" << padding << "H";
            std::cout << rules[i];
            currentY++;
        }
        
        std::string hint = "\033[1;37mPress any key to return to menu...\033[0m";
        int hintPadding = (terminalWidth - hint.length()) / 2;
        std::cout << "\033[" << (terminalHeight - 1) << ";" << hintPadding << "H";
        std::cout << hint;
        
        std::cout.flush();
        
        fd_set readfds;
        struct timeval timeout;
        
        bool keyPressed = false;
        while (!keyPressed) {
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);
            timeout.tv_sec = 0;
            timeout.tv_usec = 100000;
            
            int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
            
            if (ready > 0) {
                char input;
                read(STDIN_FILENO, &input, 1);
                shouldReturn = true;
                keyPressed = true;
            }
            
            struct winsize w2;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w2);
            if (w2.ws_col != w.ws_col || w2.ws_row != w.ws_row) {
                break;
            }
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    sa.sa_handler = handleResize;
    sigaction(SIGWINCH, &sa, nullptr);
    
    std::cout << "\033[2J\033[3J\033[1;1H";
    std::cout.flush();
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
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigaction(SIGWINCH, &sa, nullptr);
    
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    
    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    std::cout << "\033[?1049l";
    std::cout.flush();
    
    bool shouldReturn = false;
    
    while (!shouldReturn) {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        int terminalWidth = w.ws_col;
        int terminalHeight = w.ws_row;
        
        if (terminalWidth < 80 || terminalHeight < 24) {
            std::cout << "\033[2J\033[3J\033[1;1H";
            std::string errorMsg = "Terminal size too small! Minimum: 80x24, Current: " + 
                                  std::to_string(terminalWidth) + "x" + std::to_string(terminalHeight);
            
            int errorX = (terminalWidth - errorMsg.length()) / 2;
            int errorY = terminalHeight / 2;
            
            if (errorX < 0) errorX = 0;
            if (errorY < 0) errorY = 0;
            
            std::cout << "\033[" << errorY << ";" << errorX << "H";
            std::cout << "\033[1;31m" << errorMsg << "\033[0m";
            std::cout << "\033[" << (terminalHeight - 1) << ";1H";
            std::cout << "\033[33mResize terminal or press any key to return...\033[0m";
            std::cout.flush();
            
            char input;
            read(STDIN_FILENO, &input, 1);
            shouldReturn = true;
            break;
        }
        
        std::cout << "\033[2J\033[3J\033[1;1H";
        std::cout.flush();
        
        int verticalPadding = terminalHeight / 6;
        
        std::cout << "\033[" << verticalPadding << ";1H";
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
            
            int maxEntries = std::min((size_t)10, _leaderboard.size());
            for (int i = 0; i < maxEntries; i++) {
                const auto& entry = _leaderboard[i];
                
                std::string colorCode;
                if (i == 0) colorCode = "\033[1;32m";
                else if (i == 1) colorCode = "\033[1;32m";
                else if (i == 2) colorCode = "\033[1;32m";
                else if (i >= 3 && i <= 5) colorCode = "\033[1;34m";
                else if (i >= 6 && i <= 8) colorCode = "\033[1;33m";
                else if (i == 9) colorCode = "\033[1;37m";
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
        
        std::string hint = "\033[1;37mPress any key to return to menu...\033[0m";
        int hintPadding = (terminalWidth - hint.length()) / 2;
        std::cout << "\033[" << (terminalHeight - 1) << ";" << hintPadding << "H";
        std::cout << hint;
        
        std::cout.flush();
        
        fd_set readfds;
        struct timeval timeout;
        
        bool keyPressed = false;
        while (!keyPressed) {
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);
            timeout.tv_sec = 0;
            timeout.tv_usec = 100000;
            
            int ready = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
            
            if (ready > 0) {
                char input;
                read(STDIN_FILENO, &input, 1);
                shouldReturn = true;
                keyPressed = true;
            }
            
            struct winsize w2;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w2);
            if (w2.ws_col != w.ws_col || w2.ws_row != w.ws_row) {
                break;
            }
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    sa.sa_handler = handleResize;
    sigaction(SIGWINCH, &sa, nullptr);
    
    std::cout << "\033[2J\033[3J\033[1;1H";
    std::cout.flush();
}

void MenuController::saveGame(GameModel* model) {
    GameState state;
    state.playerPosition = model->getPlayerPosition();
    state.score = model->getScore();
    
    const Grid& grid = model->getGrid();
    state.width = grid.getWidth();
    state.height = grid.getHeight();
    
    int totalCells = state.width * state.height;
    
    state.cellValues.clear();
    state.cellColors.clear();
    state.cellAvailable.clear();
    state.cellTypes.clear();
    state.teleportTargetsX.clear();
    state.teleportTargetsY.clear();
    
    state.cellValues.reserve(totalCells);
    state.cellColors.reserve(totalCells);
    state.cellAvailable.reserve(totalCells);
    state.cellTypes.reserve(totalCells);
    state.teleportTargetsX.reserve(totalCells);
    state.teleportTargetsY.reserve(totalCells);
    
    for (int y = 0; y < state.height; y++) {
        for (int x = 0; x < state.width; x++) {
            Position pos(x, y);
            const ICell& cell = grid[pos];
            
            if (dynamic_cast<const TeleportCell*>(&cell)) {
                const TeleportCell* teleportCell = static_cast<const TeleportCell*>(&cell);
                state.cellTypes.push_back(1);
                
                Position tpPos = teleportCell->getTPPos();
                state.teleportTargetsX.push_back(tpPos.getX());
                state.teleportTargetsY.push_back(tpPos.getY());
                
                state.cellValues.push_back(0);
                state.cellColors.push_back(static_cast<int>(Color::GREEN));
            }
            else if (dynamic_cast<const BombCell*>(&cell)) {
                state.cellTypes.push_back(2);
                state.teleportTargetsX.push_back(0);
                state.teleportTargetsY.push_back(0);
                
                state.cellValues.push_back(0);
                state.cellColors.push_back(static_cast<int>(Color::RED));
            }
            else {
                const BasicCell* basicCell = dynamic_cast<const BasicCell*>(&cell);
                state.cellTypes.push_back(0);
                state.teleportTargetsX.push_back(0);
                state.teleportTargetsY.push_back(0);
                
                if (basicCell) {
                    state.cellValues.push_back(basicCell->getValue());
                    state.cellColors.push_back(static_cast<int>(basicCell->getColor()));
                } else {
                    state.cellValues.push_back(0);
                    state.cellColors.push_back(0);
                }
            }
            
            state.cellAvailable.push_back(cell.isAvailable() ? 1 : 0);
        }
    }
    
    std::ofstream file(SAVE_FILE, std::ios::binary);
    if (file.is_open()) {
        state.serialize(file);
        file.close();
        _hasSavedGame = true;
        
        std::cout << "\033[1;32mGame saved successfully!\033[0m" << std::endl;
        sleep(1);
    } else {
        std::cout << "\033[1;31mFailed to save game!\033[0m" << std::endl;
        sleep(1);
    }
    
}


bool MenuController::runMainMenu() {
    std::cout << "\033[?1049h\033[2J\033[1;1H";
    std::cout.flush();

    std::cout << "\033[?1049h";
    std::cout << "\033[?7l";

    globalMenuController = this;

    signal(SIGINT, [](int sig) {
        std::cout << "\033[?7h";
        std::cout << "\033[?1049l";
        system("clear");
        std::cout << "\033[1;36mGoodbye!\033[0m" << std::endl;
        exit(0);
    });

    std::vector<std::string> menuItems = {
        "Start New Game",
        "Load Saved Game",
        "Game Rules",
        "Set Player Name",
        "Leaderboard",
        "Exit"
    };
    
    std::ifstream file(SAVE_FILE, std::ios::binary);
    _hasSavedGame = file.good();
    file.close();
    
    if (!_hasSavedGame) {
        menuItems[1] = "Load Saved Game (No save)";
    } else {
        menuItems[1] = "Load Saved Game";
    }
    
    int selectedIndex = 0;
    
    displayMenuItems(menuItems, selectedIndex);
    
    while (true) {
        char input;
        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        
        ssize_t bytesRead = read(STDIN_FILENO, &input, 1);
        
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        
        if (bytesRead > 0) {
            if (input == '\033') {
                char seq[2];
                
                tcgetattr(STDIN_FILENO, &oldt);
                newt = oldt;
                newt.c_lflag &= ~(ICANON | ECHO);
                tcsetattr(STDIN_FILENO, TCSANOW, &newt);
                
                if (read(STDIN_FILENO, &seq[0], 1) > 0) {
                    if (seq[0] == '[') {
                        if (read(STDIN_FILENO, &seq[1], 1) > 0) {
                            if (seq[1] == 'A') {
                                selectedIndex = (selectedIndex - 1 + menuItems.size()) % menuItems.size();
                                displayMenuItems(menuItems, selectedIndex);
                            } else if (seq[1] == 'B') { 
                                selectedIndex = (selectedIndex + 1) % menuItems.size();
                                displayMenuItems(menuItems, selectedIndex);
                            }
                        }
                    }
                }
                
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            } 
            else if (input == '\n' || input == '\r' || input == ' ') { 
                _lastSelectedOption = selectedIndex;
                
                switch(selectedIndex) {
                    case 0: // Start New Game
                        std::cout << "\033[?7h";
                        std::cout << "\033[?1049l";
                        return true;
                        break;
                    case 1:// Load Saved Game
                        if (_hasSavedGame) {
                            std::cout << "\033[?7h";
                            std::cout << "\033[?1049l";
                            return true;
                        } else {
                            std::cout << "\033[2J\033[1;1H";
                            std::cout << "\033[?1049l";
                            std::cout << "\033[1;33m" << "No saved game available!" << "\033[0m" << std::endl;
                            sleep(1);
                            std::cout << "\033[?1049h";
                            std::cout << "\033[2J\033[1;1H";
                        }
                        break;
                    case 2: 
                        showRules();
                        break;
                    case 3:
                        setPlayerName();
                        break;
                    case 4:
                        showLeaderboard();
                        break;
                    case 5:
                        std::cout << "\033[?7h";
                        std::cout << "\033[?1049l";
                        system("clear");
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
                
                displayMenuItems(menuItems, selectedIndex);
            }
            else if (input == 'q' || input == 27) {
                std::cout << "\033[?7h";
                std::cout << "\033[?1049l";
                system("clear");
                std::cout << "\033[1;36m" << "Goodbye!" << "\033[0m" << std::endl;
                exit(0);
            }
        }
    }
    
    signal(SIGINT, SIG_DFL);
    std::cout << "\033[?7h";
    std::cout << "\033[?1049l";
    system("clear");
    globalMenuController = nullptr;
    return false;
}

bool MenuController::loadGame(GameModel* model) {
    std::ifstream loadFile(SAVE_FILE, std::ios::binary);
    if (!loadFile.is_open()) {
        return false;
    }
    
    GameState state;
    state.deserialize(loadFile);
    loadFile.close();
    
    try {
        model->initializeGameFromState(
            state.cellValues,
            state.cellColors,
            state.cellAvailable,
            state.cellTypes,
            state.teleportTargetsX,
            state.teleportTargetsY,
            state.playerPosition,
            state.score
        );
        return true;
    } catch (...) {
        return false;
    }
}

void MenuController::showWelcomeScreen() {
    std::cout << "\033[?1049h";
    std::cout << "\033[?7l"; 
    std::cout << "\033[2J\033[1;1H";
    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminalWidth = w.ws_col;
    int terminalHeight = w.ws_row;
    
    drawAsciiTitle();
    
    std::cout << "\033[" << (terminalHeight - 2) << ";1H";
    std::cout << "\033[1;37mPress any key to continue...\033[0m";
    std::cout.flush();
    
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    char input;
    read(STDIN_FILENO, &input, 1);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

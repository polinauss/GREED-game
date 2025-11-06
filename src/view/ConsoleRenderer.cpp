#include "view/ConsoleRenderer.h"
#include "model/Cell.h"
#include <iostream>
#include <unistd.h>
#include <set>
#include <functional>

ConsoleRenderer::ConsoleRenderer(): _playerSymbol("X"), _emptycellSymbol(".") {
    initializeColorCodes();
}

void ConsoleRenderer::initializeColorCodes() {
    _colorCodes[Color::RED] = "\033[31m";
    _colorCodes[Color::GREEN] = "\033[32m";
    _colorCodes[Color::BLUE] = "\033[34m";
    _colorCodes[Color::YELLOW] = "\033[33m";
    _colorCodes[Color::CYAN] = "\033[36m";
    _colorCodes[Color::MAGENTA] = "\033[35m";
    _colorCodes[Color::WHITE] = "\033[37m";
    _colorCodes[Color::DEFAULT] = "\033[0m";
    _colorCodes[Color::HIGHLIGHT] = "\033[47;30m";
}

std::string ConsoleRenderer::getColorCode(Color color) const {
    auto it = _colorCodes.find(color);
    if (it != _colorCodes.end()) {
        return it->second;
    }
    return _colorCodes.at(Color::DEFAULT);
}

void ConsoleRenderer::clearScreen() const {
    if (system("clear")) {
        std::cout << "\033[2J\033[1;1H";
    }
}

void ConsoleRenderer::drawCell(const Cell& cell, bool isPlayer, bool isAvailableMove) const {
    std::string resetCode = _colorCodes.at(Color::DEFAULT);
    std::cout << resetCode;

    if (isPlayer) {
        std::string colorCode = "\033[1;37;44m";
        std::cout << colorCode << _playerSymbol << resetCode;
    } else if (isAvailableMove) {
        std::string colorCode = "\033[44;37m";
        std::cout << colorCode << cell.getValue() << resetCode;
    } else if (!cell.isAvailable()) {
        std::string colorCode = _colorCodes.at(Color::DEFAULT);
        std::cout << colorCode << _emptycellSymbol << resetCode;
    } else {
        std::string colorCode = _colorCodes.at(cell.getColor());
        std::cout << colorCode << cell.getValue() << resetCode;
    }

    std::cout << resetCode;
}

void ConsoleRenderer::drawGrid(const Grid& grid, const Position& playerPosition, const std::vector<Position>& availableMoves) const {
    int width = grid.getWidth();
    int height = grid.getHeight();

    auto positionComparator = [](const Position& a, const Position& b) {
        if (a.getY() != b.getY()) return a.getY() < b.getY();  
        else return a.getX() < b.getX();                       
    };

    std::set<Position, decltype(positionComparator)> availableSet(
        availableMoves.begin(), 
        availableMoves.end(),
        positionComparator
    );
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Position currentPos(x, y);
            const Cell& cell = grid.getCell(currentPos);
            bool isPlayer = (currentPos == playerPosition);
            bool isAvailableMove = availableSet.find(currentPos) != availableSet.end();

            drawCell(cell, isPlayer, isAvailableMove);
        }
        std::cout << std::endl;
    }
}

void ConsoleRenderer::displayMenu(const std::vector<std::string>& menuItems, int selectedIndex, const std::string& playerName) const {
    clearScreen();
    
    // Получаем размеры терминала
    int terminalHeight = 25;
    int terminalWidth = 80;
    
    // Вычисляем вертикальный отступ для центрирования
    int totalMenuHeight = menuItems.size() + 6;
    int verticalPadding = (terminalHeight - totalMenuHeight) / 2;
    
    // Выводим пустые строки для вертикального центрирования
    for (int i = 0; i < verticalPadding; i++) {
        std::cout << std::endl;
    }
    
    // Горизонтальное центрирование заголовка
    std::string title = "=== GAME MENU ===";
    int titlePadding = (terminalWidth - title.length()) / 2;
    std::cout << std::string(titlePadding, ' ') << "\033[1;36m" << title << "\033[0m" << std::endl;
    
    // Информация о игроке
    std::string playerInfo = "Player: " + playerName;
    int playerPadding = (terminalWidth - playerInfo.length()) / 2;
    std::cout << std::string(playerPadding, ' ') << playerInfo << std::endl << std::endl;
    
    // Выводим пункты меню
    for (int i = 0; i < menuItems.size(); i++) {
        std::string menuItem = menuItems[i];
        int itemPadding = (terminalWidth - menuItem.length() - 4) / 2;
        
        std::cout << std::string(itemPadding, ' ');
        
        if (i == selectedIndex) {
            std::cout << "\033[1;32m> " << menuItem << " <\033[0m";
        } else {
            std::cout << "  " << menuItem << "  ";
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    
    // Подсказка
    std::string hint = "Use UP/DOWN arrows to navigate, ENTER to select";
    int hintPadding = (terminalWidth - hint.length()) / 2;
    std::cout << std::string(hintPadding, ' ') << "\033[3m" << hint << "\033[0m" << std::endl;
}

void ConsoleRenderer::drawScore(int score) const {
    std::cout << "Score: " << score << std::endl;
}

void ConsoleRenderer::displayWelcomeScreen() const {
    clearScreen();
    std::cout << "\033[1;36m" << "=== WELCOME TO THE GAME ===" << "\033[0m" << std::endl;
}

void ConsoleRenderer::displayGameOver() const {
    clearScreen();
    std::cout << "\033[1;31m" << "=== GAME OVER ===" << "\033[0m" << std::endl;
}

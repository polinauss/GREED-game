#include "view/ConsoleRenderer.h"
#include <iostream>
#include <unistd.h>
#include <set>
#include <functional>
#include <iostream>


ConsoleRenderer::ConsoleRenderer(Position offset): _offset(offset + Position(0, 1)), _playerSymbol("X"), _emptycellSymbol(".") {
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
    _colorCodes[Color::BLUEHIGHLIGHT] = "\033[44;37m";
    _colorCodes[Color::REDHIGHLIGHT] = "\033[41m";
}

std::string ConsoleRenderer::getColorCode(Color color) const {
    auto it = _colorCodes.find(color);
    if (it != _colorCodes.end()) {
        return it->second;
    }
    return _colorCodes.at(Color::DEFAULT);
}

void ConsoleRenderer::moveCursor(const Position& pos) const {
    std::cout << "\033[" << pos.getY() << ";" << pos.getX() << "H";
}

void ConsoleRenderer::showCursor() const {
    std::cout << "\033[?25h";
    std::cout.flush();
}

void ConsoleRenderer::hideCursor() const {
    std::cout << "\033[?25l";
    std::cout.flush();
}

void ConsoleRenderer::drawBasicCell(const BasicCell& cell, const Position& pos, Color highlightColor) const {
    moveCursor(pos);

    if (highlightColor != Color::DEFAULT) {
        std::cout << _colorCodes.at(highlightColor);
    }

    if (cell.isAvailable()) {
        std::string colorCode = _colorCodes.at(cell.getColor());
        std::cout << colorCode << cell.getValue();
    } else {
        std::cout << _emptycellSymbol;
    }

    std::cout << _colorCodes.at(Color::DEFAULT);
}

void ConsoleRenderer::drawStartingState(const Grid& grid) {
    hideCursor();
    int gridWidth = grid.getWidth();
    int gridHeight = grid.getHeight();

    for (int row = 0; row < gridHeight; row++) {
        for (int col = 0; col < gridWidth; col++) {
            const Position& cellPos = Position(col, row);
            const ICell& cell = grid[cellPos];
            cell.acceptRender(*this, cellPos + _offset);
        }
    }
}

void ConsoleRenderer::drawPlayer(const Position& playerPos) {
    moveCursor(playerPos + _offset);
    std::cout << _colorCodes.at(Color::DEFAULT) << _playerSymbol << _colorCodes.at(Color::DEFAULT);
    std::cout.flush();
}

void ConsoleRenderer::drawMove(const Grid& grid, const std::vector<Position>& affectedElements) {
    for (const Position& pos: affectedElements) {
        grid[pos].acceptRender(*this, pos + _offset);
    }

    std::cout.flush();
}   

void ConsoleRenderer::highlightMoveDirection(const Grid& grid, std::vector<std::pair<bool, Position>>& availableMoves, Direction direction) {
    for (std::pair<bool, Position> elem: availableMoves) {
        if (elem.first) {
            grid[elem.second].acceptRender(*this, elem.second + _offset);
        }
    }

    int index = static_cast<int>(direction);
    Position highlightedCellPos = availableMoves[index].second;
    if (availableMoves[index].first) {
        grid[highlightedCellPos].acceptRender(*this, highlightedCellPos + _offset, Color::BLUEHIGHLIGHT); 
    }

    std::cout.flush();
}

void ConsoleRenderer::highlightGameOverState(const Grid& grid) {
    int gridWidth = grid.getWidth();
    int gridHeight = grid.getHeight();

    std::cout << "\033[s";
    
    for (int row = 0; row < gridHeight; row++) {
        for (int col = 0; col < gridWidth; col++) {
            const Position& cellPos = Position(col, row);
            const ICell& cell = grid[cellPos];

            if (cell.isAvailable()) {
                cell.acceptRender(*this, cellPos + _offset, Color::REDHIGHLIGHT);
            } else {
                cell.acceptRender(*this, cellPos + _offset);
            }
        }
    }   
}

void ConsoleRenderer::clearScreen() const {
    if (system("clear")) {
        std::cout << "\033[2J\033[1;1H";
    }
}

void ConsoleRenderer::resetCursor() const {
    showCursor();
    moveCursor(Position(1, 999));
    std::cout.flush();
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

void ConsoleRenderer::displayMenu(const std::vector<std::string>& menuItems, int selectedIndex, const std::string& playerName) const {
    clearScreen();
    
    int terminalHeight = 25;
    int terminalWidth = 80;
    
    int totalMenuHeight = menuItems.size() + 6;
    int verticalPadding = (terminalHeight - totalMenuHeight) / 2;
    
    for (int i = 0; i < verticalPadding; i++) {
        std::cout << std::endl;
    }
    
    std::string title = "=== GAME MENU ===";
    int titlePadding = (terminalWidth - title.length()) / 2;
    std::cout << std::string(titlePadding, ' ') << "\033[1;36m" << title << "\033[0m" << std::endl;
    
    std::string playerInfo = "Player: " + playerName;
    int playerPadding = (terminalWidth - playerInfo.length()) / 2;
    std::cout << std::string(playerPadding, ' ') << playerInfo << std::endl << std::endl;
    
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
    
    std::string hint = "Use UP/DOWN arrows to navigate, ENTER to select";
    int hintPadding = (terminalWidth - hint.length()) / 2;
    std::cout << std::string(hintPadding, ' ') << "\033[3m" << hint << "\033[0m" << std::endl;
}
#include "view/ConsoleRenderer.h"
#include <iostream>
#include <unistd.h>
#include <set>
#include <functional>
#include <iostream>


ConsoleRenderer::ConsoleRenderer(Position offset): _offset(offset + Position(0, 1)), _playerSymbol("X "), _emptycellSymbol("  ") {
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
    _colorCodes[Color::BLACK] = "\033[30m";
}

std::string ConsoleRenderer::getColorCode(Color color) const {
    auto it = _colorCodes.find(color);
    if (it != _colorCodes.end()) {
        return it->second;
    }
    return _colorCodes.at(Color::DEFAULT);
}

void ConsoleRenderer::moveCursor(const Position& pos) const {
    std::cout << "\033[" << (pos.getY() +1)<< ";" << (pos.getX()+1) << "H";
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

    if (cell.isAvailable()) {
        std::string colorCode = _colorCodes.at(cell.getColor());
        
        if (highlightColor != Color::DEFAULT) {
            std::cout << _colorCodes.at(highlightColor) << "\033[1m" 
                      << cell.getValue() << " " << "\033[0m";
        } else {
            std::cout << "\033[47m" << colorCode << cell.getValue() << " " << "\033[0m";
        }
    } else {
        std::cout << "\033[47m\033[30m" << "· " << "\033[0m";
    }
}

void ConsoleRenderer::drawStartingState(const Grid& grid) {
    hideCursor();
    int gridWidth = grid.getWidth();
    int gridHeight = grid.getHeight();
    
    int visualWidth = gridWidth * 2;

    for (int col = -1; col <= visualWidth; col++) {
        Position borderPos(col, -1);
        moveCursor(borderPos + _offset);
        std::cout << "\033[40m \033[0m";
    }

    for (int row = 0; row < gridHeight; row++) {
        Position leftBorderPos(-1, row);
        moveCursor(leftBorderPos + _offset);
        std::cout << "\033[40m \033[0m";
        
        Position rightBorderPos(visualWidth, row);
        moveCursor(rightBorderPos + _offset);
        std::cout << "\033[40m \033[0m";
    }
    
    for (int col = -1; col <= visualWidth; col++) {
        Position bottomBorderPos(col, gridHeight);
        moveCursor(bottomBorderPos + _offset);
        std::cout << "\033[40m \033[0m";
    }

    for (int row = 0; row < gridHeight; row++) {
        for (int col = 0; col < gridWidth; col++) {
            const Position& cellPos = Position(col, row);
            const ICell& cell = grid[cellPos];
            
            Position drawPos = Position(col * 2, row) + _offset;
            
            std::cout << "\033[47m";
            cell.acceptRender(*this, drawPos);
            std::cout << "\033[0m";
        }
    }
    
    std::cout.flush();
}

void ConsoleRenderer::drawPlayer(const Position& playerPos) {
    Position drawPos = Position(playerPos.getX() * 2, playerPos.getY()) + _offset;
    moveCursor(drawPos);
    std::cout << _colorCodes.at(Color::DEFAULT) << _playerSymbol << _colorCodes.at(Color::DEFAULT);
    std::cout.flush();
}

void ConsoleRenderer::drawMove(const Grid& grid, const std::vector<Position>& affectedElements) {
    for (const Position& pos: affectedElements) {
        Position drawPos = Position(pos.getX() * 2, pos.getY()) + _offset;
        
        moveCursor(drawPos);
        std::cout << "\033[47m";
        grid[pos].acceptRender(*this, drawPos);
        std::cout << "\033[0m";
    }

    std::cout.flush();
} 

void ConsoleRenderer::highlightMoveDirection(const Grid& grid, std::vector<std::pair<bool, Position>>& availableMoves, Direction direction) {
    for (std::pair<bool, Position> elem: availableMoves) {
        if (elem.first) {
            Position drawPos = Position(elem.second.getX() * 2, elem.second.getY()) + _offset;
            moveCursor(drawPos);
            std::cout << "\033[47m";
            grid[elem.second].acceptRender(*this, drawPos);
            std::cout << "\033[0m";
        }
    }
    
    int index = static_cast<int>(direction);
    if (availableMoves[index].first) {
        Position highlightedCellPos = availableMoves[index].second;
        Position drawPos = Position(highlightedCellPos.getX() * 2, highlightedCellPos.getY()) + _offset;
        moveCursor(drawPos);
        grid[highlightedCellPos].acceptRender(*this, drawPos, Color::BLUEHIGHLIGHT); 
    }

    std::cout.flush();
}

void ConsoleRenderer::drawScoreAtPosition(int score, const Position& pos) const {
    moveCursor(pos);
    std::cout << "\033[1;36mScore: " << score << "\033[0m";
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
            
            Position drawPos = Position(col * 2, row) + _offset;
            moveCursor(drawPos);
            
            if (cell.isAvailable()) {
                cell.acceptRender(*this, drawPos, Color::REDHIGHLIGHT);
            } else {
                std::cout << "\033[47m   \033[0m";
            }
        }
    }
    std::cout.flush();
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
    if (system("clear")) {
        std::cout << "\033[2J\033[3J\033[H";
    }
    
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

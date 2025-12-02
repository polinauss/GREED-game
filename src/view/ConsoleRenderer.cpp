#include "view/ConsoleRenderer.h"
#include <iostream>
#include <unistd.h>
#include <set>
#include <functional>
#include <iomanip>
#include <fstream>

ConsoleRenderer::ConsoleRenderer(Position offset): 
    _offset(offset + Position(0, 3)),
    _playerSymbol("X"), 
    _emptycellSymbol(".") {
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
    _colorCodes[Color::WHITEBACKGROUND] = "\033[47m";
    _colorCodes[Color::BLACK] = "\033[40m";
    _colorCodes[Color::GRAY] = "\033[48;5;240m";
    _colorCodes[Color::DARK_GRAY] = "\033[100m";
}

void ConsoleRenderer::fillScreenWithColor(Color color) const {
    std::string colorCode = getColorCode(color);
    std::cout << colorCode << "\033[2J" << "\033[H";
    std::cout.flush();
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

void ConsoleRenderer::drawGameInfo(const std::string& playerName, int score) const {
    for (int i = 1; i <= 3; i++) {
        moveCursor(Position(1, i));
        std::cout << "\033[2K";
    }
    
    moveCursor(Position(1, 1));
    std::cout << getColorCode(Color::DARK_GRAY) << getColorCode(Color::WHITE)
              << " ИГРОК: " << playerName 
              << " | СЧЕТ: " << std::setw(6) << score << " "
              << getColorCode(Color::DEFAULT);
    
    moveCursor(Position(1, 2));
    std::cout << getColorCode(Color::DARK_GRAY) << getColorCode(Color::WHITE)
              << " УПРАВЛЕНИЕ: СТРЕЛКИ/WASD | ПАУЗА: P | МЕНЮ: M | ВЫХОД: Q | СОХРАНИТЬ: S "
              << getColorCode(Color::DEFAULT);
}

void ConsoleRenderer::drawBlackBorder(int gridWidth, int gridHeight) const {
    std::string borderColor = getColorCode(Color::BLACK);
    std::string reset = getColorCode(Color::DEFAULT);
    
    for (int x = -1; x <= gridWidth * 3; x++) {
        moveCursor(_offset + Position(x, -1));
        std::cout << borderColor << " " << reset;
    }
    
    for (int x = -1; x <= gridWidth * 3; x++) {
        moveCursor(_offset + Position(x, gridHeight));
        std::cout << borderColor << " " << reset;
    }
    
    for (int y = 0; y < gridHeight; y++) {
        moveCursor(_offset + Position(-1, y));
        std::cout << borderColor << " " << reset;
    }
    
    for (int y = 0; y < gridHeight; y++) {
        moveCursor(_offset + Position(gridWidth * 3, y));
        std::cout << borderColor << " " << reset;
    }
}

void ConsoleRenderer::drawLargeNumber(int number, Position startPos, Color color) const {
    const char* patterns[5][5] = {
        {" # ",
         "## ",
         " # ",
         " # ",
         "###"},
        {"###",
         "  #",
         "###",
         "#  ",
         "###"},
        {"###",
         "  #",
         " ##",
         "  #",
         "###"},
        {"# #",
         "# #",
         "###",
         "  #",
         "  #"},
        {"###",
         "#  ",
         "###",
         "  #",
         "###"}
    };
    
    if (number < 1 || number > 5) return;
    
    int idx = number - 1;
    std::string colorCode = getColorCode(color);
    std::string reset = getColorCode(Color::DEFAULT);
    
    for (int row = 0; row < 5; row++) {
        moveCursor(Position(startPos.getX(), startPos.getY() + row));
        std::cout << colorCode << patterns[idx][row] << reset;
    }
}

void ConsoleRenderer::drawDecorationsAroundField() const {
    drawLargeNumber(1, Position(_offset.getX() - 15, _offset.getY() + 2), Color::RED);
    drawLargeNumber(2, Position(_offset.getX() + 38, _offset.getY() + 2), Color::GREEN);
    drawLargeNumber(3, Position(_offset.getX() - 15, _offset.getY() + 10), Color::BLUE);
    drawLargeNumber(4, Position(_offset.getX() + 38, _offset.getY() + 10), Color::YELLOW);
    drawLargeNumber(5, Position(_offset.getX() + 12, _offset.getY() - 8), Color::MAGENTA);
}

void ConsoleRenderer::drawBasicCell(const BasicCell& cell, const Position& pos, Color highlightColor) const {
    moveCursor(pos);

    if (cell.isAvailable()) {
        std::string colorCode = getColorCode(cell.getColor());
        if (highlightColor != Color::DEFAULT) {
            colorCode = getColorCode(highlightColor);
        }
        std::cout << colorCode << " " << cell.getValue() << " ";
    } else {
        if (highlightColor != Color::DEFAULT) {
            std::cout << getColorCode(highlightColor) << "   ";
        } else {
            std::cout << getColorCode(Color::WHITEBACKGROUND) << "   ";
        }
    }

    std::cout << getColorCode(Color::DEFAULT);
}

void ConsoleRenderer::drawStartingState(const Grid& grid) {
    hideCursor();
    
    clearScreen();
    
    drawGameInfo("Игрок", 0);
    
    drawBlackBorder(grid.getWidth(), grid.getHeight());
    
    drawDecorationsAroundField();
    
    for (int row = 0; row < grid.getHeight(); row++) {
        for (int col = 0; col < grid.getWidth(); col++) {
            const Position& cellPos = Position(col, row);
            const ICell& cell = grid[cellPos];
            cell.acceptRender(*this, Position(cellPos.getX() * 3, cellPos.getY()) + _offset);
        }
    }
    
    std::cout.flush();
}

void ConsoleRenderer::drawPlayer(const Position& playerPos) {
    Position renderPos = Position(playerPos.getX() * 3 + 1, playerPos.getY()) + _offset;
    moveCursor(renderPos);
    std::cout << getColorCode(Color::WHITE) << getColorCode(Color::BLACK) << _playerSymbol << getColorCode(Color::DEFAULT);
    std::cout.flush();
}

void ConsoleRenderer::drawMove(const Grid& grid, const std::vector<Position>& affectedElements) {
    for (const Position& pos: affectedElements) {
        grid[pos].acceptRender(*this, Position(pos.getX() * 3, pos.getY()) + _offset);
    }
    std::cout.flush();
}

void ConsoleRenderer::highlightMoveDirection(const Grid& grid, std::vector<std::pair<bool, Position>>& availableMoves, Direction direction) {
    for (std::pair<bool, Position> elem: availableMoves) {
        if (elem.first) {
            grid[elem.second].acceptRender(*this, Position(elem.second.getX() * 3, elem.second.getY()) + _offset);
        }
    }

    int index = static_cast<int>(direction);
    if (index >= 0 && index < availableMoves.size()) {
        Position highlightedCellPos = availableMoves[index].second;
        if (availableMoves[index].first) {
            grid[highlightedCellPos].acceptRender(*this, 
                Position(highlightedCellPos.getX() * 3, highlightedCellPos.getY()) + _offset, 
                Color::BLUEHIGHLIGHT); 
        }
    }

    std::cout.flush();
}

void ConsoleRenderer::highlightGameOverState(const Grid& grid) {
    int gridWidth = grid.getWidth();
    int gridHeight = grid.getHeight();

    std::cout << "\033[s";

    std::string blueColor = _colorCodes.at(Color::BLUEHIGHLIGHT);
    std::string resetColor = _colorCodes.at(Color::DEFAULT);

    for (int x = -1; x <= gridWidth * 3; x++) {
        moveCursor(_offset + Position(x, -1));
        std::cout << blueColor << " " << resetColor;
        
        moveCursor(_offset + Position(x, gridHeight));
        std::cout << blueColor << " " << resetColor;
    }

    for (int y = 0; y < gridHeight; y++) {
        moveCursor(_offset + Position(-1, y));
        std::cout << blueColor << " " << resetColor;
        
        moveCursor(_offset + Position(gridWidth * 3, y));
        std::cout << blueColor << " " << resetColor;
    }
    
    for (int row = 0; row < gridHeight; row++) {
        for (int col = 0; col < gridWidth; col++) {
            const Position& cellPos = Position(col, row);
            const ICell& cell = grid[cellPos];

            if (cell.isAvailable()) {
                cell.acceptRender(*this, Position(cellPos.getX() * 3, cellPos.getY()) + _offset, Color::REDHIGHLIGHT);
            } else {
                cell.acceptRender(*this, Position(cellPos.getX() * 3, cellPos.getY()) + _offset, Color::WHITEBACKGROUND);
            }
        }
    }   
}

void ConsoleRenderer::clearScreen() const {
    std::cout << "\033[2J\033[H" << std::flush;
}

void ConsoleRenderer::resetCursor() const {
    showCursor();
    moveCursor(Position(1, 999));
    std::cout.flush();
}

void ConsoleRenderer::drawScore(int score) const {
    moveCursor(Position(1, 3));
    std::cout << "\033[2K";
    moveCursor(Position(1, 3));
    std::cout << getColorCode(Color::DARK_GRAY) << getColorCode(Color::WHITE)
              << " Счет: " << score << " "
              << getColorCode(Color::DEFAULT);
}

void ConsoleRenderer::displayMenu(const std::vector<std::string>& menuItems, int selectedIndex, const std::string& playerName) const {
    clearScreen();
    
    drawLargeNumber(1, Position(5, 3), Color::RED);
    drawLargeNumber(2, Position(5, 10), Color::GREEN);
    drawLargeNumber(3, Position(5, 17), Color::BLUE);
    
    drawLargeNumber(4, Position(65, 3), Color::YELLOW);
    drawLargeNumber(5, Position(65, 10), Color::MAGENTA);
    drawLargeNumber(1, Position(65, 17), Color::CYAN);
    
    drawLargeNumber(2, Position(35, 1), Color::RED);
    drawLargeNumber(3, Position(35, 22), Color::GREEN);
    
    moveCursor(Position(34, 8));
    std::cout << getColorCode(Color::CYAN) << "ГЛАВНОЕ МЕНЮ" << getColorCode(Color::DEFAULT);
    
    moveCursor(Position(32, 10));
    std::cout << getColorCode(Color::WHITE) << "Игрок: " << playerName << getColorCode(Color::DEFAULT);
    
    for (size_t i = 0; i < menuItems.size(); i++) {
        moveCursor(Position(30, 12 + i));
        if (i == selectedIndex) {
            std::cout << getColorCode(Color::GREEN) << "> " << menuItems[i] << getColorCode(Color::DEFAULT);
        } else {
            std::cout << getColorCode(Color::WHITE) << "  " << menuItems[i] << getColorCode(Color::DEFAULT);
        }
    }
    
    moveCursor(Position(25, 19));
    std::cout << getColorCode(Color::GREEN) << "Выберите пункт (1-6) или Q для выхода" << getColorCode(Color::DEFAULT);
    
    std::cout.flush();
}

void ConsoleRenderer::showRules() const {
    clearScreen();
    
    moveCursor(Position(35, 3));
    std::cout << getColorCode(Color::RED) << "ПРАВИЛА" << getColorCode(Color::DEFAULT);
    
    moveCursor(Position(20, 5));
    std::cout << "1. Двигайтесь стрелками или WASD";
    
    moveCursor(Position(20, 6));
    std::cout << "2. Цифры на поле: 1-5";
    
    moveCursor(Position(20, 7));
    std::cout << "3. Счет суммируется";
    
    moveCursor(Position(20, 8));
    std::cout << "4. Игра до пустой клетки";
    
    moveCursor(Position(20, 10));
    std::cout << "Пауза/Меню: P или M";
    
    moveCursor(Position(20, 11));
    std::cout << "Сохранить игру: S";
    
    moveCursor(Position(20, 12));
    std::cout << "Выход: Q";
    
    moveCursor(Position(25, 15));
    std::cout << "Нажмите любую клавишу...";
    
    std::cout.flush();
}

void ConsoleRenderer::showLeaderboard(const std::vector<std::pair<std::string, int>>& leaders) const {
    clearScreen();
    
    moveCursor(Position(35, 3));
    std::cout << getColorCode(Color::YELLOW) << "ЛИДЕРЫ" << getColorCode(Color::DEFAULT);
    
    if (leaders.empty()) {
        moveCursor(Position(30, 8));
        std::cout << "Пока нет результатов";
    } else {
        for (size_t i = 0; i < leaders.size(); i++) {
            moveCursor(Position(30, 6 + i));
            std::cout << (i + 1) << ". " << leaders[i].first 
                      << " - " << leaders[i].second;
        }
    }
    
    moveCursor(Position(25, 18));
    std::cout << "Нажмите любую клавишу...";
    
    std::cout.flush();
}

void ConsoleRenderer::showNameInput() const {
    clearScreen();
    
    moveCursor(Position(35, 8));
    std::cout << getColorCode(Color::CYAN) << "Введите ваше имя:" << getColorCode(Color::DEFAULT);
    
    moveCursor(Position(35, 10));
    std::cout << "____________________";
    
    moveCursor(Position(35, 12));
    std::cout << getColorCode(Color::GREEN) << "Enter - подтвердить" << getColorCode(Color::DEFAULT);
    
    std::cout.flush();
}

void ConsoleRenderer::displayWelcomeScreen() const {
    clearScreen();
    
    moveCursor(Position(30, 10));
    std::cout << getColorCode(Color::CYAN) 
              << "ДОБРО ПОЖАЛОВАТЬ В ИГРУ" 
              << getColorCode(Color::DEFAULT);
    
    moveCursor(Position(25, 12));
    std::cout << "Нажмите любую клавишу...";
    
    std::cout.flush();
}

void ConsoleRenderer::displayGameOver() const {
    clearScreen();
    
    moveCursor(Position(35, 10));
    std::cout << getColorCode(Color::RED) 
              << "ИГРА ОКОНЧЕНА" 
              << getColorCode(Color::DEFAULT);
    
    moveCursor(Position(30, 12));
    std::cout << "Нажмите любую клавишу...";
    
    std::cout.flush();
}

void ConsoleRenderer::displayLoadGameScreen() const {
    clearScreen();
    
    moveCursor(Position(35, 10));
    std::cout << getColorCode(Color::YELLOW) 
              << "ЗАГРУЗКА ИГРЫ..." 
              << getColorCode(Color::DEFAULT);
    
    std::cout.flush();
    sleep(1);
}

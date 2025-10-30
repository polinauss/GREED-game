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

// вызывается один раз в конце программы: возвращает терминал в исходное состояние
void ConsoleRenderer::clearScreen() const {
    if (system("clear")) {
        std::cout << "\033[2J\033[1;1H";
    }
}

// всопомогательный метода: рисует клетку
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

//основной метод рендеринга: отображение игрового поля с подсветкой доступных ходов
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

void ConsoleRenderer::displayWelcomeScreen() const {}
void ConsoleRenderer::displayGameOver() const {}
void ConsoleRenderer::drawScore(int score) const {}



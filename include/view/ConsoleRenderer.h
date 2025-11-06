#ifndef CONSOLERENDERER
#define CONSOLERENDERER

#include "model/Grid.h"
#include "model/Player.h"
#include "model/Position.h"
#include "core/Color.h"
#include <string>
#include <vector>
#include <map>

class ConsoleRenderer {
private:
    std::map<Color, std::string> _colorCodes;

    std::string _playerSymbol;
    std::string _emptycellSymbol;

    void initializeColorCodes();

public:
    ConsoleRenderer();
    ~ConsoleRenderer() = default;

    void drawGrid(const Grid& grid, const Position& playerPosition, const std::vector<Position>& availableMoves) const;
    void drawCell(const Cell& cell, bool isPlayer, bool isAvailableMove) const;
    void drawScore(int score) const;
    void displayWelcomeScreen() const;
    void displayGameOver() const;
    void displayMenu(const std::vector<std::string>& menuItems, int selectedIndex, const std::string& playerName) const;
    void clearScreen() const;

    std::string getColorCode(Color color) const;
};

#endif

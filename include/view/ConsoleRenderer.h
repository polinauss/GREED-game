#ifndef CONSOLERENDERER
#define CONSOLERENDERER

#include "model/Grid.h"
#include "model/Player.h"
#include "model/Position.h"
#include "model/cells/BasicCell.h"
#include "core/Directions.h"
#include "interfaces/ICellRenderVisitor.h"
#include "core/Color.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>

class ConsoleRenderer: public ICellRenderVisitor{
private:
    std::map<Color, std::string> _colorCodes;

    std::string _playerSymbol;
    std::string _emptycellSymbol;
    Position _offset;

public:
    ConsoleRenderer(Position offset);
    ~ConsoleRenderer() = default;

    void drawBasicCell(const BasicCell& cell, const Position& pos, Color highlightColor = Color::DEFAULT) const override;

    void drawStartingState(const Grid& grid);
    void drawPlayer(const Position& playerPos);
    void drawMove(const Grid& grid, const std::vector<Position>& affectedElements);
    void highlightMoveDirection(const Grid& grid, std::vector<std::pair<bool, Position>>& availableMoves, Direction direction);
    void highlightGameOverState(const Grid& grid);

    void clearScreen() const;
    void resetCursor() const;

    void drawScore(int score) const;
    void displayWelcomeScreen() const;
    void displayGameOver() const;
    void displayMenu(const std::vector<std::string>& menuItems, int selectedIndex, const std::string& playerName) const;

private:
    void initializeColorCodes();
    std::string getColorCode(Color color) const;
    void moveCursor(const Position& pos) const;
    void hideCursor() const;
    void showCursor() const;

};

#endif

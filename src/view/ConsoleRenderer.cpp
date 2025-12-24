#include "view/ConsoleRenderer.h"
#include <iostream>
#include <unistd.h>
#include <set>
#include <functional>
#include <iostream>
#include <sys/ioctl.h>

ConsoleRenderer::ConsoleRenderer(Position offset): 
    _offset(offset + Position(0, 1)),
    _playerSymbol("X "),
    _emptycellSymbol(". "),
    _teleportCellSymbol("T "),
    _bombCellSymbol("B ")
{
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
    _colorCodes[Color::DEFAULT] =_colorCodes.at(Color::DEFAULT);
    _colorCodes[Color::BLUEHIGHLIGHT] = "\033[44;37m";
    _colorCodes[Color::REDHIGHLIGHT] = "\033[41m";
    _colorCodes[Color::BLACK] = "\033[30m";
    _colorCodes[Color::GREENHIGHLIGHT] = "\033[42;37m";
}
std::string ConsoleRenderer::getColorCode(Color color) const {
    auto it = _colorCodes.find(color);
    if (it != _colorCodes.end()) {
        return it->second;
    }
    return _colorCodes.at(Color::DEFAULT);
}

void ConsoleRenderer::moveCursor(const Position& pos) const {
    int screenY = pos.getY() + 1;
    int screenX = pos.getX() + 1;
    
    if (screenY < 1) screenY = 1;
    if (screenX < 1) screenX = 1;
    
    std::cout << "\033[" << screenY << ";" << screenX << "H";
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
                      << cell.getValue() << " " << _colorCodes.at(Color::DEFAULT);
        } else {
            std::cout << "\033[47m" << colorCode << cell.getValue() << " " << _colorCodes.at(Color::DEFAULT);
        }
    } else {
        std::cout << "\033[47m\033[30m" << _emptycellSymbol << _colorCodes.at(Color::DEFAULT);
    }
}

void ConsoleRenderer::drawTeleportCell(const TeleportCell& cell, const Position& pos) const {
    moveCursor(pos);
    std::cout << _colorCodes.at(Color::GREENHIGHLIGHT) << "\033[1m" 
        << _teleportCellSymbol << _colorCodes.at(Color::DEFAULT);
}

void ConsoleRenderer::drawBombCell(const BombCell& cell, const Position& pos) const {
    moveCursor(pos);
    std::cout << _colorCodes.at(Color::REDHIGHLIGHT) << "\033[1m" 
        << _bombCellSymbol << _colorCodes.at(Color::DEFAULT);
}

void ConsoleRenderer::drawStartingState(const Grid& grid) {
    hideCursor();

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminalWidth = w.ws_col;
    int terminalHeight = w.ws_row;

    int gridWidth = grid.getWidth();
    int gridHeight = grid.getHeight();
    int visualWidth = gridWidth * 2;
    
    std::vector<std::vector<std::string>> bigNumbers = {
        {" ##  ", "###  ", " ##  ", " ##  ", " ##  ", " ##  ", "#### "},
        {" ####  ", "##  ## ", "   ##  ", "  ##   ", " ##    ", "##     ", "###### "},
        {" ####  ", "##  ## ", "    ## ", "  ###  ", "    ## ", "##  ## ", " ####  "},
        {"   ##   ", "  ###   ", " ## ##  ", "##  ##  ", "####### ", "   ##   ", "   ##   "},
        {"###### ", "##     ", "##     ", "#####  ", "    ## ", "##  ## ", " ####  "}
    };
    std::string colors[5] = {"\033[1;31m", "\033[1;32m", "\033[1;34m", "\033[1;33m", "\033[1;35m"};
     
    int maxLeftNumberWidth = 0;
    for (int i = 0; i < 5; i++) {
        for (const auto& row : bigNumbers[i]) {
            if (row.length() > maxLeftNumberWidth) {
                maxLeftNumberWidth = row.length();
            }
        }
    }
    
    int maxRightNumberWidth = 0;
    for (int i = 0; i < 5; i++) {
        for (const auto& row : bigNumbers[i]) {
            if (row.length() > maxRightNumberWidth) {
                maxRightNumberWidth = row.length();
            }
        }
    }
    
    int totalNeededWidth = visualWidth + maxLeftNumberWidth + maxRightNumberWidth + 4;
    if (totalNeededWidth > terminalWidth) {
    } else {
        for (int i = 0; i < 5; i++) {
            int digitY = _offset.getY() + (i * (gridHeight / 5));
            
            for (int row = 0; row < 7; row++) {
                int currentY = digitY + row;
                if (currentY < _offset.getY() + gridHeight && currentY >= _offset.getY()) {
                    Position numPos(_offset.getX() - bigNumbers[i][row].length() - 2, currentY);
                    if (numPos.getX() >= 0) {
                        moveCursor(numPos);
                        std::cout << colors[i] << bigNumbers[i][row] << _colorCodes.at(Color::DEFAULT);
                    }
                }
            }
        }
        
        for (int i = 0; i < 5; i++) {
            int digitY = _offset.getY() + (i * (gridHeight / 5));
            
            for (int row = 0; row < 7; row++) {
                int currentY = digitY + row;
                if (currentY < _offset.getY() + gridHeight && currentY >= _offset.getY()) {
                    Position numPos(_offset.getX() + visualWidth + 2, currentY);
                    if (numPos.getX() + bigNumbers[i][row].length() <= terminalWidth) {
                        moveCursor(numPos);
                        std::cout << colors[i] << bigNumbers[i][row] << _colorCodes.at(Color::DEFAULT);
                    }
                }
            }
        }
    }

    for (int col = -1; col <= visualWidth; col++) {
        Position borderPos(_offset.getX() + col, _offset.getY() - 1);
        moveCursor(borderPos);
        std::cout << "\033[40m \033[0m";
    }

    for (int row = 0; row < gridHeight; row++) {
        Position leftBorderPos(_offset.getX() - 1, _offset.getY() + row);
        moveCursor(leftBorderPos);
        std::cout << "\033[40m \033[0m";
        
        Position rightBorderPos(_offset.getX() + visualWidth, _offset.getY() + row);
        moveCursor(rightBorderPos);
        std::cout << "\033[40m \033[0m";
        
        for (int col = 0; col < gridWidth; col++) {
            const Position& cellPos = Position(col, row);
            const ICell& cell = grid[cellPos];
            
            Position drawPos = Position(_offset.getX() + col * 2, _offset.getY() + row);
            
            std::cout << "\033[47m";
            cell.acceptRender(*this, drawPos);
            std::cout <<_colorCodes.at(Color::DEFAULT);
        }
    }
    
    for (int col = -1; col <= visualWidth; col++) {
        Position bottomBorderPos(_offset.getX() + col, _offset.getY() + gridHeight);
        moveCursor(bottomBorderPos);
        std::cout << "\033[40m \033[0m";
    }
    
    int controlsY = _offset.getY() + gridHeight + 2;
    std::string controls = "\033[32mW/↑ A/← S/↓ D/→\033[0m Move  \033[33mP\033[0m Pause  \033[34mS\033[0m Save  \033[35mM\033[0m Menu  \033[36mESC\033[0m Exit";
    int controlsX = (terminalWidth - controls.length()) / 2;
    if (controlsX < 0) controlsX = 0;
    
    Position controlsPos(controlsX, controlsY);
    moveCursor(controlsPos);
    std::cout << controls;
    
    std::cout.flush();
}

void ConsoleRenderer::drawPlayer(const Position& playerPos) {
    Position drawPos = Position(_offset.getX() + playerPos.getX() * 2, _offset.getY() + playerPos.getY());
    moveCursor(drawPos);
    std::cout << _colorCodes.at(Color::DEFAULT) << _playerSymbol << _colorCodes.at(Color::DEFAULT);
    std::cout.flush();
}

void ConsoleRenderer::drawMove(const Grid& grid, const std::vector<Position>& affectedElements) {
    for (const Position& pos: affectedElements) {
        Position drawPos = Position(_offset.getX() + pos.getX() * 2, _offset.getY() + pos.getY());
        
        moveCursor(drawPos);
        std::cout << "\033[47m";
        grid[pos].acceptRender(*this, drawPos);
        std::cout <<_colorCodes.at(Color::DEFAULT);
    }

    std::cout.flush();
} 

void ConsoleRenderer::highlightMoveDirection(const Grid& grid, std::vector<std::pair<bool, Position>>& availableMoves, Direction direction) {
    for (std::pair<bool, Position> elem: availableMoves) {
        if (elem.first) {
            Position drawPos =  Position(_offset.getX() + elem.second.getX() * 2, _offset.getY() + elem.second.getY());
            moveCursor(drawPos);
            std::cout << "\033[47m";
            grid[elem.second].acceptRender(*this, drawPos);
            std::cout <<_colorCodes.at(Color::DEFAULT);
        }
    }
    
    int index = static_cast<int>(direction);
    if (index >= 0 && index < 4 && availableMoves[index].first) {
        Position highlightedCellPos = availableMoves[index].second;
        Position drawPos = Position(_offset.getX() + highlightedCellPos.getX() * 2, _offset.getY() + highlightedCellPos.getY());
        moveCursor(drawPos);
        grid[highlightedCellPos].acceptRender(*this, drawPos, Color::BLUEHIGHLIGHT); 
    }

    std::cout.flush();
}

void ConsoleRenderer::drawScoreAtPosition(int score, const Position& pos) const {
    moveCursor(pos);
    std::string scoreColor;
    if (score >= 0 && score <= 300) {
        scoreColor = "\033[1;31m";
    } else if (score >= 301 && score <= 700) {
        scoreColor = "\033[1;33m";
    } else {
        scoreColor = "\033[1;32m";
    }
    
    std::cout << "\033[1;36mScore: " << scoreColor << score << _colorCodes.at(Color::DEFAULT);
    std::cout.flush();
}

void ConsoleRenderer::clearScreen() const {
    std::cout << "\033[2J\033[1;1H";
    std::cout.flush();
}

void ConsoleRenderer::resetCursor() const {
    showCursor();
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    moveCursor(Position(0, w.ws_row - 1));
    std::cout.flush();
}

void ConsoleRenderer::displayWelcomeScreen() const {

    clearScreen();
    
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
        std::cout << std::string(padding, ' ') << "\033[1;35m" << line <<_colorCodes.at(Color::DEFAULT) << std::endl;
    }
    
    std::cout << std::endl;
    std::string subtitle = "A Colorful Number Jumping Adventure!";
    int subtitlePadding = (terminalWidth - subtitle.length()) / 2;
    std::cout << std::string(subtitlePadding, ' ') << "\033[1;36m" << subtitle <<_colorCodes.at(Color::DEFAULT) << std::endl << std::endl;
    
    std::cout << "\033[" << (terminalHeight - 2) << ";1H";
    std::cout << "\033[1;37mPress any key to continue...\033[0m";
    std::cout.flush();
}

void ConsoleRenderer::displayGameOver() const {
    clearScreen();
    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int terminalWidth = w.ws_col;
    int terminalHeight = w.ws_row;
    
    int verticalPadding = terminalHeight / 3;
    for (int i = 0; i < verticalPadding; i++) {
        std::cout << std::endl;
    }
    
    std::string title = "=== GAME OVER ===";
    int titlePadding = (terminalWidth - title.length()) / 2;
    std::cout << std::string(titlePadding, ' ') << "\033[1;31m" << title <<_colorCodes.at(Color::DEFAULT) << std::endl << std::endl;
    
    std::cout.flush();
}

void ConsoleRenderer::highlightGameOverState(const Grid& grid) {
    int gridWidth = grid.getWidth();
    int gridHeight = grid.getHeight();

    for (int row = 0; row < gridHeight; row++) {
        for (int col = 0; col < gridWidth; col++) {
            const Position& cellPos = Position(col, row);
            const ICell& cell = grid[cellPos];
            
            Position drawPos = Position(_offset.getX() + col * 2, _offset.getY() + row);
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

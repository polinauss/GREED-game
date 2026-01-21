#include "view/Settings.hpp"
#include <unistd.h>
#include <sys/ioctl.h>

Settings::Settings(int minWidth, int minHeight): _minWidth(52), _minHeight(27) {
    updateTerminalSize();
};

void Settings::updateTerminalSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    _terminalWidth = w.ws_col;
    _terminalHeight = w.ws_row;

    if (_terminalWidth < 80 || _terminalHeight < 30) {
        throw std::runtime_error("Terminal too small");
    }
}

Position Settings::calculateCenteringOffsets(int gridWidth, int gridHeight) {
    int totalWidth = gridWidth * 2 + 2;
    int totalHeight = gridHeight + 2;

    int offsetX = (_terminalWidth - totalWidth) / 2;
    int offsetY = (_terminalHeight - totalHeight) / 2+1;
    return Position(offsetX, offsetY);
}



#include "view/Settings.h"
#include <unistd.h>
#include <sys/ioctl.h>

Settings::Settings(int minWidth, int minHeight): _minWidth(minWidth), _minHeight(minHeight) {
    updateTerminalSize();
};

void Settings::updateTerminalSize() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    _terminalWidth = w.ws_col;
    _terminalHeight = w.ws_row;

    if (_terminalWidth < _minWidth || _terminalHeight < _minHeight) {
        throw std::runtime_error("Terminal size is too small)"); 
    }
}

Position Settings::calculateCenteringOffsets(int gridWidth, int gridHeight) {
    int offsetX = (_terminalWidth - gridWidth) / 2;
    int offsetY = (_terminalHeight - gridHeight) / 2;
    return Position(offsetX, offsetY);
}



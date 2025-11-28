#ifndef SETTINGS
#define SETTINGS

#include <iostream>
#include "model/Position.h"

class Settings {
private:
    int _terminalWidth;
    int _terminalHeight;
    int _minWidth;
    int _minHeight;

public:
    Settings(int width = 200, int height = 55);
    ~Settings() = default;

    void updateTerminalSize();
    Position calculateCenteringOffsets(int gridWith, int gridHeight);

};

#endif
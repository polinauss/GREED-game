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
    Settings(int width = 72, int height = 27);
    ~Settings() = default;

    void updateTerminalSize();
    Position calculateCenteringOffsets(int gridWith, int gridHeight);

};

#endif

#ifndef INPUTHANDLER
#define INPUTHANDLER

#include "core/Directions.h"
#include <termios.h>

class InputHandler {
private:
    struct termios _originalTermios;
    
public:
    InputHandler();
    ~InputHandler();
    
    void restoreTerminal();
    void setupGameMode();
    void setupMenuMode();
    
    char getMenuInput();
    char getAnyKey();
    Direction getDirectionFromInput();
};

#endif

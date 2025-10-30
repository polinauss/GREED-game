#ifndef INPUTHANDLER
#define INPUTHANDLER

#include "core/Directions.h"
#include <termios.h>

class InputHandler {
private:
    struct termios _originalTermios;
    bool _isNonCanonicalMode;

public:
    InputHandler();
    ~InputHandler();

    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;

    void enableCanonicalMode();
    void disableCanonicalMode();
    Direction getDirectionFromInput();

private:
    bool isValidDirection(char input) const;
    Direction convertToDirection(char input) const;
    bool handleEscapeSequence(Direction& result) const;
};


#endif

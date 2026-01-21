#include "controller/InputHandler.hpp"
#include <unistd.h>
#include <cstdio>

InputHandler::InputHandler(): _isNonCanonicalMode(false) {
    tcgetattr(STDIN_FILENO, &_originalTermios);
}

InputHandler::~InputHandler() {
    if (_isNonCanonicalMode) {
        disableCanonicalMode();
    }
}

void InputHandler::enableCanonicalMode() {
    struct termios newTermios = _originalTermios;

    newTermios.c_lflag &= ~(ICANON | ECHO);
    newTermios.c_cc[VMIN] = 1;
    newTermios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
    _isNonCanonicalMode = true;
}   

void InputHandler::disableCanonicalMode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &_originalTermios);
    _isNonCanonicalMode = false;
}

Direction InputHandler::getDirectionFromInput() {
    if (!_isNonCanonicalMode) {
        enableCanonicalMode();
    }

    char input;
    while (true) {
        ssize_t bytesRead = read(STDIN_FILENO, &input, 1);
        if (bytesRead > 0) {
            if (input == '\033') {
                Direction result;
                if (handleEscapeSequence(result)) {
                    return result;
                } 
            } 
            
            else if (isValidDirection(input)) {
                return convertToDirection(input);
            } 
            
            else if (input == ' ') {
                return Direction::NONE;
            }

        }
    }
}

bool InputHandler::isValidDirection(char input) const {
    return input == 'w' || input == 'W' || 
           input == 's' || input == 'S' || 
           input == 'a' || input == 'A' || 
           input == 'd' || input == 'D';
}

bool InputHandler::handleEscapeSequence(Direction& result) const {
    char seq[2];
    if (read(STDIN_FILENO, &seq[0], 1) > 0) {
        if (seq[0] == '[') {
            if (read(STDIN_FILENO, &seq[1], 1) > 0) {
                switch(seq[1]) {
                    case 'A':
                        result = Direction::UP;
                        return true;
                    case 'B':
                        result = Direction::DOWN;
                        return true;
                    case 'C':
                        result = Direction::RIGHT;
                        return true;
                    case 'D':
                        result = Direction::LEFT;
                        return true;
                }
            }
        }
    }
    return false;
}

Direction InputHandler::convertToDirection(char input) const {
    switch (input) {
        case 'w': case 'W':
            return Direction::UP;
        case 'a': case 'A':
            return Direction::LEFT;
        case 's': case 'S':
            return Direction::DOWN;
        case 'd': case 'D':
            return Direction::RIGHT;
        default:
            return Direction::NONE;
    }
}

#include "controller/InputHandler.h"
#include <iostream>
#include <unistd.h>
#include <termios.h>

InputHandler::InputHandler() {
    tcgetattr(STDIN_FILENO, &_originalTermios);
}

InputHandler::~InputHandler() {
    restoreTerminal();
}

void InputHandler::restoreTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &_originalTermios);
    std::cout << "\033[?25h" << std::flush;
}

void InputHandler::setupGameMode() {
    struct termios newt = _originalTermios;
    newt.c_lflag &= ~(ICANON | ECHO);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::cout << "\033[?25l" << std::flush;
}

void InputHandler::setupMenuMode() {
    struct termios newt = _originalTermios;
    newt.c_lflag &= ~ICANON;
    newt.c_lflag |= ECHO;
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    std::cout << "\033[?25l" << std::flush;
}

char InputHandler::getMenuInput() {
    setupMenuMode();
    char input;
    if (read(STDIN_FILENO, &input, 1) > 0) {
        return input;
    }
    return 0;
}

char InputHandler::getAnyKey() {
    setupMenuMode();
    char input;
    if (read(STDIN_FILENO, &input, 1) > 0) {
        return input;
    }
    return 0;
}

Direction InputHandler::getDirectionFromInput() {
    setupGameMode();
    
    char input;
    if (read(STDIN_FILENO, &input, 1) > 0) {
        
        if (input == '\033') {
            char seq[3];
            if (read(STDIN_FILENO, &seq[0], 1) > 0 && 
                read(STDIN_FILENO, &seq[1], 1) > 0) {
                if (seq[0] == '[') {
                    switch(seq[1]) {
                        case 'A': return Direction::UP;
                        case 'B': return Direction::DOWN;
                        case 'C': return Direction::RIGHT;
                        case 'D': return Direction::LEFT;
                    }
                }
            }
        }
        
        switch(input) {
            case 'w':
            case 'W': return Direction::UP;
            case 's':
            case 'S': return Direction::DOWN;
            case 'a':
            case 'A': return Direction::LEFT;
            case 'd':
            case 'D': return Direction::RIGHT;
        }
    }
    
    return Direction::NONE;
}

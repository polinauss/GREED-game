#include "controller/InputManager.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

InputManager::InputManager() : _nonBlockingMode(false) {
    tcgetattr(STDIN_FILENO, &_originalTermios);
}

InputManager::~InputManager() {
    restoreTerminal();
}

void InputManager::setNonBlockingMode(bool enable) {
    if (enable == _nonBlockingMode) return;
    
    if (enable) {
        struct termios newTermios = _originalTermios;
        newTermios.c_lflag &= ~(ICANON | ECHO);
        newTermios.c_cc[VMIN] = 0;
        newTermios.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
        
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    } else {
        restoreTerminal();
    }
    
    _nonBlockingMode = enable;
}

char InputManager::getCharNonBlocking() {
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) > 0) {
        return c;
    }
    return 0;
}

bool InputManager::hasInput() {
    fd_set readfds;
    struct timeval tv = {0, 0};
    
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);
    
    return select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0;
}

void InputManager::restoreTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &_originalTermios);
    
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    
    _nonBlockingMode = false;
}

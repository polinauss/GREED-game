#ifndef INPUTMANAGER
#define INPUTMANAGER

#include <termios.h>
#include <functional>
#include <map>

class InputManager {
private:
    struct termios _originalTermios;
    bool _nonBlockingMode;
    
public:
    InputManager();
    ~InputManager();
    
    void setNonBlockingMode(bool enable);
    char getCharNonBlocking();
    bool hasInput();
    
    void restoreTerminal();
};

#endif

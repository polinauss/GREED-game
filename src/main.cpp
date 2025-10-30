#include <iostream>
#include "model/GameModel.h"
#include "view/GameView.h"
#include "controller/GameController.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

void getTerminalSize(int& width, int& height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    
    width = w.ws_col;
    height = w.ws_row;
}
int main() {
    srand(time(NULL));
    int width, height;
    getTerminalSize(width, height);

    GameModel gameModel(width, height);
    GameView gameView(&gameModel);
    GameController gameController(&gameModel, &gameView);

    gameController.startGame();

    return 0;
}
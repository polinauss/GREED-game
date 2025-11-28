#include <iostream>
#include "model/GameModel.h"
#include "view/GameView.h"
#include "controller/GameController.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

int main() {
    try {
        GameModel gameModel;
        GameView gameView(&gameModel);
        GameController gameController(&gameModel, &gameView);

        gameController.startGame();

    } catch (std::exception& e) {
        std::cerr << "Error:" << e.what() << std::endl;
        return 1;
    }

    return 0;
}
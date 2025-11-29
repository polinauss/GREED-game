#include <iostream>
#include "model/GameModel.h"
#include "view/GameView.h"
#include "controller/GameController.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <csignal>
#include <ctime>
#include <cstdlib>

GameModel* g_model = nullptr;
GameView* g_view = nullptr;

void handleSignal(int signal) {
    if (signal == SIGINT) {
        std::cout << "\033[2J\033[1;1H";
        std::cout << "Game terminated by user" << std::endl;
        exit(0);
    }
}


int main() {

std::signal(SIGINT, handleSignal);
    
    try {
        GameModel model(70, 25);
        GameView view(&model);
        GameController controller(&model, &view);
        
        g_model = &model;
        g_view = &view;
        
        controller.startGame();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }



 //   try {
 //       GameModel gameModel;
 //       GameView gameView(&gameModel);
 //       GameController gameController(&gameModel, &gameView);

 //       gameController.startGame();

//  } catch (std::exception& e) {
//        std::cerr << "Error:" << e.what() << std::endl;
        return 1;
 //   }

    return 0;
}

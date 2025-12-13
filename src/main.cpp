#include <iostream>
#include "model/GameModel.h"
#include "view/GameView.h"
#include "controller/GameController.h"
#include "controller/MenuController.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

int main() {
    try {
        MenuController menuController;
        
        menuController.showWelcomeScreen();
        
        bool programRunning = true;
        
        while (programRunning) {
            if (menuController.runMainMenu()) {
                GameModel gameModel;
                GameView gameView(&gameModel);
                GameController gameController(&gameModel, &gameView);
                gameController.setSaveCallback([&menuController, &gameModel]() {
                    menuController.saveGame(&gameModel);
                });
                
                gameController.setMenuCallback([&]() {
                    return true;
                });
                
                gameController.startGame();
                
                if (gameModel.isGameOver() && !gameController.shouldReturnToMenu()) { menuController.addToLeaderboard(gameModel.getScore());
                    
                    system("clear");
                    std::cout << "\033[1;31m" << "GAME OVER!" << "\033[0m" << std::endl << std::endl;
                    std::cout << "Final Score: \033[1;36m" << gameModel.getScore() << "\033[0m" << std::endl << std::endl;
                    std::cout << "\033[1;37m" << "Press ENTER to return to menu..." << "\033[0m";
                    std::string dummy;
                    std::getline(std::cin, dummy);
                }
                if (gameController.shouldReturnToMenu()) {
                    gameController.resetReturnToMenu();
                }
            }
        }

    } catch (std::exception& e) {
        std::cerr << "Error:" << e.what() << std::endl;
        return 1;
    }

    return 0;
}

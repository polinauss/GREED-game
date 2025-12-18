#include <iostream>
#include <memory>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>
#include "model/GameModel.h"
#include "view/GameView.h"
#include "controller/GameController.h"
#include "controller/MenuController.h"
#include <cstddef>

int main() {

    std::cout << "\033[?7l";
    std::cout.flush();

    struct termios originalTermios;
    tcgetattr(STDIN_FILENO, &originalTermios);
    try {
        std::cout << "\033[?7l";
        std::cout.flush();
        MenuController menu;
        bool running = true;
        while (running) {
            bool startGame = menu.runMainMenu();
            
            if (startGame) {
                GameModel* model = new GameModel();
                GameView* view = new GameView(model);
                GameController* controller = new GameController(model, view);
                
                controller->setSaveCallback([&menu, model]() {
                    menu.saveGame(model);
                });
                
                controller->setMenuCallback([&]() {
                    return true;
                });
                
                controller->startGame();
                
                if (model->isGameOver()) {
                    menu.addToLeaderboard(model->getScore());
                }
                
                delete controller;
                delete view;
                delete model;
            } else {
                running = false;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        
        tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
    std::cout << "\033[?7h\033[2J\033[1;1H\033[?25h\033[0m";
        return 1;
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
    std::cout << "\033[?7h\033[2J\033[1;1H\033[?25h\033[0m";
    return 0;
}

#include <iostream>
#include <memory>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <termios.h>
#include "model/GameModel.h"
#include "view/GameView.h"
#include "controller/GameController.h"
#include "controller/MenuController.h"
#include <cstddef>

struct termios originalTermios;

void sigintHandler(int sig) {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
    std::cout << "\033[?7h\033[2J\033[1;1H\033[?25h\033[0m";
    system("clear");
    std::cout << "\033[1;36mGoodbye!\033[0m" << std::endl;
    std::exit(0);
}

int main() {
    tcgetattr(STDIN_FILENO, &originalTermios);
    std::signal(SIGINT, sigintHandler);
    std::cout << "\033[?7l";
    std::cout.flush();

    try {
        MenuController menu;
        
        while (true) {
            bool startGame = menu.runMainMenu();
            
            if (startGame) {
    GameModel* model = new GameModel();
    
    if (menu.hasSavedGame()) {
        std::cout << "\033[?1049l";
        std::cout << "Load saved game? (y/n): ";
        char choice;
        std::cin >> choice;
        
        if (choice == 'y' || choice == 'Y') {
            menu.loadGame(model);
        } else {
            model->initializeGame();
        }
    } else {
        model->initializeGame();
    }
                
                
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
                break;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
        std::cout << "\033[?7h\033[2J\033[1;1H\033[?25h\033[0m";
        system("clear");
        return 1;
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
    std::cout << "\033[?7h\033[2J\033[1;1H\033[?25h\033[0m";
    system("clear");
    return 0;
}

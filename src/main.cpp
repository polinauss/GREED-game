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
        
        menu.showWelcomeScreen();
        
        while (true) {
            bool startGame = menu.runMainMenu();
            
            if (startGame) {
                std::cout << "\033[?1049l";
                std::cout << "\033[2J\033[1;1H";
                std::cout.flush();
                
                GameModel* model = new GameModel();
                std::cout << "\033[?1049l\033[2J\033[1;1H";
                std::cout.flush();
                
                                int selectedOption = menu.getLastSelectedOption();
                
                if (selectedOption == 1 && menu.hasSavedGame()) {
                    bool loadSuccessful = menu.loadGame(model);
                    if (!loadSuccessful) {
                        std::cout << "\033[1;31mFailed to load saved game.\033[0m" << std::endl;
                        std::cout << "\033[1;33mStarting new game instead...\033[0m" << std::endl;
                        sleep(2);
                        model->initializeGame();
                    }
                } else {
                    model->initializeGame();
                }
                
                std::cout << "\033[?1049h\033[2J\033[1;1H";
                std::cout.flush();
                
                GameView* view = new GameView(model);
                GameController* controller = new GameController(model, view);
                
                controller->setSaveCallback([&menu, model]() {
                    menu.saveGame(model);
                });
                
                controller->setMenuCallback([&]() {
                    return true;
                });
                
                controller->startGame();
                
                if (model->isGameOver() && !controller->shouldReturnToMenu()) {
                    menu.addToLeaderboard(model->getScore());
                }
                
                std::cout << "\033[?1049l\033[2J\033[1;1H";
                std::cout.flush();
                
                delete controller;
                delete view;
                delete model;
                
            } else {
                break;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
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

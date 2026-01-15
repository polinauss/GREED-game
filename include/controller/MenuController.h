#ifndef MENUCONTROLLER
#define MENUCONTROLLER

#include "model/GameModel.h"
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstddef>

struct GameState {
    Position playerPosition;
    int score;
    int width;
    int height;
    std::vector<int> cellValues;
    std::vector<int> cellColors;
    std::vector<int> cellAvailable;
    
    std::vector<int> cellTypes;
    std::vector<int> teleportTargetsX;
    std::vector<int> teleportTargetsY;
    
    void serialize(std::ofstream& file) const;
    void deserialize(std::ifstream& file);
};

struct LeaderboardEntry {
    std::string playerName;
    int score;
    std::string date;
    
    bool operator>(const LeaderboardEntry& other) const {
        return score > other.score;
    }
};

class MenuController {
private:
    std::string _playerName;
    std::vector<LeaderboardEntry> _leaderboard;
    const std::string SAVE_FILE = "game_save.dat";
    const std::string LEADERBOARD_FILE = "leaderboard.txt";
    bool _hasSavedGame;
    int _lastSelectedOption;
    
    int _currentSelectedIndex;
    std::vector<std::string> _currentMenuItems;
    
    static MenuController* globalMenuController;

public:
    MenuController();
    ~MenuController() = default;
    void forceSetSelectedOption(int option) { _lastSelectedOption = option; }
    bool runMainMenu();
    int getLastSelectedOption() const { return _lastSelectedOption; }
    void showWelcomeScreen();
    void showRules();
    void setPlayerName();
    void showLeaderboard();
    
    void saveGame(GameModel* model);
    bool loadGame(GameModel* model);
    
    void addToLeaderboard(int score);
    const std::string& getPlayerName() const { return _playerName; }
    bool hasSavedGame() const { return _hasSavedGame; }
    
    void refreshMenu();

private:
    void drawAsciiTitle();
    void displayMenuItems(const std::vector<std::string>& items, int selectedIndex);
    void loadLeaderboard();
    void saveLeaderboard();
    
    static void handleResize(int sig);
  
    bool checkTerminalSize() const;
    void displaySizeError() const;
};

#endif

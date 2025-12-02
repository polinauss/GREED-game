#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
#include <map>
#include <fstream>

class Leaderboard {
private:
    std::map<std::string, int> _scores;
    std::string _filename = "scores.dat";
    
public:
    Leaderboard();
    
    void addScore(const std::string& playerName, int score);
    int getBestScore(const std::string& playerName) const;
    std::vector<std::pair<std::string, int>> getTopScores(int count = 5) const;
    
private:
    void save();
    void load();
    static bool compare(const std::pair<std::string, int>& a, 
                       const std::pair<std::string, int>& b);
};

#endif

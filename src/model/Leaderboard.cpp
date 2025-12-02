#include "model/Leaderboard.h"
#include <algorithm>
#include <iostream>

Leaderboard::Leaderboard() {
    load();
}

void Leaderboard::addScore(const std::string& playerName, int score) {
    auto it = _scores.find(playerName);
    if (it == _scores.end() || score > it->second) {
        _scores[playerName] = score;
        save();
    }
}

int Leaderboard::getBestScore(const std::string& playerName) const {
    auto it = _scores.find(playerName);
    return (it != _scores.end()) ? it->second : 0;
}

std::vector<std::pair<std::string, int>> Leaderboard::getTopScores(int count) const {
    std::vector<std::pair<std::string, int>> result;
    for (const auto& p : _scores) result.push_back(p);
    std::sort(result.begin(), result.end(), compare);
    if (result.size() > count) result.resize(count);
    return result;
}

bool Leaderboard::compare(const std::pair<std::string, int>& a, 
                         const std::pair<std::string, int>& b) {
    return a.second > b.second;
}

void Leaderboard::save() {
    std::ofstream file(_filename);
    if (file) {
        for (const auto& p : _scores) {
            file << p.first << " " << p.second << "\n";
        }
    }
}

void Leaderboard::load() {
    std::ifstream file(_filename);
    if (file) {
        std::string name;
        int score;
        while (file >> name >> score) {
            _scores[name] = score;
        }
    }
}

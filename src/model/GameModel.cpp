#include "model/GameModel.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <filesystem>

GameModel::GameModel(int width, int height): 
    _grid(width, height), 
    _player(Position(width / 2, height / 2)),
    _score(0),
    _gameOver(false),
    _isPaused(false),
    _playerName("Player"),
    _initialWidth(width),
    _initialHeight(height){
    initializeGame();
    updateGameState();
    loadLeaderboard();
}

void GameModel::initializeGame() {
    if (_grid.isValidPosition(_player.getPosition())) {
        _grid.removeCell(_player.getPosition());
    }
}

bool GameModel::isValidMove(Position position) const {
    if (!_grid.isValidPosition(position)) {
        return false;
    }
    const Cell& cell = _grid.getCell(position);
    return cell.isAvailable();
}

void GameModel::resetGame() {
    _score = 0;
    _gameOver = false;
    _isPaused = false;
    _availableMoves.clear();
    _grid = Grid(_initialWidth, _initialHeight);
    
    _player.setPosition(Position(_initialWidth / 2, _initialHeight / 2));
    
    initializeGame();
    updateGameState();
}


bool GameModel::makeMove(Direction direction) {
    if (_gameOver) {
        return false;
    }

    Position currentPos = _player.getPosition();
    Position finalPos = calculateNewPosition(currentPos, direction);

    if (!isValidMove(finalPos)) {
        return false;
    }

    _player.setPosition(finalPos);
    for (Position pos: makeOver(currentPos, finalPos)) {
        _grid.getCell(pos).setAvailable(false);
        _score++;
    }                         

    updateGameState();

    return true;
}

bool GameModel::isGameOver() const {
    return _gameOver;
}

int GameModel::getScore() const {
    return _score;
}

Grid& GameModel::getGrid() {
    return _grid;
}

const Grid& GameModel::getGrid() const {
    return _grid;
}

Position GameModel::getPlayerPosition() const {
    return _player.getPosition();
}

const std::vector<Position>& GameModel::getAvailableMoves(Direction direction) const {
    auto it = _availableMoves.find(direction);
    if (it != _availableMoves.end()) {
        return it->second;
    }
    static std::vector<Position> empty;
    return empty;
}

Position GameModel::calculateNewPosition(Position current, Direction direction) const {
    int newX = current.getX(), oldX = current.getX();
    int newY = current.getY(), oldY = current.getY();
    
    switch (direction) {
        case Direction::UP:
            newY -= 1;
            break;
        case Direction::DOWN:
            newY += 1;
            break;
        case Direction::LEFT:
            newX -= 1;
            break;
        case Direction::RIGHT:
            newX += 1;
            break;
        case Direction::NONE:
            return current;
    }
    
    Position interPos = Position(newX, newY);

    if (!isValidMove(interPos))
        return interPos;

    int moveValue = _grid.getCell(interPos).getValue();
    Position finalPos = Position(
        newX + (newX - oldX) * moveValue,
        newY + (newY - oldY) * moveValue
    );

    return finalPos;
}

void GameModel::updateGameState() {
    Position playerPos = _player.getPosition();
    bool hasValidMoves = false;
    
    Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    
    for (Direction dir : directions) {
        Position newPos = calculateNewPosition(playerPos, dir);
        if (isValidMove(newPos)) {
            hasValidMoves = true;
            _availableMoves[dir] = makeOver(playerPos, newPos);
        } else {
            _availableMoves[dir] = {};
        }
    }

    _availableMoves[Direction::NONE] = {};
    _gameOver = !hasValidMoves;
    
    std::cout << "updateGameState: hasValidMoves = " << hasValidMoves << ", _gameOver = " << _gameOver << std::endl;
    if (!hasValidMoves) {
        std::cout << "=== NO VALID MOVES DETECTED - GAME SHOULD END ===" << std::endl;
    }
}

std::vector<Position> GameModel::makeOver(Position current, Position target) const {
    std::vector<Position> jumpedOver;

    int dx = target.getX() - current.getX();
    int dy = target.getY() - current.getY();
    int stepX = (dx > 0 ? 1 : -1) * static_cast<int>(dx != 0);
    int stepY = (dy > 0 ? 1 : -1) * static_cast<int>(dy != 0);

    int distance = std::max(std::abs(dx), std::abs(dy));
    
    for (int i = 1; i <= distance; i++) {
        jumpedOver.emplace_back(Position(
            current.getX() + i * stepX,
            current.getY() + i * stepY
        ));
    }
    
    return jumpedOver;
}

void GameModel::setPlayerName(const std::string& name) {
    _playerName = name;
}

std::string GameModel::getPlayerName() const {
    return _playerName;
}

bool GameModel::saveGame(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot create save file: " << filename << std::endl;
        return false;
    }

    try {
       
        file << _score << std::endl;
        file << _gameOver << std::endl;
        file << _isPaused << std::endl;
        file << _playerName << std::endl;
        
        Position playerPos = _player.getPosition();
        file << playerPos.getX() << " " << playerPos.getY() << std::endl;
        
        file << _grid.getWidth() << " " << _grid.getHeight() << std::endl;
        
        for (int y = 0; y < _grid.getHeight(); y++) {
            for (int x = 0; x < _grid.getWidth(); x++) {
                Position pos(x, y);
                const Cell& cell = _grid.getCell(pos);
                file << cell.getValue() << " " << static_cast<int>(cell.getColor()) << " " << cell.isAvailable() << " ";
            }
            file << std::endl;
        }

        file.close();
        
        const_cast<GameModel*>(this)->saveLeaderboard();
        std::cout << "Game saved successfully to: " << filename << std::endl;
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Error saving game: " << e.what() << std::endl;
        file.close();
        return false;
    }
}

bool GameModel::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No saved game found at: " << filename << std::endl;
        return false;
    }

    try {

        file >> _score;
        file >> _gameOver;
        file >> _isPaused;
        file.ignore();
        std::getline(file, _playerName);
        
        std::cout << "Loaded - Score: " << _score << ", GameOver: " << _gameOver 
                  << ", Player: " << _playerName << std::endl;
        
        int x, y;
        file >> x >> y;
        _player.setPosition(Position(x, y));
        std::cout << "Player position: " << x << ", " << y << std::endl;
        
        int width, height;
        file >> width >> height;
        std::cout << "Grid size: " << width << "x" << height << std::endl;
        
        if (width != _grid.getWidth() || height != _grid.getHeight()) {
            _grid = Grid(width, height);
        }
        
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int value, colorInt;
                bool available;
                file >> value >> colorInt >> available;
                
                Position pos(x, y);
                Cell& cell = _grid.getCell(pos);
                cell.setValue(value);
                cell.setAvailable(available);
            }
        }

        file.close();
        
        updateGameState();
        std::cout << "Game state updated successfully" << std::endl;
        
        return true;
    } 
    catch (const std::exception& e) {
        std::cout << "Error loading game: " << e.what() << std::endl;
        file.close();
        return false;
    }
}

void GameModel::loadLeaderboard() {
    std::ifstream file(_leaderboardFile);
    if (!file.is_open()) {
        std::cout << "No existing leaderboard found. Creating new one." << std::endl;
        return;
    }

    try {
        _leaderboard.clear();
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string name;
            int score;
            if (iss >> name >> score) {
                _leaderboard.emplace_back(name, score);
                std::cout << "Loaded record: " << name << " - " << score << std::endl;
            }
        }
        file.close();

        std::sort(_leaderboard.begin(), _leaderboard.end(), 
            [](const PlayerRecord& a, const PlayerRecord& b) {
                return a.bestScore > b.bestScore;
            });
            
        std::cout << "Leaderboard loaded with " << _leaderboard.size() << " records" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error loading leaderboard: " << e.what() << std::endl;
    }
}

void GameModel::saveLeaderboard() {
    std::ofstream file(_leaderboardFile);
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot create leaderboard file: " << _leaderboardFile << std::endl;
        return;
    }

    try {
        for (const auto& record : _leaderboard) {
            file << record.name << " " << record.bestScore << std::endl;
            std::cout << "Saved record: " << record.name << " - " << record.bestScore << std::endl;
        }
        file.close();
        std::cout << "Leaderboard saved successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error saving leaderboard: " << e.what() << std::endl;
    }
}

void GameModel::updatePlayerScore() {
    std::cout << "=== updatePlayerScore called ===" << std::endl;
    std::cout << "Current score: " << _score << std::endl;
    std::cout << "Player name: " << _playerName << std::endl;
    

    auto it = std::find_if(_leaderboard.begin(), _leaderboard.end(),
        [this](const PlayerRecord& record) {
            return record.name == _playerName;
        });
    
    if (it != _leaderboard.end()) {
        std::cout << "Found existing player in leaderboard. Old score: " << it->bestScore << ", New score: " << _score << std::endl;
        
        if (_score > it->bestScore) {
            it->bestScore = _score;
            std::cout << "Updating to new high score!" << std::endl;
        } else {
            std::cout << "Existing score is better, keeping old score." << std::endl;
        }
    } else {
        std::cout << "Adding new player to leaderboard." << std::endl;
        _leaderboard.emplace_back(_playerName, _score);
    }
    
    std::sort(_leaderboard.begin(), _leaderboard.end(), 
        [](const PlayerRecord& a, const PlayerRecord& b) {
            return a.bestScore > b.bestScore;
        });
    
    std::cout << "Leaderboard now has " << _leaderboard.size() << " records" << std::endl;
    saveLeaderboard();
    std::cout << "=== updatePlayerScore finished ===" << std::endl;
}

std::vector<PlayerRecord> GameModel::getLeaderboard() const {
    return _leaderboard;
}


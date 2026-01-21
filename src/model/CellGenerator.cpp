#include "model/CellGenerator.hpp"
#include "model/cells/BasicCell.hpp"
#include "model/cells/BombCell.hpp"
#include "model/cells/TeleportCell.hpp"
#include <cmath>
#include <ctime>
#include <cstdlib>

Color CellGenerator::getColor(int value) {
    Color color;

    switch(value) {
        case 1:
            color = Color::RED;
            break;
        case 2:
            color = Color::GREEN;
            break;
        case 3:
            color = Color::BLUE;
            break;
        case 4:
            color = Color::YELLOW;
            break;
        case 5:
            color = Color::MAGENTA;
            break;
        default:
            color = Color::CYAN;
    }

    return color;
}

std::vector<ICell*> CellGenerator::generateRandomGrid(int width, int height) {
    srand(time(NULL));
    
    std::vector<ICell*> grid;
    grid.reserve(width * height);
    
    float centerX = (width - 1) / 2.0f;
    float centerY = (height - 1) / 2.0f;
    float maxDistance = sqrt(centerX * centerX + centerY * centerY);

    int bombCellsCount = 0;
    int teleportCellsCount = 0;
    int totalCells = width * height;
    
    int maxBombCells = totalCells / 12;
    int maxTeleportCells = totalCells / 12;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float dx = x - centerX;
            float dy = y - centerY;
            float distance = sqrt(dx * dx + dy * dy);
            
            float normalizedDistance = distance / maxDistance;
            float invertedDistance = 1.0f - normalizedDistance * normalizedDistance;
            
            int value;
            if (invertedDistance > 0.8f) {
                value = 4 + rand() % 2;
            } else if (invertedDistance > 0.5f) {
                value = 3 + rand() % 2;
            } else if (invertedDistance > 0.2f) {
                value = 2 + rand() % 2;
            } else {
                value = 1 + rand() % 2;
            }
            
            bool createSpecialCell = false;
            int cellType = 0; 
            
            if (bombCellsCount < maxBombCells && rand() % 12 == 0) {
                createSpecialCell = true;
                cellType = 1;
                bombCellsCount++;
            } 
            else if (teleportCellsCount < maxTeleportCells && rand() % 15 == 0) {
                createSpecialCell = true;
                cellType = 2;
                teleportCellsCount++;
            }
            
            if (createSpecialCell) {
                if (cellType == 1) {
                    grid.push_back(new BombCell());
                } 
                else if (cellType == 2) {
                    int targetX = rand() % width;
                    int targetY = rand() % height;
                    
                    if (targetX == x && targetY == y) {
                        targetX = (x + 1) % width;
                    }
                    
                    grid.push_back(new TeleportCell(Position(targetX, targetY)));
                }
            } 
            else {
                Color color = getColor(value);
                grid.push_back(new BasicCell(value, color));
            }
        }
    }
    
    return grid;
}

ICell* CellGenerator::createBasicCell(int value) {
    Color color = getColor(value);
    return new BasicCell(value, color);
}

ICell* CellGenerator::createRandomCell() {
    int value = rand() % 5 + 1;
    Color color = getColor(value);
    return new BasicCell(value, color);
}
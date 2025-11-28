#include "model/CellGenerator.h"
#include "model/cells/BasicCell.h"
#include <ctime>
#include <cstdlib>

std::vector<ICell*> CellGenerator::generateRandomGrid(int width, int height) {
    srand(time(NULL));

    std::vector<ICell*> grid;
    grid.reserve(width * height);

    for (int i = 0; i < width * height; i++) {
        grid.push_back(createRandomCell());
    }

    return grid;
}

ICell* CellGenerator::createBasicCell(int value, Color color) {
    return new BasicCell(value, color);
}

ICell* CellGenerator::createRandomCell() {
    int value = rand() % 9 + 1;
    Color color = gameColors[rand() % gameColors.size()];
    return new BasicCell(value, color);
}
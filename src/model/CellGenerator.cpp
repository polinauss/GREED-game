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
    int value = rand() % 5 + 1;
    
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
    
    return new BasicCell(value, color);
}

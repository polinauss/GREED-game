#ifndef CELLGENERATOR
#define CELLGENERATOR

#include "interfaces/ICell.h"
#include "core/Color.h"
#include <vector>

class CellGenerator {
public:
    CellGenerator() = default;
    ~CellGenerator() = default;

    std::vector<ICell*> generateRandomGrid(int width, int height);

    ICell* createBasicCell(int value, Color color);
    ICell* createRandomCell();
};

#endif
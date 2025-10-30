#ifndef GRID
#define GRID

#include "core/Color.h"
#include "model/Cell.h"
#include "model/Position.h"
#include <vector>

class Grid {
private:
    std::vector<Cell> _cells;
    int _width;
    int _height;

public:
    Grid(int width, int height);
    ~Grid() = default;

    void InitializeRandom();
    Cell& getCell(Position position);
    const Cell& getCell(Position position) const;
    bool isValidPosition(Position position) const;
    void removeCell(Position position);
    int getWidth() const;
    int getHeight() const;
};

#endif
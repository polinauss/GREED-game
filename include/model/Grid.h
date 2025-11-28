#ifndef GRID
#define GRID

#include "core/Color.h"
#include "model/CellGenerator.h"
#include "interfaces/ICell.h"
#include "model/Position.h"
#include <vector>

class Grid {
private:
    std::vector<ICell*> _cells;
    int _width;
    int _height;
    CellGenerator _generator;

public:
    Grid(int width, int height);
    ~Grid();

    void initializeRandom();

    ICell& operator[] (const Position& position);
    const ICell& operator[] (const Position& position) const;
    bool isValidPosition(const Position& position) const;
    void removeCell(const Position& position);
    int getWidth() const;
    int getHeight() const;

private:
    void clearCells();
};

#endif

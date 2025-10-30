#ifndef CELL
#define CELL

#include "core/Color.h"

class Cell {
private:
    int _value;
    Color _color;
    bool _available;

public:
    Cell(int value=0, Color color=Color::DEFAULT);
    ~Cell() = default;

    int getValue() const;
    void setValue(int value);
    Color getColor() const;
    bool isAvailable() const;
    void setAvailable(bool available);
};

#endif
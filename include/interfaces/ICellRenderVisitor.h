#ifndef CELLRENDERVISITOR
#define CELLRENDERVISITOR

#include "model/Position.h"

class BasicCell;
class Position;

class ICellRenderVisitor {
public:
    virtual ~ICellRenderVisitor() = default;

    virtual void drawBasicCell(const BasicCell& cell, const Position& pos, Color highlightColor = Color::DEFAULT) const = 0;
};

#endif

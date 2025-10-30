#ifndef PLAYER
#define PLAYER

#include "model/Position.h"

class Player {
private:
    Position _position;

public:
    Player(Position startPosition);
    ~Player() = default;

    Position getPosition() const;
    void setPosition(Position position);
};

#endif
#include "model/Player.hpp"
#include <iostream>

Player::Player(Position startPosition): _position(startPosition) {}

Position Player::getPosition() const {
    return _position;
}

void Player::setPosition(Position position) {
    _position = position;
}
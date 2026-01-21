/**
 * @file Player.hpp
 * @brief Заголовочный файл, содержащий объявление класса Player
 */
#ifndef PLAYER
#define PLAYER

#include "model/Position.hpp"

/**
 * @brief Класс игрока
 * 
 * Отвечает за хранение и управление позицией игрока на игровом поле.
 */
class Player {
private:
    Position _position; /**< Текущая позиция игрока на поле */

public:
    /**
     * @brief Конструктор игрока
     * @param startPosition Начальная позиция игрока
     */
    Player(Position startPosition);
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~Player() = default;

    /**
     * @brief Возвращает текущую позицию игрока
     * @return Позиция игрока
     */
    Position getPosition() const;
    
    /**
     * @brief Устанавливает новую позицию игрока
     * @param position Новая позиция
     */
    void setPosition(Position position);
};

#endif
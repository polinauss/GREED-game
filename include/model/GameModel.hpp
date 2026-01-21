/**
 * @file GameModel.hpp
 * @brief Заголовочный файл, содержащий объявление класса GameModel
 */
#ifndef GAMEMODEL
#define GAMEMODEL

#include "model/Grid.hpp"
#include "model/Player.hpp"
#include "model/Position.hpp"
#include "model/InteractionHandler.hpp"
#include "interfaces/ICell.hpp"
#include "model/cells/BasicCell.hpp"
#include "model/cells/TeleportCell.hpp"
#include "model/cells/BombCell.hpp"
#include "core/Directions.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <cstddef>

/**
 * @brief Основная модель игры
 * 
 * Содержит игровое состояние: поле, игрока, счет, доступные ходы.
 * Управляет игровой логикой и взаимодействиями.
 */
class GameModel {
private:
    Grid _grid;                                /**< Игровое поле */
    Player _player;                            /**< Игрок */
    int _score;                                /**< Текущий счет */
    bool _gameOver;                            /**< Флаг завершения игры */
    std::vector<std::pair<bool, Position>> _availableMoves; /**< Доступные ходы (возможность + позиция) */
    InteractionHandler _interactionHandler;    /**< Обработчик взаимодействий */

public:
    /**
     * @brief Конструктор модели игры
     * @param width Ширина поля (по умолчанию 25)
     * @param height Высота поля (по умолчанию 25)
     */
    GameModel(int width=25, int height=25);
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~GameModel() = default;

    /**
     * @brief Объявление дружественного класса для доступа к приватным членам
     */
    friend class InteractionHandler;

    /**
     * @brief Инициализирует новую игру
     */
    void initializeGame();
    
    /**
     * @brief Инициализирует игру из сохраненного состояния
     * @param cellValues Значения клеток
     * @param cellColors Цвета клеток
     * @param cellAvailable Флаги доступности клеток
     * @param cellTypes Типы клеток
     * @param teleportTargetsX X-координаты целей телепортов
     * @param teleportTargetsY Y-координаты целей телепортов
     * @param playerPos Позиция игрока
     * @param score Начальный счет
     */
    void initializeGameFromState(const std::vector<int>& cellValues, 
                            const std::vector<int>& cellColors,
                            const std::vector<int>& cellAvailable,
                            const std::vector<int>& cellTypes,
                            const std::vector<int>& teleportTargetsX,
                            const std::vector<int>& teleportTargetsY,
                            const Position& playerPos,
                            int score);
    
    /**
     * @brief Проверяет валидность хода
     * @param position Целевая позиция
     * @return true если ход возможен, false в противном случае
     */
    bool isValidMove(Position position) const;
    
    /**
     * @brief Выполняет ход в указанном направлении
     * @param direction Направление движения
     */
    void makeMove(Direction direction);
    
    /**
     * @brief Проверяет завершена ли игра
     * @return true если игра завершена, false в противном случае
     */
    bool isGameOver() const;
    
    /**
     * @brief Возвращает текущий счет
     * @return Значение счета
     */
    int getScore() const;
    
    /**
     * @brief Возвращает ссылку на игровое поле
     * @return Ссылка на поле
     */
    Grid& getGrid();
    
    /**
     * @brief Возвращает константную ссылку на игровое поле
     * @return Константная ссылка на поле
     */
    const Grid& getGrid() const;
    
    /**
     * @brief Возвращает позицию игрока
     * @return Позиция игрока
     */
    Position getPlayerPosition() const;
    
    /**
     * @brief Возвращает элементы, затронутые последним ходом
     * @return Вектор позиций затронутых элементов
     */
    const std::vector<Position>& getAffectedElements() const;
    
    /**
     * @brief Возвращает доступные ходы
     * @return Вектор пар (возможность хода, позиция)
     */
    std::vector<std::pair<bool, Position>>& getAvailableMoves();

private:
    /**
     * @brief Обновляет состояние игры после хода
     * @note Проверяет доступные ходы и статус завершения игры
     */
    void updateGameState();
};

#endif
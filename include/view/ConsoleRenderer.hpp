/**
 * @file ConsoleRenderer.hpp
 * @brief Заголовочный файл, содержащий объявление класса ConsoleRenderer
 */
#ifndef CONSOLERENDERER
#define CONSOLERENDERER

#include "model/Grid.hpp"
#include "model/Player.hpp"
#include "model/Position.hpp"
#include "model/cells/BasicCell.hpp"
#include "model/cells/TeleportCell.hpp"
#include "model/cells/BombCell.hpp"
#include "core/Directions.hpp"
#include "interfaces/ICellRenderVisitor.hpp"
#include "core/Color.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstddef>

/**
 * @brief Рендерер для консольного отображения игры
 * 
 * Реализует отрисовку всех элементов игры в терминале,
 * поддерживает цвета, позиционирование курсора и различные состояния отображения.
 */
class ConsoleRenderer: public ICellRenderVisitor{
private:
    std::map<Color, std::string> _colorCodes; /**< Карта соответствия цветов и ANSI-кодов */

    std::string _playerSymbol;      /**< Символ для отображения игрока */
    std::string _emptycellSymbol;   /**< Символ для пустой клетки */
    std::string _teleportCellSymbol; /**< Символ для клетки-телепорта */
    std::string _bombCellSymbol;    /**< Символ для клетки-бомбы */
    Position _offset;               /**< Смещение для центрирования отображения */

public:
    /**
     * @brief Конструктор рендерера
     * @param offset Начальное смещение для отрисовки
     */
    ConsoleRenderer(Position offset);
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~ConsoleRenderer() = default;

    /**
     * @brief Отрисовывает базовую клетку
     * @param cell Ссылка на базовую клетку
     * @param pos Позиция для отрисовки
     * @param highlightColor Цвет выделения (по умолчанию стандартный)
     * @override
     */
    void drawBasicCell(const BasicCell& cell, const Position& pos, Color highlightColor = Color::DEFAULT) const override;
    
    /**
     * @brief Отрисовывает клетку-телепорт
     * @param cell Ссылка на клетку-телепорт
     * @param pos Позиция для отрисовки
     * @override
     */
    void drawTeleportCell(const TeleportCell& cell, const Position& pos) const override;
    
    /**
     * @brief Отрисовывает клетку-бомбу
     * @param cell Ссылка на клетку-бомбу
     * @param pos Позиция для отрисовки
     * @override
     */
    void drawBombCell(const BombCell& cell, const Position& pos) const override;

    /**
     * @brief Отрисовывает начальное состояние игрового поля
     * @param grid Ссылка на игровое поле
     */
    void drawStartingState(const Grid& grid);
    
    /**
     * @brief Отрисовывает игрока
     * @param playerPos Позиция игрока
     */
    void drawPlayer(const Position& playerPos);
    
    /**
     * @brief Отрисовывает ход игрока
     * @param grid Ссылка на игровое поле
     * @param affectedElements Элементы, затронутые ходом
     */
    void drawMove(const Grid& grid, const std::vector<Position>& affectedElements);
    
    /**
     * @brief Подсвечивает возможные направления движения
     * @param grid Ссылка на игровое поле
     * @param availableMoves Доступные ходы
     * @param direction Направление для подсветки
     */
    void highlightMoveDirection(const Grid& grid, std::vector<std::pair<bool, Position>>& availableMoves, Direction direction);
    
    /**
     * @brief Подсвечивает состояние завершения игры
     * @param grid Ссылка на игровое поле
     */
    void highlightGameOverState(const Grid& grid);

    /**
     * @brief Очищает экран терминала
     */
    void clearScreen() const;
    
    /**
     * @brief Сбрасывает позицию курсора в начало
     */
    void resetCursor() const;

    /**
     * @brief Отрисовывает счет игры
     * @param score Текущий счет
     */
    void drawScore(int score) const;
    
    /**
     * @brief Отображает экран приветствия
     */
    void displayWelcomeScreen() const;
    
    /**
     * @brief Отображает экран завершения игры
     */
    void displayGameOver() const;
    
    /**
     * @brief Отображает меню
     * @param menuItems Пункты меню
     * @param selectedIndex Индекс выбранного пункта
     * @param playerName Имя текущего игрока
     */
    void displayMenu(const std::vector<std::string>& menuItems, int selectedIndex, const std::string& playerName) const;
    
    /**
     * @brief Отрисовывает счет в указанной позиции
     * @param score Текущий счет
     * @param pos Позиция для отрисовки счета
     */
    void drawScoreAtPosition(int score, const Position& pos) const;

private:
    /**
     * @brief Инициализирует карту цветовых кодов
     * @note Заполняет карту ANSI-кодами для каждого цвета
     */
    void initializeColorCodes();
    
    /**
     * @brief Возвращает ANSI-код для указанного цвета
     * @param color Цвет
     * @return ANSI-строка с кодом цвета
     */
    std::string getColorCode(Color color) const;
    
    /**
     * @brief Перемещает курсор в указанную позицию
     * @param pos Целевая позиция курсора
     */
    void moveCursor(const Position& pos) const;
    
    /**
     * @brief Скрывает курсор терминала
     */
    void hideCursor() const;
    
    /**
     * @brief Показывает курсор терминала
     */
    void showCursor() const;
};

#endif
/**
 * @file InteractionHandler.hpp
 * @brief Заголовочный файл, содержащий объявление класса InteractionHandler
 */
#ifndef INTERACTIONHANDLER
#define INTERACTIONHANDLER

#include "interfaces/ICellInteractionVisitor.hpp"
#include "model/Position.hpp"
#include "core/Directions.hpp"
#include <vector>

class GameModel;

/**
 * @brief Обработчик взаимодействий с клетками
 * 
 * Реализует паттерн "Посетитель" для обработки взаимодействий игрока
 * с различными типами клеток.
 */
class InteractionHandler: public ICellInteractionVisitor {
private:
    GameModel& _model; /**< Ссылка на модель игры */
    std::vector<Position> _prevMoveAffectedElements; /**< Элементы, затронутые предыдущим ходом */
    Position _lastFinalPos; /**< Последняя конечная позиция после хода */

public:
    /**
     * @brief Конструктор обработчика взаимодействий
     * @param model Ссылка на модель игры
     */
    explicit InteractionHandler(GameModel& model);
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~InteractionHandler() = default;

    /**
     * @brief Обрабатывает столкновение с базовой клеткой
     * @param cell Ссылка на базовую клетку
     * @return Результат столкновения
     * @override
     */
    int collideWithBasicCell(BasicCell& cell) override;
    
    /**
     * @brief Обрабатывает наступание на базовую клетку
     * @param cell Ссылка на базовую клетку
     * @param cellPos Позиция клетки
     * @return Итоговая позиция после наступания
     * @override
     */
    Position stepOnBasicCell(BasicCell& cell, const Position& cellPos) override;
    
    /**
     * @brief Обрабатывает завершение наступания на базовую клетку
     * @param startPos Начальная позиция
     * @param finalPos Конечная позиция
     * @override
     */
    void handleStepOnBasicCell(const Position& startPos, const Position& finalPos) override;

    /**
     * @brief Обрабатывает столкновение с клеткой-телепортом
     * @param cell Ссылка на клетку-телепорт
     * @return Результат столкновения
     * @override
     */
    int collideWithTeleportCell(TeleportCell& cell) override;
    
    /**
     * @brief Обрабатывает наступание на клетку-телепорт
     * @param cell Ссылка на клетку-телепорт
     * @param cellPos Позиция клетки
     * @override
     */
    void stepOnTeleportCell(TeleportCell& cell, const Position& cellPos) override;

    /**
     * @brief Обрабатывает столкновение с клеткой-бомбой
     * @param cell Ссылка на клетку-бомбу
     * @return Результат столкновения
     * @override
     */
    int collideWithBombCell(BombCell& cell) override;
    
    /**
     * @brief Обрабатывает наступание на клетку-бомбу
     * @param cell Ссылка на клетку-бомбу
     * @param cellPos Позиция клетки
     * @override
     */
    void stepOnBombCell(BombCell& cell, const Position& cellPos) override;

    /**
     * @brief Возвращает элементы, затронутые предыдущим ходом
     * @return Вектор позиций затронутых элементов
     */
    const std::vector<Position>& getAffectedElements() const;
    
    /**
     * @brief Возвращает последнюю конечную позицию
     * @return Позиция после последнего хода
     */
    Position getLastFinalPos() const { return _lastFinalPos; }

private:
    /**
     * @brief Определяет клетки, через которые проходит движение
     * @param start Начальная позиция
     * @param finish Конечная позиция
     * @return Вектор позиций клеток на пути движения
     */
    std::vector<Position> makeOver(const Position& start, const Position& finish) const;
};

#endif
/**
 * @file ICell.h
 * @brief Заголовочный файл, содержащий объявление интерфейса ICell
 */
#ifndef CELL
#define CELL

#include "core/Color.hpp"
#include "core/Directions.hpp"
#include "model/Position.hpp"

class ICellRenderVisitor;
class ICellInteractionVisitor;

/**
 * @brief Базовый интерфейс для всех типов клеток
 * 
 * Определяет общий интерфейс для клеток игрового поля.
 * Реализует паттерн "Посетитель" для двойной диспетчеризации.
 */
class ICell {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~ICell() = default;

    /**
     * @brief Проверяет доступность клетки
     * @return true если клетка доступна для перемещения, false в противном случае
     */
    virtual bool isAvailable() const = 0;
    
    /**
     * @brief Устанавливает доступность клетки
     * @param available true - сделать клетку доступной, false - заблокировать
     */
    virtual void setAvailable(bool available) = 0;

    /**
     * @brief Принимает посетителя для отрисовки
     * @param visitor Посетитель для отрисовки
     * @param pos Позиция клетки
     * @param highlightColor Цвет выделения (по умолчанию стандартный)
     */
    virtual void acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor = Color::DEFAULT) const = 0;
    
    /**
     * @brief Принимает посетителя для обработки столкновения
     * @param visitor Посетитель для взаимодействия
     * @return Результат столкновения (например, изменение счета)
     */
    virtual int acceptInteractionColission(ICellInteractionVisitor& visitor) = 0;
    
    /**
     * @brief Принимает посетителя для обработки наступания
     * @param visitor Посетитель для взаимодействия
     * @param position Позиция наступания
     */
    virtual void acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) = 0;
};

#endif
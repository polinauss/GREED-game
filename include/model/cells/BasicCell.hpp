/**
 * @file BasicCell.hpp
 * @brief Заголовочный файл, содержащий объявление класса BasicCell
 */
#ifndef BASICCELL
#define BASICCELL

#include "interfaces/ICell.hpp"

/**
 * @brief Базовая клетка игрового поля
 * 
 * Основной тип клетки, имеющий числовое значение и цвет.
 * Игрок набирает очки при наступании на эту клетку.
 */
class BasicCell : public ICell {
private:
    int _value;       /**< Числовое значение клетки */
    Color _color;     /**< Цвет клетки для отображения */
    bool _isAvailable; /**< Флаг доступности клетки */

public:
    /**
     * @brief Конструктор базовой клетки
     * @param value Числовое значение клетки
     * @param color Цвет клетки
     */
    BasicCell(int value, Color color);
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~BasicCell() = default;

    /**
     * @brief Проверяет доступность клетки
     * @return true если клетка доступна для перемещения, false в противном случае
     * @override
     */
    bool isAvailable() const override;
    
    /**
     * @brief Устанавливает доступность клетки
     * @param available true - сделать клетку доступной, false - заблокировать
     * @override
     */
    void setAvailable(bool available) override;

    /**
     * @brief Принимает посетителя для отрисовки
     * @param visitor Посетитель для отрисовки
     * @param pos Позиция клетки
     * @param highlightColor Цвет выделения (по умолчанию стандартный)
     * @override
     */
    void acceptRender(ICellRenderVisitor& visitor, const Position& pos, Color highlightColor = Color::DEFAULT) const override;
    
    /**
     * @brief Принимает посетителя для обработки столкновения
     * @param visitor Посетитель для взаимодействия
     * @return Результат столкновения (значение клетки как очки)
     * @override
     */
    int acceptInteractionColission(ICellInteractionVisitor& visitor) override;
    
    /**
     * @brief Принимает посетителя для обработки наступания
     * @param visitor Посетитель для взаимодействия
     * @param position Позиция наступания
     * @override
     */
    void acceptInteractionStepOn(ICellInteractionVisitor& visitor, const Position& position) override;

    /**
     * @brief Возвращает цвет клетки
     * @return Цвет клетки
     */
    Color getColor() const;
    
    /**
     * @brief Возвращает значение клетки
     * @return Числовое значение клетки
     */
    int getValue() const;
};

#endif
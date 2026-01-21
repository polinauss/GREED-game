/**
 * @file BombCell.hpp
 * @brief Заголовочный файл, содержащий объявление класса BombCell
 */
#ifndef BOMBCELL
#define BOMBCELL

#include "interfaces/ICell.hpp"

/**
 * @brief Клетка-бомба
 * 
 * Специальная клетка, которая завершает игру при наступании на нее.
 * Игрок должен избегать этих клеток.
 */
class BombCell: public ICell {
private:
    bool _isAvailable; /**< Флаг доступности клетки */

public:
    /**
     * @brief Конструктор клетки-бомбы
     */
    BombCell();
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~BombCell() = default;

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
     * @return Результат столкновения
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
};

#endif
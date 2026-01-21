/**
 * @file TeleportCell.hpp
 * @brief Заголовочный файл, содержащий объявление класса TeleportCell
 */
#ifndef TELEPORTCELL
#define TELEPORTCELL

#include "interfaces/ICell.hpp"

/**
 * @brief Клетка-телепорт
 * 
 * Специальная клетка, которая телепортирует игрока в другую позицию на поле
 * при наступании на нее.
 */
class TeleportCell: public ICell {
private:
    Position _teleportPos; /**< Целевая позиция телепортации */
    bool _tpAvailable;     /**< Флаг доступности телепорта */

public:
    /**
     * @brief Конструктор клетки-телепорта
     * @param teleportPos Целевая позиция телепортации
     */
    TeleportCell(Position teleportPos);
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~TeleportCell() = default;

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
    
    /**
     * @brief Возвращает целевую позицию телепортации
     * @return Позиция телепортации
     */
    Position getTPPos() const;
};

#endif
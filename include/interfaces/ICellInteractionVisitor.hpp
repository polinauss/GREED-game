/**
 * @file ICellInteractionVisitor.h
 * @brief Заголовочный файл, содержащий объявление интерфейса ICellInteractionVisitor
 */
#ifndef CELLINTERACTIONVISITOR
#define CELLINTERACTIONVISITOR

class Position;
class BasicCell;
class TeleportCell;
class BombCell;

/**
 * @brief Интерфейс посетителя для взаимодействия с клетками
 * 
 * Реализует паттерн "Посетитель" для обработки столкновений и наступаний
 * на различные типы клеток. Позволяет отделить логику взаимодействия от классов клеток.
 */
class ICellInteractionVisitor {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~ICellInteractionVisitor() = default;

    /**
     * @brief Обрабатывает столкновение с базовой клеткой
     * @param cell Ссылка на базовую клетку
     * @return Результат столкновения (например, изменение счета)
     */
    virtual int collideWithBasicCell(BasicCell& cell) = 0;
    
    /**
     * @brief Обрабатывает наступание на базовую клетку
     * @param cell Ссылка на базовую клетку
     * @param position Позиция наступания
     * @return Итоговая позиция после наступания
     */
    virtual Position stepOnBasicCell(BasicCell& cell, const Position& position) = 0;
    
    /**
     * @brief Обрабатывает завершение наступания на базовую клетку
     * @param startPos Начальная позиция
     * @param finalPos Конечная позиция
     */
    virtual void handleStepOnBasicCell(const Position& startPos, const Position& finalPos) = 0;

    /**
     * @brief Обрабатывает столкновение с клеткой-телепортом
     * @param cell Ссылка на клетку-телепорт
     * @return Результат столкновения
     */
    virtual int collideWithTeleportCell(TeleportCell& cell) = 0;
    
    /**
     * @brief Обрабатывает наступание на клетку-телепорт
     * @param cell Ссылка на клетку-телепорт
     * @param position Позиция наступания
     */
    virtual void stepOnTeleportCell(TeleportCell& cell, const Position& position) = 0;

    /**
     * @brief Обрабатывает столкновение с клеткой-бомбой
     * @param cell Ссылка на клетку-бомбу
     * @return Результат столкновения
     */
    virtual int collideWithBombCell(BombCell& cell) = 0;
    
    /**
     * @brief Обрабатывает наступание на клетку-бомбу
     * @param cell Ссылка на клетку-бомбу
     * @param position Позиция наступания
     */
    virtual void stepOnBombCell(BombCell& cell, const Position& position) = 0;
};

#endif
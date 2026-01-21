/**
 * @file ICellRenderVisitor.h
 * @brief Заголовочный файл, содержащий объявление интерфейса ICellRenderVisitor
 */
#ifndef CELLRENDERVISITOR
#define CELLRENDERVISITOR

#include "model/Position.hpp"

class BasicCell;
class Position;

/**
 * @brief Интерфейс посетителя для отрисовки клеток
 * 
 * Реализует паттерн "Посетитель" для отрисовки различных типов клеток.
 * Позволяет отделить логику отрисовки от самих классов клеток.
 */
class ICellRenderVisitor {
public:
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~ICellRenderVisitor() = default;

    /**
     * @brief Отрисовывает базовую клетку
     * @param cell Ссылка на базовую клетку
     * @param pos Позиция для отрисовки
     * @param highlightColor Цвет выделения (по умолчанию стандартный)
     */
    virtual void drawBasicCell(const BasicCell& cell, const Position& pos, Color highlightColor = Color::DEFAULT) const = 0;
    
    /**
     * @brief Отрисовывает клетку-телепорт
     * @param cell Ссылка на клетку-телепорт
     * @param pos Позиция для отрисовки
     */
    virtual void drawTeleportCell(const TeleportCell& cell, const Position& pos) const = 0;
    
    /**
     * @brief Отрисовывает клетку-бомбу
     * @param cell Ссылка на клетку-бомбу
     * @param pos Позиция для отрисовки
     */
    virtual void drawBombCell(const BombCell& cell, const Position& pos) const = 0;
};

#endif
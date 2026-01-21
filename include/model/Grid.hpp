/**
 * @file Grid.hpp
 * @brief Заголовочный файл, содержащий объявление класса Grid
 */
#ifndef GRID
#define GRID

#include "core/Color.hpp"
#include "model/CellGenerator.hpp"
#include "interfaces/ICell.hpp"
#include "model/Position.hpp"
#include <vector>

/**
 * @brief Класс игрового поля
 * 
 * Управляет сеткой клеток, предоставляет доступ к клеткам по позициям
 * и отвечает за инициализацию и восстановление состояния поля.
 */
class Grid {
private:
    std::vector<ICell*> _cells; /**< Вектор клеток поля (линейное представление) */
    int _width;                 /**< Ширина поля */
    int _height;                /**< Высота поля */
    CellGenerator _generator;   /**< Генератор клеток */

public:
    /**
     * @brief Конструктор поля
     * @param width Ширина поля
     * @param height Высота поля
     */
    Grid(int width, int height);
    
    /**
     * @brief Деструктор поля
     * @note Освобождает память занятую клетками
     */
    ~Grid();

    /**
     * @brief Инициализирует поле случайными клетками
     */
    void initializeRandom();
    
    /**
     * @brief Восстанавливает состояние поля из данных
     * @param cellValues Значения клеток
     * @param cellColors Цвета клеток
     * @param cellAvailable Флаги доступности клеток
     * @param cellTypes Типы клеток (по умолчанию пустой вектор)
     * @param teleportTargetsX X-координаты целей телепортов
     * @param teleportTargetsY Y-координаты целей телепортов
     */
    void restoreState(const std::vector<int>& cellValues, 
                 const std::vector<int>& cellColors,
                 const std::vector<int>& cellAvailable,
                 const std::vector<int>& cellTypes = std::vector<int>(),
                 const std::vector<int>& teleportTargetsX = std::vector<int>(),
                 const std::vector<int>& teleportTargetsY = std::vector<int>());

    /**
     * @brief Оператор доступа к клетке по позиции
     * @param position Позиция клетки
     * @return Ссылка на клетку
     */
    ICell& operator[] (const Position& position);
    
    /**
     * @brief Константный оператор доступа к клетке по позиции
     * @param position Позиция клетки
     * @return Константная ссылка на клетку
     */
    const ICell& operator[] (const Position& position) const;
    
    /**
     * @brief Проверяет валидность позиции
     * @param position Проверяемая позиция
     * @return true если позиция в пределах поля, false в противном случае
     */
    bool isValidPosition(const Position& position) const;
    
    /**
     * @brief Удаляет клетку с заданной позиции
     * @param position Позиция удаляемой клетки
     */
    void removeCell(const Position& position);
    
    /**
     * @brief Возвращает ширину поля
     * @return Ширина поля в клетках
     */
    int getWidth() const;
    
    /**
     * @brief Возвращает высоту поля
     * @return Высота поля в клетках
     */
    int getHeight() const;

private:
    /**
     * @brief Очищает все клетки поля
     * @note Освобождает память и очищает вектор
     */
    void clearCells();
};

#endif
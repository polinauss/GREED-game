/**
 * @file CellGenerator.hpp
 * @brief Заголовочный файл, содержащий объявление класса CellGenerator
 */
#ifndef CELLGENERATOR
#define CELLGENERATOR

#include "interfaces/ICell.hpp"
#include "core/Color.hpp"
#include <vector>

/**
 * @brief Генератор клеток для игрового поля
 * 
 * Отвечает за создание различных типов клеток и генерацию случайных сеток.
 */
class CellGenerator {
public:
    /**
     * @brief Конструктор по умолчанию
     */
    CellGenerator() = default;
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~CellGenerator() = default;

    /**
     * @brief Генерирует случайную сетку клеток
     * @param width Ширина сетки
     * @param height Высота сетки
     * @return Вектор указателей на созданные клетки
     */
    std::vector<ICell*> generateRandomGrid(int width, int height);

    /**
     * @brief Создает базовую клетку
     * @param value Значение клетки
     * @return Указатель на созданную базовую клетку
     */
    ICell* createBasicCell(int value);
    
    /**
     * @brief Создает случайную клетку
     * @return Указатель на созданную клетку случайного типа
     */
    ICell* createRandomCell();

private:
    /**
     * @brief Получает цвет по значению клетки
     * @param value Значение клетки
     * @return Цвет для отображения клетки
     */
    Color getColor(int value);
};

#endif
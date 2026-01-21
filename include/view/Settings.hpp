/**
 * @file Settings.hpp
 * @brief Заголовочный файл, содержащий объявление класса Settings
 */
#ifndef SETTINGS
#define SETTINGS

#include <iostream>
#include "model/Position.hpp"

/**
 * @brief Класс настроек отображения
 * 
 * Управляет параметрами терминала и расчетами для центрирования игрового поля.
 */
class Settings {
private:
    int _terminalWidth;  /**< Текущая ширина терминала */
    int _terminalHeight; /**< Текущая высота терминала */
    int _minWidth;       /**< Минимальная допустимая ширина */
    int _minHeight;      /**< Минимальная допустимая высота */

public:
    /**
     * @brief Конструктор настроек
     * @param width Ширина терминала (по умолчанию 200)
     * @param height Высота терминала (по умолчанию 55)
     */
    Settings(int width = 200, int height = 55);
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~Settings() = default;

    /**
     * @brief Обновляет размеры терминала
     * @note Считывает текущие размеры терминала из системы
     */
    void updateTerminalSize();
    
    /**
     * @brief Рассчитывает смещения для центрирования игрового поля
     * @param gridWidth Ширина игрового поля
     * @param gridHeight Высота игрового поля
     * @return Позиция смещения для центрирования
     */
    Position calculateCenteringOffsets(int gridWidth, int gridHeight);
};

#endif
/**
 * @file InputHandler.h
 * @brief Заголовочный файл, содержащий объявление класса InputHandler
 */
#ifndef INPUTHANDLER
#define INPUTHANDLER

#include "core/Directions.hpp"
#include <termios.h>

/**
 * @brief Обработчик пользовательского ввода
 * 
 * Обрабатывает нажатия клавиш, преобразует их в направления движения,
 * управляет режимами работы терминала (канонический/неканонический).
 */
class InputHandler {
private:
    struct termios _originalTermios; /**< Оригинальные настройки терминала */
    bool _isNonCanonicalMode;        /**< Флаг неканонического режима */

public:
    /**
     * @brief Конструктор обработчика ввода
     * @note Сохраняет текущие настройки терминала
     */
    InputHandler();
    
    /**
     * @brief Деструктор обработчика ввода
     * @note Восстанавливает оригинальные настройки терминала
     */
    ~InputHandler();

    /**
     * @brief Удаленный конструктор копирования
     */
    InputHandler(const InputHandler&) = delete;
    
    /**
     * @brief Удаленный оператор присваивания
     */
    InputHandler& operator=(const InputHandler&) = delete;

    /**
     * @brief Включает канонический режим терминала
     * @note Ввод обрабатывается построчно с поддержкой редактирования
     */
    void enableCanonicalMode();
    
    /**
     * @brief Отключает канонический режим терминала
     * @note Ввод обрабатывается посимвольно без буферизации
     */
    void disableCanonicalMode();
    
    /**
     * @brief Получает направление из пользовательского ввода
     * @return Направление движения (UP, DOWN, LEFT, RIGHT, NONE)
     * @note Обрабатывает как обычные клавиши (wasd/стрелки), так и ESC-последовательности
     */
    Direction getDirectionFromInput();

    /**
     * @brief Проверяет валидность символа как направления
     * @param input Введенный символ
     * @return true если символ соответствует направлению, false в противном случае
     */
    bool isValidDirection(char input) const;
    
    /**
     * @brief Преобразует символ в направление
     * @param input Введенный символ
     * @return Соответствующее направление движения
     */
    Direction convertToDirection(char input) const;
    
    /**
     * @brief Обрабатывает ESC-последовательности (стрелки)
     * @param result Направление, полученное из последовательности
     * @return true если обработана ESC-последовательность, false в противном случае
     */
    bool handleEscapeSequence(Direction& result) const;
};

#endif
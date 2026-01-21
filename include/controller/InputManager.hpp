/**
 * @file InputManager.h
 * @brief Заголовочный файл, содержащий объявление класса InputManager
 */
#ifndef INPUTMANAGER
#define INPUTMANAGER

#include <termios.h>
#include <functional>
#include <map>

/**
 * @brief Менеджер ввода для работы с терминалом
 * 
 * Управляет режимами работы терминала (блокирующий/неблокирующий ввод),
 * сохраняет и восстанавливает оригинальные настройки терминала.
 */
class InputManager {
private:
    struct termios _originalTermios; /**< Оригинальные настройки терминала */
    bool _nonBlockingMode;           /**< Флаг неблокирующего режима */

public:
    /**
     * @brief Конструктор менеджера ввода
     * @note Сохраняет текущие настройки терминала
     */
    InputManager();
    
    /**
     * @brief Деструктор менеджера ввода
     * @note Восстанавливает оригинальные настройки терминала
     */
    ~InputManager();
    
    /**
     * @brief Устанавливает неблокирующий режим ввода
     * @param enable true - включить неблокирующий режим, false - выключить
     */
    void setNonBlockingMode(bool enable);
    
    /**
     * @brief Получает символ в неблокирующем режиме
     * @return Введенный символ или 0 если ввода нет
     */
    char getCharNonBlocking();
    
    /**
     * @brief Проверяет наличие ввода
     * @return true если есть доступный ввод, false в противном случае
     */
    bool hasInput();
    
    /**
     * @brief Восстанавливает настройки терминала
     * @note Возвращает терминал в состояние на момент создания объекта
     */
    void restoreTerminal();
};

#endif
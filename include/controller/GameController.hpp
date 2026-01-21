/**
 * @file GameController.h
 * @brief Заголовочный файл, содержащий объявление класса GameController
 */
#ifndef GAMECONTROLLER
#define GAMECONTROLLER

#include "view/GameView.hpp"
#include "model/GameModel.hpp"
#include "controller/InputHandler.hpp"
#include <vector>
#include <memory>
#include <functional>
#include <cstddef>

/**
 * @brief Основной контроллер игры
 * 
 * Управляет взаимодействием между моделью (GameModel) и представлением (GameView),
 * обрабатывает пользовательский ввод и управляет состоянием игры.
 */
class GameController {
private:
    GameModel* _model;                    /**< Указатель на модель игры */
    GameView* _view;                      /**< Указатель на представление игры */
    std::unique_ptr<InputHandler> _inputHandler; /**< Обработчик пользовательского ввода */
    bool _paused;                         /**< Флаг паузы игры */
    bool _shouldReturnToMenu;             /**< Флаг необходимости возврата в меню */
    bool _terminalTooSmall;               /**< Флаг несоответствия терминала минимальным размерам */
    int _minTerminalWidth;                /**< Минимальная ширина терминала */
    int _minTerminalHeight;               /**< Минимальная высота терминала */
    std::function<void()> _saveCallback;  /**< Callback-функция для сохранения игры */
    std::function<bool()> _menuCallback;  /**< Callback-функция для возврата в меню */

public:
    /**
     * @brief Конструктор контроллера игры
     * @param model Указатель на модель игры
     * @param view Указатель на представление игры
     */
    GameController(GameModel* model, GameView* view);
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~GameController() = default;
    
    /**
     * @brief Запускает основной игровой цикл
     * @note Управляет обновлением состояния, отрисовкой и обработкой ввода
     */
    void startGame();
    
    /**
     * @brief Устанавливает callback для сохранения игры
     * @param callback Функция сохранения игры
     */
    void setSaveCallback(std::function<void()> callback);
    
    /**
     * @brief Устанавливает callback для возврата в меню
     * @param callback Функция возврата в меню
     */
    void setMenuCallback(std::function<bool()> callback);
    
    /**
     * @brief Проверяет необходимость возврата в меню
     * @return true если требуется возврат в меню, false в противном случае
     */
    bool shouldReturnToMenu() const { return _shouldReturnToMenu; }
    
    /**
     * @brief Сбрасывает флаг возврата в меню
     */
    void resetReturnToMenu() { _shouldReturnToMenu = false; }
    
    /**
     * @brief Проверяет, находится ли игра на паузе
     * @return true если игра на паузе, false в противном случае
     */
    bool isPaused() const { return _paused; }

private:
    /**
     * @brief Проверяет размер терминала
     * @return true если размер терминала достаточен, false в противном случае
     * @note Сравнивает текущий размер с минимальными требованиями
     */
    bool checkTerminalSize();
    
    /**
     * @brief Показывает сообщение о недостаточном размере терминала
     * @note Выводит инструкции для изменения размера окна
     */
    void showTerminalTooSmallMessage();
    
    /**
     * @brief Очищает экран терминала
     */
    void clearScreen();
};

#endif
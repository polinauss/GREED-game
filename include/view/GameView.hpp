/**
 * @file GameView.hpp
 * @brief Заголовочный файл, содержащий объявление класса GameView
 */
#ifndef GAMEVIEW
#define GAMEVIEW

#include "model/GameModel.hpp"
#include "model/Position.hpp"
#include "view/ConsoleRenderer.hpp"
#include "view/Settings.hpp"
#include <memory>
#include <vector>
#include <string>

/**
 * @brief Основной класс представления игры
 * 
 * Отвечает за отображение игрового состояния, взаимодействует с моделью
 * и делегирует отрисовку специализированному рендереру.
 */
class GameView {
private:
    GameModel* _model;                    /**< Указатель на модель игры */
    std::unique_ptr<ConsoleRenderer> _renderer; /**< Умный указатель на рендерер */
    std::unique_ptr<Settings> _settings;        /**< Умный указатель на настройки */

    /**
     * @brief Обновляет рендерер
     * @note Пересоздает рендерер с актуальными смещениями
     */
    void updateRenderer();

public:
    /**
     * @brief Конструктор представления игры
     * @param model Указатель на модель игры
     */
    explicit GameView(GameModel* model);
    
    /**
     * @brief Виртуальный деструктор
     */
    virtual ~GameView();
  
    /**
     * @brief Обновляет отображение
     * @note Очищает экран и перерисовывает текущее состояние
     */
    void refresh();
    
    /**
     * @brief Отображает информацию о смещении (для отладки)
     */
    void displayOffset();
    
    /**
     * @brief Рендерит текущий счет
     */
    void renderScore();
    
    /**
     * @brief Рендерит начальное состояние игры
     */
    void renderStatringState();
    
    /**
     * @brief Рендерит ход игрока
     */
    void renderMove();
    
    /**
     * @brief Подсвечивает возможные направления движения
     * @param availableMoves Доступные ходы
     * @param direction Направление для подсветки
     */
    void highlightMoveDirection(std::vector<std::pair<bool, Position>>& availableMoves, Direction direction);
    
    /**
     * @brief Подсвечивает состояние завершения игры
     */
    void highlightGameOver();
    
    /**
     * @brief Отображает экран завершения игры
     */
    void displayGameOver();
    
    /**
     * @brief Отображает экран приветствия
     */
    void displayWelcomeScreen();
    
    /**
     * @brief Отображает меню
     * @param menuItems Пункты меню
     * @param selectedIndex Индекс выбранного пункта
     */
    void displayMenu(const std::vector<std::string>& menuItems, int selectedIndex);
    
    /**
     * @brief Показывает справку по управлению
     */
    void showHelp();
};

#endif
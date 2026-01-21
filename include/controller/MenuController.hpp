/**
 * @file MenuController.h
 * @brief Заголовочный файл, содержащий объявление класса MenuController и вспомогательных структур
 */
#ifndef MENUCONTROLLER
#define MENUCONTROLLER

#include "model/GameModel.hpp"
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <cstddef>

/**
 * @brief Структура для хранения состояния игры
 * 
 * Содержит все необходимые данные для сохранения и загрузки игрового состояния.
 */
struct GameState {
    Position playerPosition;          /**< Позиция игрока на поле */
    int score;                       /**< Текущий счет */
    int width;                       /**< Ширина игрового поля */
    int height;                      /**< Высота игрового поля */
    std::vector<int> cellValues;     /**< Значения клеток поля */
    std::vector<int> cellColors;     /**< Цвета клеток поля */
    std::vector<int> cellAvailable;  /**< Флаги доступности клеток */
    
    std::vector<int> cellTypes;      /**< Типы клеток (обычные, телепорты и т.д.) */
    std::vector<int> teleportTargetsX; /**< X-координаты целей телепортов */
    std::vector<int> teleportTargetsY; /**< Y-координаты целей телепортов */
    
    /**
     * @brief Сериализует состояние игры в файл
     * @param file Поток для записи данных
     */
    void serialize(std::ofstream& file) const;
    
    /**
     * @brief Десериализует состояние игры из файла
     * @param file Поток для чтения данных
     */
    void deserialize(std::ifstream& file);
};

/**
 * @brief Структура записи таблицы лидеров
 */
struct LeaderboardEntry {
    std::string playerName; /**< Имя игрока */
    int score;              /**< Набранные очки */
    std::string date;       /**< Дата достижения результата */
    
    /**
     * @brief Оператор сравнения для сортировки по убыванию счета
     * @param other Другая запись для сравнения
     * @return true если текущий счет больше
     */
    bool operator>(const LeaderboardEntry& other) const {
        return score > other.score;
    }
};

/**
 * @brief Контроллер меню игры
 * 
 * Управляет главным меню, сохранением/загрузкой игры,
 * таблицей лидеров и вводом имени игрока.
 */
class MenuController {
private:
    std::string _playerName;                     /**< Имя текущего игрока */
    std::vector<LeaderboardEntry> _leaderboard;  /**< Таблица лидеров */
    const std::string SAVE_FILE = "game_save.dat"; /**< Имя файла сохранения */
    const std::string LEADERBOARD_FILE = "leaderboard.txt"; /**< Имя файла таблицы лидеров */
    bool _hasSavedGame;                          /**< Флаг наличия сохраненной игры */
    int _lastSelectedOption;                     /**< Последний выбранный пункт меню */
    
    int _currentSelectedIndex;                   /**< Текущий выбранный индекс в меню */
    std::vector<std::string> _currentMenuItems;  /**< Текущие пункты меню */
    
    static MenuController* globalMenuController; /**< Глобальный указатель на контроллер меню (для обработки сигналов) */

public:
    /**
     * @brief Конструктор контроллера меню
     * @note Загружает таблицу лидеров и проверяет наличие сохраненной игры
     */
    MenuController();
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~MenuController() = default;
    
    /**
     * @brief Принудительно устанавливает выбранный пункт меню
     * @param option Номер пункта меню
     */
    void forceSetSelectedOption(int option) { _lastSelectedOption = option; }
    
    /**
     * @brief Запускает главное меню
     * @return true если игра должна быть запущена, false для выхода
     */
    bool runMainMenu();
    
    /**
     * @brief Возвращает последний выбранный пункт меню
     * @return Номер выбранного пункта
     */
    int getLastSelectedOption() const { return _lastSelectedOption; }
    
    /**
     * @brief Показывает экран приветствия
     */
    void showWelcomeScreen();
    
    /**
     * @brief Показывает правила игры
     */
    void showRules();
    
    /**
     * @brief Устанавливает имя игрока
     * @note Запрашивает ввод имени через терминал
     */
    void setPlayerName();
    
    /**
     * @brief Показывает таблицу лидеров
     */
    void showLeaderboard();
    
    /**
     * @brief Сохраняет состояние игры
     * @param model Указатель на модель игры
     */
    void saveGame(GameModel* model);
    
    /**
     * @brief Загружает сохраненную игру
     * @param model Указатель на модель игры
     * @return true если загрузка успешна, false в противном случае
     */
    bool loadGame(GameModel* model);
    
    /**
     * @brief Добавляет результат в таблицу лидеров
     * @param score Набранные очки
     */
    void addToLeaderboard(int score);
    
    /**
     * @brief Возвращает имя текущего игрока
     * @return Имя игрока
     */
    const std::string& getPlayerName() const { return _playerName; }
    
    /**
     * @brief Проверяет наличие сохраненной игры
     * @return true если есть сохраненная игра, false в противном случае
     */
    bool hasSavedGame() const { return _hasSavedGame; }
    
    /**
     * @brief Обновляет меню
     * @note Перерисовывает меню с учетом изменений (например, после resize)
     */
    void refreshMenu();

private:
    /**
     * @brief Рисует ASCII-заголовок игры
     */
    void drawAsciiTitle();
    
    /**
     * @brief Отображает пункты меню
     * @param items Список пунктов меню
     * @param selectedIndex Индекс выбранного пункта
     */
    void displayMenuItems(const std::vector<std::string>& items, int selectedIndex);
    
    /**
     * @brief Загружает таблицу лидеров из файла
     */
    void loadLeaderboard();
    
    /**
     * @brief Сохраняет таблицу лидеров в файл
     */
    void saveLeaderboard();
    
    /**
     * @brief Обработчик изменения размера терминала
     * @param sig Номер сигнала
     * @note Статический метод для использования в обработчике сигналов
     */
    static void handleResize(int sig);
    
    /**
     * @brief Проверяет размер терминала
     * @return true если размер достаточен, false в противном случае
     */
    bool checkTerminalSize() const;
    
    /**
     * @brief Показывает сообщение об ошибке размера терминала
     */
    void displaySizeError() const;
};

#endif
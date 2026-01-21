/**
 * @file Position.hpp
 * @brief Заголовочный файл, содержащий объявление класса Position
 */
#ifndef POSITION
#define POSITION

/**
 * @brief Класс позиции на игровом поле
 * 
 * Хранит координаты X и Y, предоставляет операции для работы с позициями.
 */
class Position {
private:
    int _x; /**< Координата X (горизонталь) */
    int _y; /**< Координата Y (вертикаль) */
 
public:
    /**
     * @brief Конструктор по умолчанию
     * @note Инициализирует позицию (0, 0)
     */
    Position() : _x(0), _y(0) {}
    
    /**
     * @brief Конструктор с координатами
     * @param x Координата X
     * @param y Координата Y
     */
    Position(int x, int y);
    
    /**
     * @brief Деструктор по умолчанию
     */
    ~Position() = default;
    
    /**
     * @brief Возвращает координату X
     * @return Координата X
     */
    int getX() const;
    
    /**
     * @brief Возвращает координату Y
     * @return Координата Y
     */
    int getY() const;
    
    /**
     * @brief Оператор сравнения позиций
     * @param other Другая позиция для сравнения
     * @return true если позиции равны, false в противном случае
     */
    bool operator== (const Position& other) const;
    
    /**
     * @brief Оператор сложения позиций
     * @param other Другая позиция для сложения
     * @return Новая позиция как сумма координат
     */
    Position operator+(const Position& other) const;
};

#endif
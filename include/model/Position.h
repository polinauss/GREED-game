#ifndef POSITION
#define POSITION

class Position {
private:
    int _x;
    int _y;
 
public:
    Position() : _x(0), _y(0) {}
    Position(int x, int y);
    ~Position() = default;
    
    int getX() const;
    int getY() const;
    bool operator== (const Position& other) const;
    Position operator+(const Position& other) const;

};

#endif

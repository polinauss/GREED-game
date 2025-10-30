#ifndef POSITION
#define POSITION

class Position {
private:
    int _x;
    int _y;
 
public:
    Position(int x, int y);
    ~Position() = default;
    
    int getX() const;
    int getY() const;
    bool operator== (const Position& other) const;
};

#endif
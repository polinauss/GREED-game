#ifndef COLOR
#define COLOR

#include <vector>

enum class Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,
    WHITE,
    DEFAULT,
    BLUEHIGHLIGHT,
    REDHIGHLIGHT,
    BLACK,
    GREENHIGHLIGHT
};

inline const std::vector<Color> gameColors = {
    Color::RED, Color::GREEN, Color::BLUE, 
    Color::YELLOW, Color::CYAN, Color::MAGENTA
};

#endif

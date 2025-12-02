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
    WHITEBACKGROUND,
    BLACK,
    GRAY,
    DARK_GRAY
};

inline const std::vector<Color> gameColors = {
    Color::RED,
    Color::GREEN, 
    Color::BLUE,
    Color::YELLOW,
    Color::CYAN,
    Color::MAGENTA,
    Color::WHITE
};

#endif

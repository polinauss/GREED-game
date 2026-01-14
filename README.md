# 🎮 Greed Game - Colorful Number Jumping Adventure

[![C++ Version](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/Platform-Linux%2FUnix-lightgrey.svg)](https://en.wikipedia.org/wiki/Linux)
[![CMake](https://img.shields.io/badge/CMake-3.12+-green.svg)](https://cmake.org/)

## 📋 Description

**Greed** is a console-based strategic arcade game developed as a C++ programming practice project. The game challenges players to navigate a colorful grid, jumping on numbered cells to accumulate points while strategically avoiding bombs and utilizing teleporters for tactical movement.

## 🎥 Video Demonstration

[![Gameplay Demo](media/game.gif)](media/game.mp4)


## ✨ Key Features

### 🎯 Core Gameplay
+ **Player movement control** via keyboard with intuitive WASD/arrow key navigation
+ **Color-coded cell system** with six distinct color-value combinations (1-5 points each)
+ **Strategic grid navigation** on a 25×25 playing field
+ **Progressive difficulty** as score increases

### 💣 Special Elements
+ **Bomb cells** that deduct 20% of current score + 9 points
+ **Teleport cells** for instant battlefield repositioning
+ **Dynamic scoring system** with three mastery tiers (Novice, Experienced, Master)

### 💾 System Features
+ **Persistent game saving/loading** with automatic and manual save options
+ **Leaderboard system** tracking top 10 scores with player names and dates
+ **Interactive menu system** with intuitive navigation
+ **Console rendering** using ANSI color codes and ASCII graphics

### Architecture
+ **Design Patterns**: MVC, Visitor, Factory patterns implementation
+ **Modular codebase** with clear separation of concerns
+ **Configurable settings** through structured game state files

## 🎮 Controls

### In-Game Controls:
| Key | Action | Description |
|-----|--------|-------------|
| **W** or **↑** | Move Up | Move character upward |
| **A** or **←** | Move Left | Move character left |
| **S** or **↓** | Move Down | Move character downward |
| **D** or **→** | Move Right | Move character right |
| **P** | Pause | Pause game / Access pause menu |
| **S** | Save | Save current game progress |
| **M** | Menu | Return to main menu |
| **ESC** | Exit | Exit game |

### Menu Navigation:
+ **↑/↓ Arrow Keys** - Navigate menu options
+ **Enter/Return** - Select highlighted option
+ **ESC** or **Q** - Exit application

## 🎯 How to Play

### Basic Rules:
1. **Start from the center** of a 25×25 grid
2. **Move to adjacent cells** using directional controls
3. **When landing on a numbered cell**, jump forward by its value
4. **Earn points** equal to the cell's value
5. **Avoid bomb cells** - they reduce your score
6. **Use teleport cells** for strategic repositioning
7. **Game ends** if you land off-grid or on an unavailable cell

### Cell Types & Values:
| Cell | Display | Color | Value | Effect |
|------|---------|-------|-------|--------|
| Basic Cell 1 | `1` | 🔴 Red | 1 point | Jump 1 cell forward |
| Basic Cell 2 | `2` | 🟢 Green | 2 points | Jump 2 cells forward |
| Basic Cell 3 | `3` | 🔵 Blue | 3 points | Jump 3 cells forward |
| Basic Cell 4 | `4` | 🟡 Yellow | 4 points | Jump 4 cells forward |
| Basic Cell 5 | `5` | 🟣 Purple | 5 points | Jump 5 cells forward |
| Bomb Cell | `B` | 💣 Red Background | -20% -9 | Deducts points |
| Teleport Cell | `T` | 🌀 Green Background | 0 | Random teleportation |

### Scoring Tiers:
| Tier | Points Range | Border Color | Badge |
|------|--------------|--------------|-------|
| **🥉 Novice** | 0-99 points | 🔴 Red | Bronze |
| **🥈 Experienced** | 100-199 points | 🟡 Yellow | Silver |
| **🥇 Master** | 200+ points | 🟢 Green | Gold(Winner) |

## 🚀 Installation

### Prerequisites:
- **C++17 Compiler** (g++ 7.0+ or clang++ 5.0+ recommended)
- **CMake 3.12+**
- **Make** (GNU Make 3.81+)
- **Linux/Unix Terminal** with ANSI color support
- **Minimum Terminal Size**: 80 columns × 30 rows

### Quick Installation:

#### Option 1: Using CMake (Recommended)
```bash
# Clone the repository
git clone https://github.com/polinauss/GREED-game.git
cd greed-game

# Create and enter build directory
mkdir build && cd build

# Configure the build system
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compile the project
make -j$(nproc)

# Run the game
./greed_game

# Controls

## In-Game Controls:
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

## Menu Navigation:
+ **↑/↓ Arrow Keys** - Navigate menu options
+ **Enter/Return** - Select highlighted option
+ **ESC** or **Q** - Exit application

## Basic Rules:
1. **Start from the center** of a 25×25 grid
2. **Move to adjacent cells** using directional controls
3. **When landing on a numbered cell**, jump forward by its value
4. **Earn points** equal to the cell's value
5. **Avoid bomb cells** - they reduce your score
6. **Use teleport cells** for strategic repositioning
7. **Game ends** if you land off-grid or on an unavailable cell

## Cell Types & Values:
| Cell | Display | Color | Value | Effect |
|------|---------|-------|-------|--------|
| Basic Cell 1 | `1` | 🔴 Red | 1 point | Jump 1 cell forward |
| Basic Cell 2 | `2` | 🟢 Green | 2 points | Jump 2 cells forward |
| Basic Cell 3 | `3` | 🔵 Blue | 3 points | Jump 3 cells forward |
| Basic Cell 4 | `4` | 🟡 Yellow | 4 points | Jump 4 cells forward |
| Basic Cell 5 | `5` | 🟣 Purple | 5 points | Jump 5 cells forward |
| Bomb Cell | `B` | Red Background | -20% -9 | Deducts points |
| Teleport Cell | `T` | Green Background | 0 | Random teleportation |

[Get back](README.md)

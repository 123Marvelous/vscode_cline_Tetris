#include "input.h"
#include <conio.h>

GameKey get_input(void) {
    if (_kbhit()) {
        int ch = _getch();
        
        // ESC key
        if (ch == 27) {
            return KEY_EXIT;
        }
        
        // Support arrow keys (which prefix with 0 or 224 on Windows console)
        if (ch == 0 || ch == 224) {
            int arrow = _getch();
            switch (arrow) {
                case 72: return KEY_ROTATE; // Up arrow -> Rotate
                case 75: return KEY_LEFT;   // Left arrow -> Left
                case 77: return KEY_RIGHT;  // Right arrow -> Right
                case 80: return KEY_DOWN;   // Down arrow -> Soft Drop
                default: break;
            }
        }
        
        // Support WASD keys
        switch (ch) {
            case 'a':
            case 'A':
                return KEY_LEFT;
            case 'd':
            case 'D':
                return KEY_RIGHT;
            case 'w':
            case 'W':
                return KEY_ROTATE;
            case 's':
            case 'S':
                return KEY_DOWN;
            default:
                break;
        }
    }
    return KEY_NONE;
}

void handle_input(GameState *state, GameKey key) {
    if (state->game_over) return;
    
    switch (key) {
        case KEY_LEFT:
            move_tetromino(state, -1, 0);
            break;
        case KEY_RIGHT:
            move_tetromino(state, 1, 0);
            break;
        case KEY_ROTATE:
            rotate_tetromino(state);
            break;
        case KEY_DOWN:
            // Soft Drop: Try to move down. If it fails, standard update_game logic will lock it
            move_tetromino(state, 0, 1);
            break;
        default:
            break;
    }
}

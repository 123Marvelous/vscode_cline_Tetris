#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>

#define MAP_HEIGHT 20
#define MAP_WIDTH 10

// 7 Tetromino types
// 0: Empty
// 1: I, 2: O, 3: T, 4: S, 5: Z, 6: J, 7: L
extern const int8_t SHAPES[7][4][4][4];
extern const uint8_t COLOR_MAP[8];

// Game State structure
typedef struct {
    uint8_t map[MAP_HEIGHT][MAP_WIDTH];
    uint32_t score;
    uint32_t lines_cleared;
    bool game_over;
    
    // Current falling tetromino
    int8_t cur_type;         // 0 to 6
    int8_t cur_rotation;     // 0 to 3
    int8_t cur_x;            // X coordinate (column) on the map (can be negative during rotation/movement tests)
    int8_t cur_y;            // Y coordinate (row) on the map
    
    // Next tetromino
    int8_t next_type;        // 0 to 6
} GameState;

// Function declarations
void init_game(GameState *state);
void spawn_tetromino(GameState *state);
bool check_collision(const GameState *state, int8_t type, int8_t rotation, int8_t x, int8_t y);
bool rotate_tetromino(GameState *state);
bool move_tetromino(GameState *state, int8_t dx, int8_t dy);
void lock_tetromino(GameState *state);
uint8_t clear_lines(GameState *state);
void update_game(GameState *state);

#endif // GAME_H

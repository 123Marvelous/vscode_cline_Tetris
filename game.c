#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definition of the 7 Tetrominos, each has 4 rotations of 4x4 matrix
// 1-7 represent the block index (for color mapping and logical distinction)
const int8_t SHAPES[7][4][4][4] = {
    // 0: I shape
    {
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        }
    },
    // 1: O shape
    {
        {
            {0,2,2,0},
            {0,2,2,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,2,2,0},
            {0,2,2,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,2,2,0},
            {0,2,2,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,2,2,0},
            {0,2,2,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },
    // 2: T shape
    {
        {
            {0,3,0,0},
            {3,3,3,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,3,0,0},
            {0,3,3,0},
            {0,3,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {3,3,3,0},
            {0,3,0,0},
            {0,0,0,0}
        },
        {
            {0,3,0,0},
            {3,3,0,0},
            {0,3,0,0},
            {0,0,0,0}
        }
    },
    // 3: S shape
    {
        {
            {0,4,4,0},
            {4,4,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {4,0,0,0},
            {4,4,0,0},
            {0,4,0,0},
            {0,0,0,0}
        },
        {
            {0,4,4,0},
            {4,4,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,4,0,0},
            {0,4,4,0},
            {0,0,4,0},
            {0,0,0,0}
        }
    },
    // 4: Z shape
    {
        {
            {5,5,0,0},
            {0,5,5,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,5,0,0},
            {5,5,0,0},
            {5,0,0,0},
            {0,0,0,0}
        },
        {
            {5,5,0,0},
            {0,5,5,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,5,0},
            {0,5,5,0},
            {0,5,0,0},
            {0,0,0,0}
        }
    },
    // 5: J shape
    {
        {
            {6,0,0,0},
            {6,6,6,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,6,6,0},
            {0,6,0,0},
            {0,6,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {6,6,6,0},
            {0,0,6,0},
            {0,0,0,0}
        },
        {
            {0,6,0,0},
            {0,6,0,0},
            {6,6,0,0},
            {0,0,0,0}
        }
    },
    // 6: L shape
    {
        {
            {0,0,7,0},
            {7,7,7,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,7,0,0},
            {0,7,0,0},
            {0,7,7,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {7,7,7,0},
            {7,0,0,0},
            {0,0,0,0}
        },
        {
            {7,7,0,0},
            {0,7,0,0},
            {0,7,0,0},
            {0,0,0,0}
        }
    }
};

void init_game(GameState *state) {
    memset(state->map, 0, sizeof(state->map));
    state->score = 0;
    state->lines_cleared = 0;
    state->game_over = false;
    
    // Seed random number generator
    srand((unsigned int)time(NULL));
    
    // Initial tetrominoes
    state->cur_type = rand() % 7;
    state->cur_rotation = 0;
    state->cur_x = MAP_WIDTH / 2 - 2;
    state->cur_y = 0;
    
    state->next_type = rand() % 7;
}

void spawn_tetromino(GameState *state) {
    state->cur_type = state->next_type;
    state->cur_rotation = 0;
    state->cur_x = MAP_WIDTH / 2 - 2;
    state->cur_y = 0;
    
    state->next_type = rand() % 7;
    
    // If spawned piece immediately collides, it's game over
    if (check_collision(state, state->cur_type, state->cur_rotation, state->cur_x, state->cur_y)) {
        state->game_over = true;
    }
}

bool check_collision(const GameState *state, int8_t type, int8_t rotation, int8_t x, int8_t y) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (SHAPES[type][rotation][r][c] != 0) {
                int map_x = x + c;
                int map_y = y + r;
                
                // Out of horizontal bounds
                if (map_x < 0 || map_x >= MAP_WIDTH) {
                    return true;
                }
                
                // Out of bottom bound
                if (map_y >= MAP_HEIGHT) {
                    return true;
                }
                
                // Note: We allow blocks to be above the map (map_y < 0) during spawn or rotation
                if (map_y >= 0) {
                    if (state->map[map_y][map_x] != 0) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool rotate_tetromino(GameState *state) {
    int8_t next_rot = (state->cur_rotation + 1) % 4;
    
    // Wall kick simple check: try original position, then try left, right, and up
    int8_t offsets[5][2] = {
        {0, 0},
        {-1, 0},
        {1, 0},
        {0, -1},
        {-2, 0}
    };
    
    for (int i = 0; i < 5; i++) {
        int8_t test_x = state->cur_x + offsets[i][0];
        int8_t test_y = state->cur_y + offsets[i][1];
        if (!check_collision(state, state->cur_type, next_rot, test_x, test_y)) {
            state->cur_rotation = next_rot;
            state->cur_x = test_x;
            state->cur_y = test_y;
            return true;
        }
    }
    return false;
}

bool move_tetromino(GameState *state, int8_t dx, int8_t dy) {
    if (!check_collision(state, state->cur_type, state->cur_rotation, state->cur_x + dx, state->cur_y + dy)) {
        state->cur_x += dx;
        state->cur_y += dy;
        return true;
    }
    return false;
}

void lock_tetromino(GameState *state) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (SHAPES[state->cur_type][state->cur_rotation][r][c] != 0) {
                int map_y = state->cur_y + r;
                int map_x = state->cur_x + c;
                if (map_y >= 0 && map_y < MAP_HEIGHT && map_x >= 0 && map_x < MAP_WIDTH) {
                    state->map[map_y][map_x] = SHAPES[state->cur_type][state->cur_rotation][r][c];
                }
            }
        }
    }
}

uint8_t clear_lines(GameState *state) {
    uint8_t cleared = 0;
    
    for (int r = MAP_HEIGHT - 1; r >= 0; r--) {
        bool line_full = true;
        for (int c = 0; c < MAP_WIDTH; c++) {
            if (state->map[r][c] == 0) {
                line_full = false;
                break;
            }
        }
        
        if (line_full) {
            cleared++;
            // Shift all rows above down by one
            for (int r_shift = r; r_shift > 0; r_shift--) {
                memcpy(state->map[r_shift], state->map[r_shift - 1], sizeof(state->map[0]));
            }
            // Clear top row
            memset(state->map[0], 0, sizeof(state->map[0]));
            // Re-check same row index since lines shifted down
            r++;
        }
    }
    
    if (cleared > 0) {
        state->lines_cleared += cleared;
        // Scoring system:
        // 1 line: 100, 2 lines: 300, 3 lines: 600, 4 lines: 1000
        const uint32_t scores[5] = {0, 100, 300, 600, 1000};
        state->score += scores[cleared];
    }
    
    return cleared;
}

void update_game(GameState *state) {
    if (state->game_over) return;
    
    // Try moving down. If we can't, lock it and spawn next
    if (!move_tetromino(state, 0, 1)) {
        lock_tetromino(state);
        clear_lines(state);
        spawn_tetromino(state);
    }
}

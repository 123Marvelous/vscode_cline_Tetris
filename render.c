#include "render.h"
#include <stdio.h>
#include <windows.h>

// Color mapping for game blocks (indices 0 to 7)
// Empty, I, O, T, S, Z, J, L
const uint16_t COLOR_ATTRIBUTES[8] = {
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, // 0: Empty (dim white dots)
    BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY, // 1: Cyan (I)
    BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,  // 2: Yellow (O)
    BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY,   // 3: Magenta (T)
    BACKGROUND_GREEN | BACKGROUND_INTENSITY,                   // 4: Green (S)
    BACKGROUND_RED | BACKGROUND_INTENSITY,                     // 5: Red (Z)
    BACKGROUND_BLUE | BACKGROUND_INTENSITY,                    // 6: Blue (J)
    BACKGROUND_RED | BACKGROUND_GREEN                         // 7: Orange/Brown (L)
};

static HANDLE hConsole = NULL;
static CONSOLE_CURSOR_INFO originalCursorInfo;
static WORD originalAttributes;

void init_render(void) {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Set console title
    SetConsoleTitleA("Tetris - Pure C Windows Console");
    
    // Hide cursor
    GetConsoleCursorInfo(hConsole, &originalCursorInfo);
    CONSOLE_CURSOR_INFO cursorInfo = originalCursorInfo;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    
    // Get original attributes to restore later
    CONSOLE_SCREEN_BUFFER_INFO screenInfo;
    GetConsoleScreenBufferInfo(hConsole, &screenInfo);
    originalAttributes = screenInfo.wAttributes;
    
    // Clear screen initially
    system("cls");
}

void cleanup_render(void) {
    if (hConsole) {
        // Restore cursor
        SetConsoleCursorInfo(hConsole, &originalCursorInfo);
        // Restore original text attributes
        SetConsoleTextAttribute(hConsole, originalAttributes);
    }
}

static void set_cursor(int x, int y) {
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hConsole, pos);
}

void draw_game(const GameState *state) {
    // We compose the screen in memory or just draw from (0,0) without clearing
    // to avoid flickering completely.
    set_cursor(0, 0);
    
    // Create temporary board representing the merged map and current tetromino
    uint8_t display_grid[MAP_HEIGHT][MAP_WIDTH];
    memcpy(display_grid, state->map, sizeof(display_grid));
    
    if (!state->game_over) {
        for (int r = 0; r < 4; r++) {
            for (int c = 0; c < 4; c++) {
                if (SHAPES[state->cur_type][state->cur_rotation][r][c] != 0) {
                    int map_y = state->cur_y + r;
                    int map_x = state->cur_x + c;
                    if (map_y >= 0 && map_y < MAP_HEIGHT && map_x >= 0 && map_x < MAP_WIDTH) {
                        display_grid[map_y][map_x] = SHAPES[state->cur_type][state->cur_rotation][r][c];
                    }
                }
            }
        }
    }
    
    // Top border of the game board
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf(" +--------------------+\n");
    
    // Draw map rows
    for (int r = 0; r < MAP_HEIGHT; r++) {
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf(" |"); // Left border
        
        for (int c = 0; c < MAP_WIDTH; c++) {
            uint8_t cell = display_grid[r][c];
            if (cell == 0) {
                // Empty cell - dot grid
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                printf(". ");
            } else {
                // Colored tetromino block
                SetConsoleTextAttribute(hConsole, COLOR_ATTRIBUTES[cell]);
                printf("  "); // Print two spaces with background color
            }
        }
        
        // Right border
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        printf("|");
        
        // Side Panel (Score, Next Block, etc.)
        if (r == 1) {
            printf("   TETRIS IN PURE C");
        } else if (r == 3) {
            printf("   SCORE: %06u", state->score);
        } else if (r == 4) {
            printf("   LINES: %06u", state->lines_cleared);
        } else if (r == 6) {
            printf("   NEXT BLOCK:");
        } else if (r == 7) {
            printf("   +--------+");
        } else if (r >= 8 && r <= 11) {
            // Draw Next Block in a 4x4 grid
            int next_r = r - 8;
            printf("   | ");
            for (int next_c = 0; next_c < 4; next_c++) {
                int cell_val = SHAPES[state->next_type][0][next_r][next_c];
                if (cell_val != 0) {
                    SetConsoleTextAttribute(hConsole, COLOR_ATTRIBUTES[cell_val]);
                    printf("  ");
                } else {
                    SetConsoleTextAttribute(hConsole, 0); // Black space
                    printf("  ");
                }
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf(" |");
        } else if (r == 12) {
            printf("   +--------+");
        } else if (r == 14) {
            printf("   CONTROLS:");
        } else if (r == 15) {
            printf("   [A] / [D] : Move Left / Right");
        } else if (r == 16) {
            printf("   [W]       : Rotate");
        } else if (r == 17) {
            printf("   [S]       : Soft Drop");
        } else if (r == 18) {
            printf("   [ESC]     : Exit Game");
        }
        
        printf("\n");
    }
    
    // Bottom border of the game board
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf(" +--------------------+\n");
    
    // Reset attribute
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void draw_game_over(const GameState *state) {
    set_cursor(0, MAP_HEIGHT + 3);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("\n");
    printf("   =========================\n");
    printf("           GAME OVER        \n");
    printf("   =========================\n");
    printf("   FINAL SCORE: %u\n", state->score);
    printf("   LINES CLEARED: %u\n", state->lines_cleared);
    printf("\n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

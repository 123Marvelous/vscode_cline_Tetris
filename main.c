#include "game.h"
#include "render.h"
#include "input.h"
#include <windows.h>
#include <stdio.h>

int main(void) {
    GameState state;
    
    // Initialize game logic and rendering console
    init_game(&state);
    init_render();
    
    // Spawn the first tetromino
    spawn_tetromino(&state);
    
    // Initial draw
    draw_game(&state);
    
    DWORD last_fall_time = GetTickCount();
    DWORD last_render_time = GetTickCount();
    DWORD fall_delay = 800; // Falling delay in milliseconds
    
    while (!state.game_over) {
        DWORD current_time = GetTickCount();
        
        // 1. Check and get non-blocking keyboard input
        GameKey key = get_input();
        
        // ESC key exits the game immediately
        if (key == KEY_EXIT) {
            break;
        }
        
        // 2. Process input and update game state immediately for smooth responsiveness
        if (key != KEY_NONE) {
            handle_input(&state, key);
            draw_game(&state);
        }
        
        // 3. Adjust difficulty (speed) dynamically based on lines cleared
        // Level increases speed by decreasing the delay
        if (state.lines_cleared < 30) {
            fall_delay = 800 - (state.lines_cleared * 20); // Down to 200ms
        } else {
            fall_delay = 200; // Fast cap
        }
        
        // 4. Move tetromino down automatically when interval is reached
        if (current_time - last_fall_time >= fall_delay) {
            update_game(&state);
            last_fall_time = current_time;
            draw_game(&state);
        }
        
        // 5. Periodic render refresh (every 50ms) to ensure smooth screen state
        if (current_time - last_render_time >= 50) {
            draw_game(&state);
            last_render_time = current_time;
        }
        
        // 6. Relinquish CPU slice to keep execution quiet and low-power
        Sleep(15);
    }
    
    // Draw final state and show game over screen
    draw_game(&state);
    draw_game_over(&state);
    
    // Pause for user to see game over stats before closing console
    Sleep(1000);
    printf("   Press any key to close the window...\n");
    
    // Flush input buffer
    while (get_input() != KEY_NONE) {
        Sleep(10);
    }
    // Wait for a new key press
    while (get_input() == KEY_NONE) {
        Sleep(50);
    }
    
    cleanup_render();
    return 0;
}

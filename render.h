#ifndef RENDER_H
#define RENDER_H

#include "game.h"

// Initialize console (hide cursor, set title, etc.)
void init_render(void);

// Clean up console (show cursor, reset colors)
void cleanup_render(void);

// Render the entire game screen efficiently
void draw_game(const GameState *state);

// Display the Game Over screen
void draw_game_over(const GameState *state);

#endif // RENDER_H

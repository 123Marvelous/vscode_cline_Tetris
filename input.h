#ifndef INPUT_H
#define INPUT_H

#include "game.h"

// Define command keys
typedef enum {
    KEY_NONE,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ROTATE,
    KEY_DOWN,
    KEY_EXIT
} GameKey;

// Read non-blocking input
GameKey get_input(void);

// Handle the input key on the game state
void handle_input(GameState *state, GameKey key);

#endif // INPUT_H

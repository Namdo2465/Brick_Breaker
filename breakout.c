#include <stdlib.h>

#include "game.h"

int main(int argc, char** argv) {

  // Create and initialize the main game struct
  game_t game = {0};

  // Initialize SDL, create the window and renderer
  if (!setup_sdl(&game)) {
    exit(EXIT_FAILURE);
  }

  // Load the background image
  setup_background(&game);

  // Allocate memory and initialize the bricks
  if (!setup_bricks(&game)) {
    cleanup_game(&game);
    exit(EXIT_FAILURE);
  }

  // Initialize the paddle, ball, lives, and game state
  setup_game_state(&game);

  // Start the main game loop
  while (game.running) {

    // Handle keyboard and window events
    handle_events(&game);

    // Update movement, collisions, and game state
    update_game(&game);

    // Draw the current game state to the screen
    render_game(&game);

    // Delay to maintain a stable frame rate
    SDL_Delay(1000 / FRAME_RATE);
  }

  // Free memory and clean up SDL resources
  cleanup_game(&game);

  return 0;
}
#include <stdlib.h>
#include <stdio.h>

#include "game.h"

bool setup_sdl(game_t* game) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    // Something went wrong. Log an error and exit.
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return false;
  }

  // Create an SDL window
  game->window = SDL_CreateWindow(
    "Breakout - Final Project",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );

  if (game->window == NULL) {
    //// Something went wrong. Log an error, shut down SDL, and then exit
    fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }

  // Create an SDL renderer
  game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);

  // Check if the renderer was created successfully
  if (game->renderer == NULL) {
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    return false;
  }

  return true;
}

bool setup_background(game_t* game) {
  // Initialize SDL_image for loading PNG images
  IMG_Init(IMG_INIT_PNG);

  // Load our image
  SDL_Surface* bg_surface = IMG_Load("background.png");

  game->bg_texture = NULL;

  // Check if the image was loaded successfully
  if (bg_surface != NULL) {
    game->bg_texture = SDL_CreateTextureFromSurface(game->renderer, bg_surface);
    SDL_FreeSurface(bg_surface);
  } else {
    printf("Could not load background image\n");
  }

  return true;
}

bool setup_bricks(game_t* game) {
  // Initialize brick states
  game->bricks = malloc(sizeof(brick_state_t) * BRICK_ROWS * BRICK_COLS);

  // Check if memory allocation was successful
  if (game->bricks == NULL) {
    return false;
  }

  // Set all bricks to active
  for (int i = 0; i < BRICK_ROWS * BRICK_COLS; i++) { // This is a loop to initialize all the bricks in the game. We set them all to active at the start of the game.
    game->bricks[i] = CELL_ACTIVE;
  }

  return true;
}

void setup_game_state(game_t* game) {
  // The player paddle starts in the middle of the screen near the bottom, and is stationary at the start of the game
  game->player = (paddle_t) {
    .pos = { .x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, .y = SCREEN_HEIGHT - 50 },
    .vel = 0
  };

  // The ball starts in the middle of the screen and moves diagonally downwards at the start of the game
  game->ball = (ball_t) {
    .pos = { .x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT / 2 },
    .vel = { .x = BALL_SPEED_X, .y = BALL_SPEED_Y }
  };

  // track whether the left or right movement keys are currently pressed. This allows for smooth movement of the paddle when the keys are held down.
  game->left_pressed = false;
  game->right_pressed = false;

  game->lives = 3;

  // Initialize game state variables to track whether the game is over or won. This will be used to control the game flow and display appropriate messages.
  game->game_over = false;
  game->game_won = false;
  game->running = true;
}

void handle_events(game_t* game) {
  // Handle events
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      game->running = false;
    } else if (event.type == SDL_KEYDOWN) { // If a key is pressed down, we check if it's the left or right movement keys and set the corresponding variables to true. This allows the paddle to start moving in the next update cycle.
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        game->running = false;
      }

      // check for both the left arrow key and the 'A' key for left movement
      if (event.key.keysym.sym == SDLK_LEFT ||
          event.key.keysym.sym == SDLK_a) {
        game->left_pressed = true;
      }

      // same as above but for right movement keys.
      if (event.key.keysym.sym == SDLK_RIGHT ||
          event.key.keysym.sym == SDLK_d) {
        game->right_pressed = true;
      }
    } else if (event.type == SDL_KEYUP) { // If a key is released, this allows the paddle to stop moving left when the keys are released.
      if (event.key.keysym.sym == SDLK_LEFT ||
          event.key.keysym.sym == SDLK_a) {
        game->left_pressed = false;
      }

      if (event.key.keysym.sym == SDLK_RIGHT ||
          event.key.keysym.sym == SDLK_d) {
        game->right_pressed = false;
      }
    }
  }
}

void update_game(game_t* game) {
  // Update paddle
  // We only update the game state if the game is not over or won. This prevents the game from continuing to update and move the ball and paddle after the game has ended.
  if (!game->game_over && !game->game_won) {
    if (game->left_pressed) {
      // divide the speed by the frame rate to ensure consistent movement of the frame rate of the game.
      game->player.pos.x -= PADDLE_SPEED / (float)FRAME_RATE;
    }

    if (game->right_pressed) {
      game->player.pos.x += PADDLE_SPEED / (float)FRAME_RATE;
    }

    // Preventing paddle from moving off the left edge of the game screen
    if (game->player.pos.x < 0) {
      game->player.pos.x = 0;
    }

    // Preventing paddle from moving off the right edge of the game screen
    if (game->player.pos.x > SCREEN_WIDTH - PADDLE_WIDTH) {
      game->player.pos.x = SCREEN_WIDTH - PADDLE_WIDTH;
    }

    // Update ball's position based on its velocity
    game->ball.pos.x += game->ball.vel.x / (float)FRAME_RATE;
    game->ball.pos.y += game->ball.vel.y / (float)FRAME_RATE;

    // Check for collisions with walls
    // If the ball hits the left or right wall, we reverse its x velocity to make it bounce back in the opposite direction.
    if (game->ball.pos.x <= 0 || game->ball.pos.x >= SCREEN_WIDTH - BALL_SIZE) {
      game->ball.vel.x *= -1;
    }

    // If the ball hits the top wall, we reverse its y velocity to make it bounce back downwards towards the brick and paddle
    if (game->ball.pos.y <= 0) {
      game->ball.vel.y *= -1;
    }

    // If the ball goes below the bottom of the screen, it means the player missed it
    if (game->ball.pos.y >= SCREEN_HEIGHT) {
      game->lives--;

      // If the player has no lives left, we set the game over state to true to end the game and display the game over message
      if (game->lives <= 0) {
        game->game_over = true;
      } else { // If the player still has lives left, we reset the ball and paddle to their starting positions to continue playin
        game->ball.pos.x = SCREEN_WIDTH / 2;
        game->ball.pos.y = SCREEN_HEIGHT / 2;

        game->ball.vel.x = BALL_SPEED_X;
        game->ball.vel.y = BALL_SPEED_Y;

        game->player.pos.x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
        game->player.pos.y = SCREEN_HEIGHT - 50;
      }
    }

    // Paddle collision
    if (game->ball.pos.y + BALL_SIZE >= game->player.pos.y &&
        game->ball.pos.y <= game->player.pos.y + PADDLE_HEIGHT &&
        game->ball.pos.x + BALL_SIZE >= game->player.pos.x &&
        game->ball.pos.x <= game->player.pos.x + PADDLE_WIDTH) {
      // If the ball collides with the paddle, we reverse its y velocity to make it bounce back upwards towards the bricks
      //We also check if the ball is moving downwards before reversing the velocity to prevent it from getting stuck inside the paddle as well
      if (game->ball.vel.y > 0) {
        game->ball.vel.y *= -1;
      }
    }

    // initialize variable to track if a brick was hit during this cycle
    bool hit_brick = false;

    for (int r = 0; r < BRICK_ROWS && !hit_brick; r++) {// loop thru each row of bricks and check for collision of brick and ball
      for (int c = 0; c < BRICK_COLS && !hit_brick; c++) { // loop thru each column of bricks and check for collision of brick and ball
        int index = r * BRICK_COLS + c; // 1D index for the brick array based on the current row and column

        // Only check for collision if the brick is active
        if (game->bricks[index] == CELL_ACTIVE) {
          // Calculate the x and y position of the current brick based on its row and column
          float brick_x = BRICK_OFFSET_X + c * 78;
          float brick_y = BRICK_OFFSET_Y + r * 35;

          // If the ball collides with the current brick, we set that brick to empty to remove it from the game and also reverse the ball's y velocity to make it bounce back downwards towards the paddle
          if (game->ball.pos.x + BALL_SIZE >= brick_x &&
              game->ball.pos.x <= brick_x + BRICK_WIDTH &&
              game->ball.pos.y + BALL_SIZE >= brick_y &&
              game->ball.pos.y <= brick_y + BRICK_HEIGHT) {
            game->bricks[index] = CELL_EMPTY;
            game->ball.vel.y *= -1;
            hit_brick = true;
          }
        }
      }
    }

    int remaining_bricks = 0;

    for (int i = 0; i < BRICK_ROWS * BRICK_COLS; i++) {
      if (game->bricks[i] == CELL_ACTIVE) {
        remaining_bricks++;
      }
    }

    if (remaining_bricks == 0) {
      game->game_won = true;
    }
  }
}

void render_game(game_t* game) {
  // Clear the screen
  SDL_RenderClear(game->renderer);

  if (game->bg_texture != NULL) {
    SDL_RenderCopy(game->renderer, game->bg_texture, NULL, NULL);
  }

  // Draw bricks
  SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);

  // Loop through bricks and draw active ones
  for (int r = 0; r < BRICK_ROWS; r++) {
    for (int c = 0; c < BRICK_COLS; c++) {
      // Calculate the index in the brocks array
      int index = r * BRICK_COLS + c;

      // If the brick is active then it is drawn
      if (game->bricks[index] == CELL_ACTIVE) {
        SDL_Rect brick_rect = {
          .x = BRICK_OFFSET_X + c * 78,
          .y = BRICK_OFFSET_Y + r * 35,
          .w = BRICK_WIDTH,
          .h = BRICK_HEIGHT
        };

        SDL_RenderFillRect(game->renderer, &brick_rect);
      }
    }
  }

  // Draw paddle
  SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

  SDL_Rect paddle_rect = {
    .x = game->player.pos.x,
    .y = game->player.pos.y,
    .w = PADDLE_WIDTH,
    .h = PADDLE_HEIGHT
  };

  SDL_RenderFillRect(game->renderer, &paddle_rect);

  // Draw ball
  SDL_Rect ball_rect = {
    .x = game->ball.pos.x,
    .y = game->ball.pos.y,
    .w = BALL_SIZE,
    .h = BALL_SIZE
  };

  SDL_RenderFillRect(game->renderer, &ball_rect);

  if (game->game_won) {
    SDL_SetWindowTitle(game->window, "Breakout - YOU WIN!");
  } else if (game->game_over) {
    SDL_SetWindowTitle(game->window, "Breakout - GAME OVER");
  } else if (game->lives == 3) {
    SDL_SetWindowTitle(game->window, "Breakout - Lives: 3");
  } else if (game->lives == 2) {
    SDL_SetWindowTitle(game->window, "Breakout - Lives: 2");
  } else if (game->lives == 1) {
    SDL_SetWindowTitle(game->window, "Breakout - Lives: 1");
  }

  // Show everything
  SDL_RenderPresent(game->renderer);
}

void cleanup_game(game_t* game) {
  // Clean up
  free(game->bricks);

  if (game->bg_texture != NULL) {
    SDL_DestroyTexture(game->bg_texture);
  }

  IMG_Quit();

  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  SDL_Quit();
}

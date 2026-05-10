#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FRAME_RATE 60

#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 15
#define PADDLE_SPEED 500

#define BALL_SIZE 15
#define BALL_SPEED_X 300
#define BALL_SPEED_Y 300

#define BRICK_ROWS 6
#define BRICK_COLS 10
#define BRICK_WIDTH 75
#define BRICK_HEIGHT 30
#define BRICK_OFFSET_X 10
#define BRICK_OFFSET_Y 50

typedef enum brick_state {
  CELL_EMPTY,
  CELL_ACTIVE
} brick_state_t;

typedef struct vec {
  float x;
  float y;
} vec_t;

typedef struct paddle {
  vec_t pos;
  float vel;
} paddle_t;

typedef struct ball {
  vec_t pos;
  vec_t vel;
} ball_t;

int main(int argc, char** argv) {
    // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    // Something went wrong. Log an error and exit.
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

    // Create an SDL window
  SDL_Window* window = SDL_CreateWindow(
    "Breakout - Final Project",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );

  if (window == NULL) {
    //// Something went wrong. Log an error, shut down SDL, and then exit
    fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  // Create an SDL renderer
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  // Check if the renderer was created successfully
  if (renderer == NULL) {
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  IMG_Init(IMG_INIT_PNG);

  SDL_Surface* bg_surface = IMG_Load("background.png");

  SDL_Texture* bg_texture = NULL;

  if (bg_surface != NULL) {
    bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);
    SDL_FreeSurface(bg_surface);
  } else {
    printf("Could not load background image\n");
  }
  // Initialize brick states
  brick_state_t* bricks = malloc(sizeof(brick_state_t) * BRICK_ROWS * BRICK_COLS);
  // Check if memory allocation was successful
  if (bricks == NULL) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }
  // Set all bricks to active
  for (int i = 0; i < BRICK_ROWS * BRICK_COLS; i++) {
    bricks[i] = CELL_ACTIVE;
  }

  paddle_t player = {
    .pos = { .x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, .y = SCREEN_HEIGHT - 50 },
    .vel = 0
  };

  ball_t ball = {
    .pos = { .x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT / 2 },
    .vel = { .x = BALL_SPEED_X, .y = BALL_SPEED_Y }
  };

  bool left_pressed = false;
  bool right_pressed = false;

  int lives = 2;

  bool running = true;
  // Start the game loop
  while (running) {
    // Handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        else if (event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
        }

        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_LEFT ||
                event.key.keysym.sym == SDLK_a) {
            left_pressed = true;
            }

            if (event.key.keysym.sym == SDLK_RIGHT ||
                event.key.keysym.sym == SDLK_d) {
            right_pressed = true;
            }
        }

        else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_LEFT ||
                event.key.keysym.sym == SDLK_a) {
            left_pressed = false;
            }

            if (event.key.keysym.sym == SDLK_RIGHT ||
                event.key.keysym.sym == SDLK_d) {
            right_pressed = false;
            }
        }
    }

    // Update paddle
    if (left_pressed) {
        player.pos.x -= PADDLE_SPEED / (float)FRAME_RATE;
    }

    if (right_pressed) {
        player.pos.x += PADDLE_SPEED / (float)FRAME_RATE;
    }

    if (player.pos.x < 0) {
        player.pos.x = 0;
    }

    if (player.pos.x > SCREEN_WIDTH - PADDLE_WIDTH) {
        player.pos.x = SCREEN_WIDTH - PADDLE_WIDTH;
    }

    // Update ball
    ball.pos.x += ball.vel.x / (float)FRAME_RATE;
    ball.pos.y += ball.vel.y / (float)FRAME_RATE;

    // Check for collisions with walls
    if (ball.pos.x <= 0 || ball.pos.x >= SCREEN_WIDTH - BALL_SIZE) {
        ball.vel.x *= -1;
    }

    if (ball.pos.y <= 0) {
        ball.vel.y *= -1;
    }

    if (ball.pos.y >= SCREEN_HEIGHT) {
        lives--;

        if (lives <= 0) {
            running = false;
        } else {
            ball.pos.x = SCREEN_WIDTH / 2;
            ball.pos.y = SCREEN_HEIGHT / 2;

            ball.vel.x = BALL_SPEED_X;
            ball.vel.y = BALL_SPEED_Y;

            player.pos.x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2;
            player.pos.y = SCREEN_HEIGHT - 50;
        }
    }

    // Paddle collision
    if (ball.pos.y + BALL_SIZE >= player.pos.y &&
        ball.pos.y <= player.pos.y + PADDLE_HEIGHT &&
        ball.pos.x + BALL_SIZE >= player.pos.x &&
        ball.pos.x <= player.pos.x + PADDLE_WIDTH) {

        if (ball.vel.y > 0) {
            ball.vel.y *= -1;
        }
    }

    bool hit_brick = false;

    for (int r = 0; r < BRICK_ROWS && !hit_brick; r++) {
        for (int c = 0; c < BRICK_COLS && !hit_brick; c++) {
            int index = r * BRICK_COLS + c;

            if (bricks[index] == CELL_ACTIVE) {

                float brick_x = BRICK_OFFSET_X + c * 78;
                float brick_y = BRICK_OFFSET_Y + r * 35;

                if (ball.pos.x + BALL_SIZE >= brick_x &&
                    ball.pos.x <= brick_x + BRICK_WIDTH &&
                    ball.pos.y + BALL_SIZE >= brick_y &&
                    ball.pos.y <= brick_y + BRICK_HEIGHT) {

                        bricks[index] = CELL_EMPTY;

                        ball.vel.y *= -1;

                        hit_brick = true;
                }
            }
        }
    }

    // Clear the screen
    SDL_RenderClear(renderer);

    if (bg_texture != NULL) {
    SDL_RenderCopy(renderer, bg_texture, NULL, NULL);
    }

    // Draw bricks
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Loop through bricks and draw active ones
    for (int r = 0; r < BRICK_ROWS; r++) {
        for (int c = 0; c < BRICK_COLS; c++) {
            // Calculate the index in the brocks array
            int index = r * BRICK_COLS + c;
            // If the brick is active then it is drawn
            if (bricks[index] == CELL_ACTIVE) {
                SDL_Rect brick_rect = {
                    .x = BRICK_OFFSET_X + c * 78,
                    .y = BRICK_OFFSET_Y + r * 35,
                    .w = BRICK_WIDTH,
                    .h = BRICK_HEIGHT
                };

                SDL_RenderFillRect(renderer, &brick_rect);
            }
        }
    }

    // Draw paddle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect paddle_rect = {
        .x = player.pos.x,
        .y = player.pos.y,
        .w = PADDLE_WIDTH,
        .h = PADDLE_HEIGHT
    };

    SDL_RenderFillRect(renderer, &paddle_rect);

    // Draw ball
    SDL_Rect ball_rect = {
        .x = ball.pos.x,
        .y = ball.pos.y,
        .w = BALL_SIZE,
        .h = BALL_SIZE
    };

    SDL_RenderFillRect(renderer, &ball_rect);

    if (lives == 2) {
        SDL_SetWindowTitle(window, "Breakout - Lives: 2");
    } else if (lives == 1) {
        SDL_SetWindowTitle(window, "Breakout - Lives: 1");
    } else {
        SDL_SetWindowTitle(window, "Breakout - Game Over");
    }

    // Show everything
    SDL_RenderPresent(renderer);

    SDL_Delay(1000 / FRAME_RATE);
  }

  // Clean up 
  free(bricks);

  if (bg_texture != NULL) {
    SDL_DestroyTexture(bg_texture);
  }

  IMG_Quit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>

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
#define BRICK_WIDTH 70
#define BRICK_HEIGHT 30
#define BRICK_OFFSET_X 50
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

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  if (renderer == NULL) {
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  paddle_t player = {
    .pos = { .x = SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, .y = SCREEN_HEIGHT - 50 },
    .vel = 0
  };

  ball_t ball = {
    .pos = { .x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT / 2 },
    .vel = { .x = BALL_SPEED_X, .y = BALL_SPEED_Y }
  };

  bool running = true;
    // Start the game loop
  while (running) {
    // Handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        // When the user clicks the "X" in the window we get an SDL_QUIT event
        running = false;
      }
    }

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(1000 / FRAME_RATE);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
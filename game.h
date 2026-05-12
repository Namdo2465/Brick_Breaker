#pragma once

#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

// First, we define constants for the game
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FRAME_RATE 60

// Paddle for reflecting the ball constants
#define PADDLE_WIDTH 100
#define PADDLE_HEIGHT 15
#define PADDLE_SPEED 500

//Ball constants
#define BALL_SIZE 15
#define BALL_SPEED_X 300
#define BALL_SPEED_Y 300

// Brick wall constants
#define BRICK_ROWS 6
#define BRICK_COLS 10
#define BRICK_WIDTH 75
#define BRICK_HEIGHT 30
#define BRICK_OFFSET_X 10
#define BRICK_OFFSET_Y 50

// Bricks can either be empty or active
typedef enum brick_state {
  CELL_EMPTY,
  CELL_ACTIVE
} brick_state_t;

// We define a vector struct to represent positions and velocities in 2D space
typedef struct vec {
  float x;
  float y;
} vec_t;

//The paddle has a position and a velocity
typedef struct paddle {
  vec_t pos;
  float vel;
} paddle_t;

//The ball has a position and a velocity
typedef struct ball {
  vec_t pos;
  vec_t vel;
} ball_t;

// The game struct holds all the state for the game, including the SDL window and renderer, the background texture, the state of the bricks, the player paddle, the ball, input states for left and right movement, the number of lives remaining, and flags for game over and game won states
typedef struct game {
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* bg_texture;
  brick_state_t* bricks;
  paddle_t player;
  ball_t ball;
  bool left_pressed;
  bool right_pressed;
  int lives;
  bool game_over;
  bool game_won;
  bool running;
} game_t;

// Initializes SDL, creates the window and renderer.
bool setup_sdl(game_t* game);

// Loads the background image and texture.
bool setup_background(game_t* game);

// Allocates memory for the bricks and sets them active.
bool setup_bricks(game_t* game);

// Initializes the paddle, ball, lives, and game state.
void setup_game_state(game_t* game);

// Handles keyboard input and quit events.
void handle_events(game_t* game);

// Updates movement, collisions, lives, and win/loss states.
void update_game(game_t* game);

// Draws the background, bricks, paddle, and ball to the screen.
void render_game(game_t* game);

// Frees memory and destroys SDL resources.
void cleanup_game(game_t* game);
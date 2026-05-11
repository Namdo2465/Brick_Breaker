# Breakout Game

A classic Breakout/Brick Breaker game written in C using SDL2 library.

## Overview

This is an implementation of the classic arcade game "Breakout" where the player controls a paddle to bounce a ball and break bricks. The goal is to break all bricks without letting the ball fall off the bottom of the screen.

## Game Features

- **Classic Gameplay**: Control a paddle to bounce the ball and break bricks
- **Multiple Bricks**: 60 bricks arranged in a 6x10 grid
- **Lives System**: Start with 3 lives; lose a life when the ball falls off the bottom
- **Win/Lose Conditions**:
  - Win by breaking all bricks
  - Lose when all lives are depleted
- **Smooth Graphics**: Rendered with SDL2 and includes background image support
- **Frame Rate Control**: Runs at 60 FPS

## Requirements

- **C Compiler**: clang (or any C compiler supporting C99)
- **SDL2**: Simple DirectMedia Layer 2 library
- **SDL2_image**: SDL2 image loading extension
- **pkg-config**: For SDL2 configuration

### Installation on macOS (using Homebrew)

```bash
brew install sdl2 sdl2_image
```

### Installation on Ubuntu/Debian

```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev
```

## Building

The project includes a Makefile for easy building:

```bash
make
```

This will compile `breakout.c` into an executable named `breakout` with the following flags:

- `-g`: Enable debugging symbols
- `-Wall`: Show all warnings
- `-fsanitize=address`: Enable address sanitizer for memory error detection

### Clean Build

To remove the compiled executable:

```bash
make clean
```

## Running

After building, run the game with:

```bash
./breakout
```

Make sure `background.png` is in the same directory as the executable for the background image to load properly.

## Controls

- **Left Arrow** or **A**: Move paddle left
- **Right Arrow** or **D**: Move paddle right
- **ESC** or **Close Window**: Quit the game

## Game Mechanics

### Paddle

- Moves horizontally across the bottom of the screen
- Speed: 500 pixels per second
- Width: 100 pixels, Height: 15 pixels
- Constrained to screen boundaries

### Ball

- Starts at the center of the screen
- Initial velocity: 300 pixels/second in both X and Y directions
- Bounces off walls, paddle, and bricks
- If it falls below the screen, you lose a life

### Bricks

- Grid: 6 rows × 10 columns (60 total)
- Size: 75×30 pixels each
- One hit destroys a brick
- Break all bricks to win the game

### Lives

- Start with 3 lives
- Window title displays current lives or game status
- Lose a life when ball falls off bottom
- Ball resets to center when a life is lost (ball and paddle respawn)

## Code Structure

### Main Components

1. **SDL Initialization**: Sets up the SDL window, renderer, and image loading
2. **Game State**: Tracks paddle position, ball position/velocity, brick states, and lives
3. **Game Loop**:
   - Handles input (keyboard events)
   - Updates game objects (paddle, ball, collision detection)
   - Renders graphics
   - Maintains 60 FPS frame rate
4. **Collision Detection**:
   - Wall collisions (left, right, top)
   - Paddle collision
   - Brick collisions
   - Game over condition (ball falls off bottom)

### Data Structures

- **`vec_t`**: 2D vector with x, y components (for positions and velocities)
- **`paddle_t`**: Paddle with position and velocity
- **`ball_t`**: Ball with position and velocity
- **`brick_state_t`**: Enum to track if a brick is empty or active

## Game Constants

```c
SCREEN_WIDTH   = 800           // Pixels
SCREEN_HEIGHT  = 600           // Pixels
FRAME_RATE     = 60            // FPS

PADDLE_WIDTH   = 100           // Pixels
PADDLE_HEIGHT  = 15            // Pixels
PADDLE_SPEED   = 500           // Pixels per second

BALL_SIZE      = 15            // Pixels
BALL_SPEED_X   = 300           // Pixels per second
BALL_SPEED_Y   = 300           // Pixels per second

BRICK_ROWS     = 6
BRICK_COLS     = 10
BRICK_WIDTH    = 75            // Pixels
BRICK_HEIGHT   = 30            // Pixels
```

## Resources

- SDL2 Documentation: https://wiki.libsdl.org/
- SDL2_image: https://www.libsdl.org/projects/SDL_image/

## Author

Namdo

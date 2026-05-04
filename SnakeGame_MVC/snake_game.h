/*
 * snake_game.h
 *
 * Main header file for Snake Game - MVC Edition
 *
 * ============================================================================
 * MVC ARCHITECTURE
 * ============================================================================
 *
 * MODEL  (model.cpp)
 *   - Owns all game data: Snake, Food, GameState
 *   - Contains all game logic: movement, collision, scoring, spawning
 *   - Has zero knowledge of rendering or input
 *
 * VIEW   (view.cpp)
 *   - Reads data from the Model (read-only) and draws to screen
 *   - Never modifies game state
 *   - All raylib draw calls live here and only here
 *
 * CONTROLLER (controller.cpp)
 *   - Polls user input and translates it into Model commands
 *   - Coordinates the game loop: calls Model update, then View render
 *   - Bridges Model and View without coupling them to each other
 *
 * ============================================================================
 */

#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "raylib.h"
#include <stdbool.h>

// ============================================================================
// GAME CONFIGURATION CONSTANTS
// ============================================================================

constexpr int MAX_SNAKE_LENGTH = 400;
constexpr int SQUARE_SIZE      = 31;
constexpr int SCREEN_WIDTH     = 800;
constexpr int SCREEN_HEIGHT    = 450;
constexpr int TARGET_FPS       = 30;
constexpr int MOVE_FRAME_DELAY = 5;
constexpr int FREEZE_DURATION  = 60;

// ============================================================================
// DATA STRUCTURES  (owned by the Model)
// ============================================================================

struct Position {
    float x;
    float y;
};

struct SnakeSegment {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color   color;
};

struct Food {
    Vector2 position;
    Vector2 size;
    bool    active;
    Color   color;
};

struct Snake {
    SnakeSegment segments[MAX_SNAKE_LENGTH];
    Vector2      segmentPositions[MAX_SNAKE_LENGTH];
    int          length;
    bool         allowMove;
};

struct GameState {
    int     framesCounter;
    int     playerScore;
    bool    isGameOver;
    bool    isPaused;
    int     freezeCounter;
    Vector2 gridOffset;
};

// ============================================================================
// INPUT COMMAND  (produced by Controller, consumed by Model)
// ============================================================================

/*
 * InputCommand decouples the Controller from the Model.
 * The Controller translates raw key presses into an InputCommand struct.
 * The Model reads from InputCommand — it never calls IsKeyPressed() itself.
 */
enum class Direction { None, Up, Down, Left, Right };

struct InputCommand {
    Direction moveDirection;   // Desired snake direction (None = no change)
    bool      togglePause;     // True if pause key was pressed this frame
    bool      restart;         // True if restart key was pressed this frame
};

// ============================================================================
// MODEL  —  snake_model.h interface
// ============================================================================

void Model_Initialize(void);
void Model_Update(const InputCommand& cmd);

// Read-only accessors (used by View and Controller)
const Snake*     Model_GetSnake(void);
const Food*      Model_GetFood(void);
const GameState* Model_GetGameState(void);

// ============================================================================
// VIEW  —  snake_view.h interface
// ============================================================================

void View_Render(const Snake* snake, const Food* food, const GameState* state);

// ============================================================================
// CONTROLLER  —  snake_controller.h interface
// ============================================================================

void Controller_Run(void);          // Full game loop (init → loop → cleanup)

// Internal helpers exposed for testing / web platform entry point
InputCommand Controller_PollInput(const Snake* snake, const GameState* state);
void         Controller_Tick(void); // One frame: poll → update model → render

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

int     Utils_GetGridColumns(void);
int     Utils_GetGridRows(void);
Vector2 Utils_CalculateGridOffset(void);
bool    Utils_IsPositionValid(Vector2 position, Vector2 gridOffset);

#endif // SNAKE_GAME_H

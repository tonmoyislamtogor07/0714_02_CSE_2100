/*
 * snake_game.h
 *
 * Main header file for Snake Game (C++ Edition)
 * Contains type definitions, constants, interfaces, and class declarations
 *
 * Course: Advanced Programming Lab
 * Date: February 2026
 *
 * ============================================================================
 * SOLID PRINCIPLES APPLIED IN THIS FILE
 * ============================================================================
 *
 * [SRP] Single Responsibility Principle:
 *   - Each struct/class is responsible for one concept only:
 *     Position -> holds 2D coordinates
 *     SnakeSegment -> holds one snake body piece
 *     Food -> holds food entity data
 *     Snake -> holds all snake segments + behaviour
 *     GameState -> holds game-level counters and flags only
 *
 * [OCP] Open/Closed Principle:
 *   - IRenderable and IUpdatable interfaces allow new game entities to be
 *     added without modifying existing rendering or update code.
 *
 * [LSP] Liskov Substitution Principle:
 *   - IRenderable / IUpdatable pure-virtual interfaces are designed so that
 *     any subtype can safely replace the base type wherever used.
 *
 * [ISP] Interface Segregation Principle:
 *   - IRenderable (render only) and IUpdatable (update only) are kept
 *     separate so classes implement only what they need — a render-only
 *     entity does not have to carry an update method.
 *
 * [DIP] Dependency Inversion Principle:
 *   - High-level modules (Game) depend on abstractions (IRenderable,
 *     IUpdatable) rather than concrete classes, decoupling the game
 *     loop from implementation details of each entity.
 * ============================================================================
 */

#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include "raylib.h"
#include <stdbool.h>

// ============================================================================
// GAME CONFIGURATION CONSTANTS
// ============================================================================

constexpr int MAX_SNAKE_LENGTH  = 400;
constexpr int SQUARE_SIZE       = 31;
constexpr int SCREEN_WIDTH      = 800;
constexpr int SCREEN_HEIGHT     = 450;
constexpr int TARGET_FPS        = 30;
constexpr int MOVE_FRAME_DELAY  = 5;
constexpr int FREEZE_DURATION   = 60;   // Frames to freeze before game over

// ============================================================================
// TYPE DEFINITIONS
// ============================================================================

/*
 * [SRP] Position is solely responsible for representing a 2D coordinate.
 */
struct Position {
    float x;
    float y;
};

/*
 * [SRP] SnakeSegment holds data for exactly one body piece.
 */
struct SnakeSegment {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color   color;
};

/*
 * [SRP] Food holds state for the food entity only.
 */
struct Food {
    Vector2 position;
    Vector2 size;
    bool    active;
    Color   color;
};

/*
 * [SRP] Snake aggregates all segments and movement metadata.
 */
struct Snake {
    SnakeSegment segments[MAX_SNAKE_LENGTH];
    Vector2      segmentPositions[MAX_SNAKE_LENGTH];
    int          length;
    bool         allowMove;
};

/*
 * [SRP] GameState holds only game-level counters and flags.
 *       It knows nothing about entities — they live in game.cpp.
 */
struct GameState {
    int     framesCounter;
    int     playerScore;
    bool    isGameOver;
    bool    isPaused;
    int     freezeCounter;
    Vector2 gridOffset;
};

// ============================================================================
// ABSTRACTIONS (interfaces via pure-virtual structs)
//
// [OCP] New renderable or updatable entities can be added without touching
//       existing Game, Renderer, or other module code.
//
// [ISP] Rendering and updating concerns are segregated into separate
//       interfaces so that render-only entities are not forced to implement
//       update logic and vice-versa.
//
// [DIP] High-level game loop code depends on these abstractions, not on
//       concrete Snake / Food / Renderer types.
// ============================================================================

/*
 * IRenderable — anything that can draw itself to the screen.
 * [ISP] Kept separate from IUpdatable.
 */
struct IRenderable {
    virtual void render() const = 0;
    virtual ~IRenderable() = default;
};

/*
 * IUpdatable — anything that can advance its own state.
 * [ISP] Kept separate from IRenderable.
 */
struct IUpdatable {
    virtual void update(int framesCounter) = 0;
    virtual ~IUpdatable() = default;
};

// ============================================================================
// CORE GAME FUNCTIONS
// ============================================================================

void Game_Initialize(void);
void Game_Update(void);
void Game_Render(void);
void Game_Cleanup(void);
void Game_UpdateAndDraw(void);

// ============================================================================
// SNAKE MODULE FUNCTIONS
// ============================================================================

void Snake_Initialize(Snake* snake, Vector2 startPosition, Vector2 gridOffset);
void Snake_UpdatePosition(Snake* snake, int framesCounter);
void Snake_ProcessInput(Snake* snake);
void Snake_HandleWrapAround(Snake* snake, Vector2 gridOffset);
bool Snake_CheckSelfCollision(const Snake* snake);
void Snake_Grow(Snake* snake);
void Snake_Render(const Snake* snake);

// ============================================================================
// FOOD MODULE FUNCTIONS
// ============================================================================

void Food_Initialize(Food* food);
void Food_Spawn(Food* food, const Snake* snake, Vector2 gridOffset);
bool Food_CheckCollision(const Food* food, Vector2 position);
void Food_Render(const Food* food);

// ============================================================================
// COLLISION MODULE FUNCTIONS
// ============================================================================

bool Collision_CheckSnakeWithFood(const Snake* snake, const Food* food);
bool Collision_CheckSnakeWithSelf(const Snake* snake);

// ============================================================================
// RENDERING MODULE FUNCTIONS
// ============================================================================

void Renderer_DrawGrid(Vector2 gridOffset);
void Renderer_DrawGameOver(int finalScore);
void Renderer_DrawPauseScreen(void);
void Renderer_DrawFreezeEffect(void);

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

int     Utils_GetGridColumns(void);
int     Utils_GetGridRows(void);
Vector2 Utils_CalculateGridOffset(void);
bool    Utils_IsPositionValid(Vector2 position, Vector2 gridOffset);

#endif // SNAKE_GAME_H

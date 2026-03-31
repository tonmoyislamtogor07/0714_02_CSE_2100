/*
 * snake.cpp
 *
 * Snake entity management (C++ Edition)
 * Handles snake movement, input processing, collision detection, and rendering
 *
 * Course: Advanced Programming Lab
 * Date: February 2026
 *
 * ============================================================================
 * SOLID PRINCIPLES APPLIED IN THIS FILE
 * ============================================================================
 *
 * [SRP] Single Responsibility Principle:
 *   - Each function manages exactly one concern:
 *       Snake_Initialize    -> set-up/reset only
 *       Snake_ProcessInput  -> keyboard polling only
 *       Snake_UpdatePosition-> movement arithmetic only
 *       Snake_HandleWrapAround -> boundary wrap logic only
 *       Snake_CheckSelfCollision -> body-hit detection only
 *       Snake_Grow          -> length extension only
 *       Snake_Render        -> drawing only
 *   - No single function reaches into food, game state, or the renderer.
 *
 * [OCP] Open/Closed Principle:
 *   - Snake behaviour is extended (e.g. new movement modes) by adding new
 *     functions or overloads rather than modifying the existing ones.
 *   - The Snake struct itself is closed for modification once published.
 *
 * [LSP] Liskov Substitution Principle:
 *   - Snake_Render accepts a `const Snake*`, matching the IRenderable
 *     contract: any call site can swap in a derived snake type without
 *     breaking the renderer.
 *
 * [ISP] Interface Segregation Principle:
 *   - Callers that only need rendering include only Snake_Render.
 *   - Callers that only need input include only Snake_ProcessInput.
 *   - No monolithic "DoEverything" function forces callers to depend on
 *     operations they do not need.
 *
 * [DIP] Dependency Inversion Principle:
 *   - Snake functions depend on the abstract Vector2 / Color types from
 *     raylib (stable, low-level types) and on Utils_* helper abstractions
 *     rather than on raw screen-dimension magic numbers.
 * ============================================================================
 */

#include "snake_game.h"
#include <cassert>

// ============================================================================
// SNAKE INITIALIZATION
// [SRP] Snake_Initialize is solely responsible for setting up the snake to a
//       clean initial state.  It touches nothing outside the Snake struct.
// ============================================================================

/*
 * Initialize snake with starting position and configuration.
 *
 * @param snake         - Pointer to snake structure to initialise
 * @param startPosition - Initial position for snake head
 * @param gridOffset    - Grid offset for proper positioning
 */
void Snake_Initialize(Snake* snake, Vector2 startPosition, Vector2 gridOffset)
{
    // [SRP] Pre-condition check: this function owns the validity guarantee
    assert(snake != nullptr);

    snake->length    = 1;
    snake->allowMove = false;

    // Initialise every segment to the same starting tile
    for (int i = 0; i < MAX_SNAKE_LENGTH; i++)
    {
        // [OCP] Segment defaults are centralised here; changing them for all
        //       segments does not require touching any other function.
        snake->segments[i].position = { gridOffset.x, gridOffset.y };
        snake->segments[i].size     = { static_cast<float>(SQUARE_SIZE),
                                         static_cast<float>(SQUARE_SIZE) };
        snake->segments[i].speed    = { static_cast<float>(SQUARE_SIZE), 0.0f };

        // Head is blue, all body segments are sky blue
        snake->segments[i].color    = (i == 0) ? BLUE : SKYBLUE;

        snake->segmentPositions[i]  = { 0.0f, 0.0f };
    }
}

// ============================================================================
// INPUT PROCESSING
// [SRP] Snake_ProcessInput has one job: translate keyboard state into a new
//       head speed vector.  It does not move the snake or touch game state.
// [ISP] Callers that do not need input (e.g. a replay system) never call this
//       function — it is cleanly separated from update and render.
// ============================================================================

/*
 * Process keyboard input for snake direction.
 * Prevents 180-degree turns (moving directly backwards).
 *
 * @param snake - Pointer to snake to control
 */
void Snake_ProcessInput(Snake* snake)
{
    assert(snake != nullptr);

    // [SRP] Local alias keeps the intent clear; we only ever modify the head
    SnakeSegment* head = &snake->segments[0];

    // [OCP] Each direction branch is independent; adding a new control scheme
    //       (gamepad, touch) means adding new branches, not editing these.
    if (IsKeyPressed(KEY_RIGHT) && (head->speed.x == 0.0f) && snake->allowMove)
    {
        head->speed   = { static_cast<float>(SQUARE_SIZE), 0.0f };
        snake->allowMove = false;
    }

    if (IsKeyPressed(KEY_LEFT) && (head->speed.x == 0.0f) && snake->allowMove)
    {
        head->speed   = { static_cast<float>(-SQUARE_SIZE), 0.0f };
        snake->allowMove = false;
    }

    if (IsKeyPressed(KEY_UP) && (head->speed.y == 0.0f) && snake->allowMove)
    {
        head->speed   = { 0.0f, static_cast<float>(-SQUARE_SIZE) };
        snake->allowMove = false;
    }

    if (IsKeyPressed(KEY_DOWN) && (head->speed.y == 0.0f) && snake->allowMove)
    {
        head->speed   = { 0.0f, static_cast<float>(SQUARE_SIZE) };
        snake->allowMove = false;
    }
}

// ============================================================================
// MOVEMENT AND POSITION UPDATE
// [SRP] Snake_UpdatePosition owns exactly one concern: advancing the snake's
//       position by one tick according to its current speed and the frame
//       timing constant.  Wrap-around is a separate concern handled elsewhere.
// [DIP] Uses the MOVE_FRAME_DELAY constant abstraction rather than a hard-
//       coded literal so the timing policy can change without touching this
//       function's body.
// ============================================================================

/*
 * Update snake position based on current speed.
 * Moves snake forward and updates all body segments.
 *
 * @param snake         - Pointer to snake to update
 * @param framesCounter - Current frame count for timing
 */
void Snake_UpdatePosition(Snake* snake, int framesCounter)
{
    assert(snake != nullptr);

    // [SRP] Store previous positions — this sub-step belongs to movement only
    for (int i = 0; i < snake->length; i++)
    {
        snake->segmentPositions[i] = snake->segments[i].position;
    }

    // [OCP] Frame-gate is a single expression; swapping for a timer-based
    //       approach requires changing only this condition, nothing else.
    if ((framesCounter % MOVE_FRAME_DELAY) == 0)
    {
        for (int i = 0; i < snake->length; i++)
        {
            if (i == 0)
            {
                // Move head by its current speed vector
                snake->segments[0].position.x += snake->segments[0].speed.x;
                snake->segments[0].position.y += snake->segments[0].speed.y;
                snake->allowMove = true;
            }
            else
            {
                // [SRP] Body segments simply follow the segment ahead of them
                snake->segments[i].position = snake->segmentPositions[i - 1];
            }
        }
    }
}

// ============================================================================
// SCREEN WRAP-AROUND
// [SRP] Snake_HandleWrapAround is responsible solely for teleporting the head
//       to the opposite side of the grid when it exits the boundary.
// [DIP] Boundary limits are derived from Utils_GetGridColumns / Rows
//       abstractions, not raw screen-size literals.
// ============================================================================

/*
 * Handle screen wrap-around (snake teleports to opposite side).
 *
 * @param snake      - Pointer to snake to check
 * @param gridOffset - Grid offset for boundary calculation
 */
void Snake_HandleWrapAround(Snake* snake, Vector2 gridOffset)
{
    assert(snake != nullptr);

    // [DIP] Depend on utility abstractions, not on hard-coded constants
    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    const float minX = gridOffset.x;
    const float minY = gridOffset.y;
    const float maxX = gridOffset.x + static_cast<float>((cols - 1) * SQUARE_SIZE);
    const float maxY = gridOffset.y + static_cast<float>((rows - 1) * SQUARE_SIZE);

    // Horizontal wrap
    if      (snake->segments[0].position.x > maxX) snake->segments[0].position.x = minX;
    else if (snake->segments[0].position.x < minX) snake->segments[0].position.x = maxX;

    // Vertical wrap
    if      (snake->segments[0].position.y > maxY) snake->segments[0].position.y = minY;
    else if (snake->segments[0].position.y < minY) snake->segments[0].position.y = maxY;
}

// ============================================================================
// COLLISION DETECTION — SELF
// [SRP] Snake_CheckSelfCollision is responsible only for determining whether
//       the head occupies the same cell as any body segment.
// [ISP] Kept separate from food-collision so callers can opt into only the
//       checks they need.
// ============================================================================

/*
 * Check if snake head collides with its own body.
 *
 * @param snake - Pointer to snake to check
 * @return true if collision detected, false otherwise
 */
bool Snake_CheckSelfCollision(const Snake* snake)
{
    assert(snake != nullptr);

    for (int i = 1; i < snake->length; i++)
    {
        if ((snake->segments[0].position.x == snake->segments[i].position.x) &&
            (snake->segments[0].position.y == snake->segments[i].position.y))
        {
            return true;
        }
    }

    return false;
}

// ============================================================================
// GROWTH
// [SRP] Snake_Grow does one thing: append one segment to the snake's tail.
//       Score tracking and food deactivation are other modules' concerns.
// [OCP] To implement a "grow-by-N" feature, a new overload or parameter can
//       be added without changing this core function.
// ============================================================================

/*
 * Increase snake length by one segment.
 * Called when the snake eats food.
 *
 * @param snake - Pointer to snake to grow
 */
void Snake_Grow(Snake* snake)
{
    assert(snake != nullptr);
    assert(snake->length < MAX_SNAKE_LENGTH);

    if (snake->length < MAX_SNAKE_LENGTH)
    {
        // New segment starts at the position the tail occupied last tick
        snake->segments[snake->length].position =
            snake->segmentPositions[snake->length - 1];
        snake->length++;
    }
}

// ============================================================================
// RENDERING
// [SRP] Snake_Render has exactly one job: draw every active segment to the
//       screen.  It does not compute positions or handle input.
// [LSP] Accepts a `const Snake*` so it can safely be called anywhere a
//       read-only view of the snake is available.
// [ISP] Isolated from update logic; a replay visualiser can call this without
//       ever touching Snake_ProcessInput or Snake_UpdatePosition.
// ============================================================================

/*
 * Draw snake to screen.
 *
 * @param snake - Pointer to snake to render (const: render does not mutate)
 */
void Snake_Render(const Snake* snake)
{
    assert(snake != nullptr);

    for (int i = 0; i < snake->length; i++)
    {
        // [SRP] Each segment draws itself; no layout logic lives here
        DrawRectangleV(
            snake->segments[i].position,
            snake->segments[i].size,
            snake->segments[i].color
        );
    }
}

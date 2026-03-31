/*
 * food.cpp
 *
 * Food entity management (C++ Edition)
 * Handles food spawning, collision detection, and rendering
 *
 * Course: Advanced Programming Lab
 * Date: February 2026
 *
 * ============================================================================
 * SOLID PRINCIPLES APPLIED IN THIS FILE
 * ============================================================================
 *
 * [SRP] Single Responsibility Principle:
 *   - Food_Initialize  -> sets up a blank food entity only
 *   - Food_Spawn       -> picks a valid grid cell and activates food only
 *   - Food_CheckCollision -> checks whether a position matches food only
 *   - Food_Render      -> draws food to the screen only
 *   No function reaches into snake state updates, scoring, or game flow.
 *
 * [OCP] Open/Closed Principle:
 *   - Spawn strategy (random position) is encapsulated in Food_Spawn.
 *     Adding a deterministic or weighted spawn algorithm means introducing
 *     a new function / overload — existing code is closed for modification.
 *
 * [LSP] Liskov Substitution Principle:
 *   - Food_Render / Food_CheckCollision accept `const Food*` — any subtype
 *     of Food (e.g. a bonus fruit) can be substituted without side-effects.
 *
 * [ISP] Interface Segregation Principle:
 *   - Callers that only need collision checking (e.g. collision.cpp) depend
 *     solely on Food_CheckCollision and never on Food_Spawn or Food_Render.
 *
 * [DIP] Dependency Inversion Principle:
 *   - Food_Spawn depends on the Utils_GetGridColumns / Utils_GetGridRows
 *     abstractions rather than on raw SCREEN_WIDTH / SQUARE_SIZE arithmetic,
 *     so grid-layout policy can change without touching food logic.
 * ============================================================================
 */

#include "snake_game.h"
#include <cassert>

// ============================================================================
// FOOD INITIALIZATION
// [SRP] Food_Initialize is solely responsible for resetting a Food struct to
//       a safe, inactive starting state.
// ============================================================================

/*
 * Initialize food with default properties.
 * Food starts inactive and will be spawned during gameplay.
 *
 * @param food - Pointer to food structure to initialise
 */
void Food_Initialize(Food* food)
{
    assert(food != nullptr);

    // [SRP] Only touches the Food struct — no game state, no snake, no score
    food->size     = { static_cast<float>(SQUARE_SIZE),
                       static_cast<float>(SQUARE_SIZE) };
    food->color    = YELLOW;
    food->active   = false;
    food->position = { 0.0f, 0.0f };
}

// ============================================================================
// FOOD SPAWNING
// [SRP] Food_Spawn owns the single concern of placing food at a valid,
//       unoccupied grid cell.
// [OCP] The "avoid snake body" constraint is implemented here; a future
//       "avoid walls" constraint can be added without modifying callers.
// [DIP] Grid dimensions come from Utils_* abstractions, not magic numbers.
// ============================================================================

/*
 * Spawn food at a random grid position.
 * Ensures food does not overlap any snake segment.
 *
 * @param food       - Pointer to food to spawn
 * @param snake      - Pointer to snake (to avoid spawning on snake body)
 * @param gridOffset - Grid offset for screen-coordinate conversion
 */
void Food_Spawn(Food* food, const Snake* snake, Vector2 gridOffset)
{
    assert(food  != nullptr);
    assert(snake != nullptr);

    food->active = true;

    // [DIP] Depend on utility abstractions for grid dimensions
    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    // [SRP] Edge-case guard: grid completely filled — no valid cell exists
    if (snake->length >= cols * rows)
    {
        food->active = false;
        return;
    }

    int  randomX;
    int  randomY;
    bool positionValid;

    /*
     * [OCP] Spawn loop uses a rejection-sampling strategy.
     *       Swapping to a free-cell list approach requires changing only
     *       this block, leaving Food_Initialize / Food_Render untouched.
     */
    do
    {
        positionValid = true;

        // Generate random grid coordinates within legal range
        randomX = GetRandomValue(0, cols - 1);
        randomY = GetRandomValue(0, rows - 1);

        // Convert grid indices to screen-space coordinates
        food->position = {
            gridOffset.x + static_cast<float>(randomX * SQUARE_SIZE),
            gridOffset.y + static_cast<float>(randomY * SQUARE_SIZE)
        };

        // [SRP] Collision check against snake body — rejection condition only
        for (int i = 0; i < snake->length; i++)
        {
            if ((food->position.x == snake->segments[i].position.x) &&
                (food->position.y == snake->segments[i].position.y))
            {
                positionValid = false;
                break;
            }
        }
    }
    while (!positionValid);
}

// ============================================================================
// COLLISION DETECTION
// [SRP] Food_CheckCollision is responsible only for the geometric test:
//       does the food cell coincide with the given position?
// [ISP] Collision callers need this one function; they do not need Spawn or
//       Render, so those are never forced upon them.
// [LSP] Accepts `const Food*` — any read-only food view is safely accepted.
// ============================================================================

/*
 * Check if food collides with a given position.
 *
 * @param food     - Pointer to food (const: no mutation)
 * @param position - Position to check collision against
 * @return true if food is active and positions match, false otherwise
 */
bool Food_CheckCollision(const Food* food, Vector2 position)
{
    assert(food != nullptr);

    // [SRP] Inactive food can never collide — guard here, not at call sites
    if (!food->active)
    {
        return false;
    }

    return (food->position.x == position.x) &&
           (food->position.y == position.y);
}

// ============================================================================
// RENDERING
// [SRP] Food_Render has one job: draw the food if it is active.
//       Position, colour, and activity state are managed by other functions.
// [LSP] `const Food*` — render does not mutate; any subtype is safe to pass.
// [ISP] Separated from spawn and collision so callers needing only rendering
//       (e.g. a replay viewer) carry no dependency on spawn logic.
// ============================================================================

/*
 * Draw food to screen.
 * Only renders if food is active.
 *
 * @param food - Pointer to food to render (const: no mutation)
 */
void Food_Render(const Food* food)
{
    assert(food != nullptr);

    // [SRP] Activity check is part of rendering responsibility — whether to
    //       draw is owned by the render function, not by the caller.
    if (food->active)
    {
        DrawRectangleV(food->position, food->size, food->color);
    }
}

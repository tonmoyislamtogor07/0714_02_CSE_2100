/*
 * utils.cpp
 *
 * Utility functions module (C++ Edition)
 * Provides helper functions for calculations and common operations
 *
 * Course: Advanced Programming Lab
 * Date: February 2026
 *
 * ============================================================================
 * SOLID PRINCIPLES APPLIED IN THIS FILE
 * ============================================================================
 *
 * [SRP] Single Responsibility Principle:
 *   - Each function handles exactly one calculation:
 *       Utils_GetGridColumns      -> column count only
 *       Utils_GetGridRows         -> row count only
 *       Utils_CalculateGridOffset -> centering offset only
 *       Utils_IsPositionValid     -> bounds validation only
 *   - No utility function has side effects or modifies any external state.
 *
 * [OCP] Open/Closed Principle:
 *   - Grid layout logic (SCREEN_WIDTH / SQUARE_SIZE) is encapsulated in the
 *     two getter functions.  Changing the layout algorithm (e.g. supporting
 *     multiple resolutions) requires updating only those two functions — all
 *     callers that depend on Utils_GetGridColumns / Rows remain unchanged.
 *   - Utils_IsPositionValid can be extended (e.g. exclude border cells)
 *     without modifying the column/row getters.
 *
 * [LSP] Liskov Substitution Principle:
 *   - All functions are pure (no global mutation, deterministic output for
 *     the same inputs), making them safe to use wherever a stable utility
 *     function is expected.  A mock or test-double can substitute each one
 *     without breaking any contract.
 *
 * [ISP] Interface Segregation Principle:
 *   - Callers that only need column count call Utils_GetGridColumns; they are
 *     never forced to import the full position-validation logic.  Each util is
 *     a stand-alone function, not part of a bloated utility class with dozens
 *     of unrelated methods.
 *
 * [DIP] Dependency Inversion Principle:
 *   - All other modules (snake.cpp, food.cpp, renderer.cpp, game.cpp) depend
 *     on these utility abstractions rather than on raw arithmetic over
 *     SCREEN_WIDTH and SQUARE_SIZE.  This way, the grid-layout policy is
 *     expressed once here and consumed everywhere via a stable interface.
 * ============================================================================
 */

#include "snake_game.h"

// ============================================================================
// GRID DIMENSION CALCULATIONS
// [SRP] Each function owns one piece of grid geometry — nothing else.
// [DIP] Other modules call these functions instead of repeating the division
//       arithmetic, so they depend on this abstraction layer.
// ============================================================================

/*
 * Calculate number of columns in the game grid.
 * [SRP] Solely responsible for the column-count computation.
 * [OCP] Changing to a fixed column count or multi-resolution scheme only
 *       requires editing this one function.
 *
 * @return Number of columns that fit in the screen width
 */
int Utils_GetGridColumns(void)
{
    // [DIP] Expresses layout policy through named constants — callers do not
    //       need to know SCREEN_WIDTH or SQUARE_SIZE exist.
    return SCREEN_WIDTH / SQUARE_SIZE;
}

/*
 * Calculate number of rows in the game grid.
 * [SRP] Solely responsible for the row-count computation.
 *
 * @return Number of rows that fit in the screen height
 */
int Utils_GetGridRows(void)
{
    return SCREEN_HEIGHT / SQUARE_SIZE;
}

/*
 * Calculate grid offset to centre the grid on screen.
 * Handles cases where the grid does not perfectly fill the screen dimensions.
 * [SRP] Solely responsible for offset/centering arithmetic.
 * [OCP] A future multi-screen or HiDPI mode can update this function only.
 *
 * @return Vector2 containing x and y pixel offsets
 */
Vector2 Utils_CalculateGridOffset(void)
{
    // [DIP] Delegate column/row counts to the authoritative getter functions
    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    Vector2 offset;
    // [SRP] Integer division intentional — grid must align to whole pixels
    offset.x = static_cast<float>((SCREEN_WIDTH  - cols * SQUARE_SIZE) / 2);
    offset.y = static_cast<float>((SCREEN_HEIGHT - rows * SQUARE_SIZE) / 2);

    return offset;
}

// ============================================================================
// POSITION VALIDATION
// [SRP] Utils_IsPositionValid has one job: answer "is this cell within the
//       legal grid bounds?"  It does not move things or modify state.
// [DIP] Derives bounds from the getter abstractions, not raw constants.
// [ISP] Callers that only need bounds checking depend on this one function,
//       not on the full utils module.
// ============================================================================

/*
 * Check if a position is within valid grid bounds.
 * [SRP] Boundary calculation and validation are this function's only concern.
 *
 * @param position   - Position to validate (screen-space coordinates)
 * @param gridOffset - Grid offset for boundary calculation
 * @return true if position lies within the grid, false otherwise
 */
bool Utils_IsPositionValid(Vector2 position, Vector2 gridOffset)
{
    // [DIP] Use getter abstractions — not raw division arithmetic
    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    const float minX = gridOffset.x;
    const float minY = gridOffset.y;
    // Last valid cell starts at (max - 1) * SQUARE_SIZE from origin
    const float maxX = gridOffset.x + static_cast<float>((cols - 1) * SQUARE_SIZE);
    const float maxY = gridOffset.y + static_cast<float>((rows - 1) * SQUARE_SIZE);

    // [SRP] Single boolean expression — one validation responsibility
    return (position.x >= minX && position.x <= maxX &&
            position.y >= minY && position.y <= maxY);
}

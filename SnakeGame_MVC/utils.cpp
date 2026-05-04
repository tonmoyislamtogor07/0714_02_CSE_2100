/*
 * utils.cpp
 *
 * Utility / helper functions — shared across all MVC layers.
 * These are pure calculations with no side effects.
 */

#include "snake_game.h"

int Utils_GetGridColumns(void)
{
    return SCREEN_WIDTH / SQUARE_SIZE;
}

int Utils_GetGridRows(void)
{
    return SCREEN_HEIGHT / SQUARE_SIZE;
}

Vector2 Utils_CalculateGridOffset(void)
{
    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    Vector2 offset;
    offset.x = (float)((SCREEN_WIDTH  - cols * SQUARE_SIZE) / 2);
    offset.y = (float)((SCREEN_HEIGHT - rows * SQUARE_SIZE) / 2);
    return offset;
}

bool Utils_IsPositionValid(Vector2 position, Vector2 gridOffset)
{
    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    const float minX = gridOffset.x;
    const float minY = gridOffset.y;
    const float maxX = gridOffset.x + (float)((cols - 1) * SQUARE_SIZE);
    const float maxY = gridOffset.y + (float)((rows - 1) * SQUARE_SIZE);

    return (position.x >= minX && position.x <= maxX &&
            position.y >= minY && position.y <= maxY);
}

/*
 * view.cpp
 *
 * ============================================================================
 * MVC ROLE: VIEW
 * ============================================================================
 *
 * The View is responsible for ONE thing: drawing the current game state to
 * the screen. It:
 *   - Receives read-only pointers to Model data (Snake, Food, GameState)
 *   - Contains ALL raylib draw calls — no other file draws anything
 *   - Never modifies any data it receives
 *   - Has zero knowledge of input, key bindings, or game rules
 *
 * WHY THIS IS THE VIEW:
 *   In your original code, drawing was spread across snake.cpp (Snake_Render),
 *   food.cpp (Food_Render), renderer.cpp, and game.cpp (BeginDrawing/
 *   EndDrawing). In MVC, the View is a single, cohesive layer. All visual
 *   decisions — colors, layout, text — live here and nowhere else.
 *
 * ============================================================================
 */

#include "snake_game.h"

// ============================================================================
// PRIVATE DRAW HELPERS
// ============================================================================

static void DrawGrid(Vector2 gridOffset)
{
    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    for (int i = 0; i <= cols; i++)
        DrawLineV(
            { gridOffset.x + (float)(i * SQUARE_SIZE), gridOffset.y },
            { gridOffset.x + (float)(i * SQUARE_SIZE), gridOffset.y + (float)(rows * SQUARE_SIZE) },
            LIGHTGRAY
        );

    for (int i = 0; i <= rows; i++)
        DrawLineV(
            { gridOffset.x, gridOffset.y + (float)(i * SQUARE_SIZE) },
            { gridOffset.x + (float)(cols * SQUARE_SIZE), gridOffset.y + (float)(i * SQUARE_SIZE) },
            LIGHTGRAY
        );
}

static void DrawSnake(const Snake* snake)
{
    for (int i = 0; i < snake->length; i++)
        DrawRectangleV(
            snake->segments[i].position,
            snake->segments[i].size,
            snake->segments[i].color
        );
}

static void DrawFood(const Food* food)
{
    if (food->active)
        DrawRectangleV(food->position, food->size, food->color);
}

static void DrawScore(int score)
{
    // Small live score display in top-left corner
    DrawText(TextFormat("SCORE: %d", score), 10, 10, 20, WHITE);
}

static void DrawPauseOverlay(void)
{
    const char* txt = "GAME PAUSED";
    constexpr int fs = 40;
    DrawText(txt,
        SCREEN_WIDTH  / 2 - MeasureText(txt, fs) / 2,
        SCREEN_HEIGHT / 2 - 40,
        fs, GRAY);
}

static void DrawFreezeOverlay(void)
{
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(RED, 0.3f));
}

static void DrawGameOver(int finalScore)
{
    const char* title = "GAME OVER!";
    constexpr int titleFs = 40;
    DrawText(title,
        SCREEN_WIDTH  / 2 - MeasureText(title, titleFs) / 2,
        SCREEN_HEIGHT / 2 - 80,
        titleFs, RED);

    const char* score = TextFormat("FINAL SCORE: %d", finalScore);
    constexpr int scoreFs = 30;
    DrawText(score,
        SCREEN_WIDTH  / 2 - MeasureText(score, scoreFs) / 2,
        SCREEN_HEIGHT / 2 - 30,
        scoreFs, YELLOW);

    const char* restart = "PRESS [ENTER] TO PLAY AGAIN";
    constexpr int restartFs = 20;
    DrawText(restart,
        SCREEN_WIDTH  / 2 - MeasureText(restart, restartFs) / 2,
        SCREEN_HEIGHT / 2 + 20,
        restartFs, GRAY);
}

// ============================================================================
// PUBLIC VIEW API
// ============================================================================

/*
 * View_Render — draw one complete frame from the given Model data.
 *
 * MVC Note:
 *   - All three parameters are read-only (const pointers).
 *   - View_Render never calls Model_Update, InputCommand, or IsKeyPressed.
 *   - The Controller calls this after Model_Update; they are never reversed.
 */
void View_Render(const Snake* snake, const Food* food, const GameState* state)
{
    BeginDrawing();
    ClearBackground(BLACK);

    if (!state->isGameOver)
    {
        DrawGrid(state->gridOffset);
        DrawSnake(snake);
        DrawFood(food);
        DrawScore(state->playerScore);

        if (state->isPaused)      DrawPauseOverlay();
        if (state->freezeCounter) DrawFreezeOverlay();
    }
    else
    {
        DrawGameOver(state->playerScore);
    }

    EndDrawing();
}

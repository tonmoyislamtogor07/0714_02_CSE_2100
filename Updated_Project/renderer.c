/*
 * renderer.cpp
 *
 * Rendering module (C++ Edition)
 * Handles all drawing operations for UI and game elements
 *
 * Course: Advanced Programming Lab
 * Date: February 2026
 *
 * ============================================================================
 * SOLID PRINCIPLES APPLIED IN THIS FILE
 * ============================================================================
 *
 * [SRP] Single Responsibility Principle:
 *   - Every function is responsible for drawing exactly one visual element:
 *       Renderer_DrawGrid        -> draws the background grid lines only
 *       Renderer_DrawPauseScreen -> draws the pause overlay only
 *       Renderer_DrawFreezeEffect-> draws the crash red-tint only
 *       Renderer_DrawGameOver    -> draws the game-over UI only
 *   - No rendering function modifies game state, moves entities, or makes
 *     gameplay decisions — those belong in game.cpp.
 *
 * [OCP] Open/Closed Principle:
 *   - New UI overlays (e.g. high-score screen, level-up banner) are added as
 *     new Renderer_Draw* functions without modifying the existing ones.
 *   - Grid line colour or style can be changed inside Renderer_DrawGrid
 *     without touching any other draw function.
 *
 * [LSP] Liskov Substitution Principle:
 *   - All functions are pure draw calls with no return value and no mutation
 *     of shared state, so they can be called in any order or substituted with
 *     mock renderers in tests without violating expected behaviour.
 *
 * [ISP] Interface Segregation Principle:
 *   - Game logic that only needs the grid drawn never has to reference
 *     Renderer_DrawGameOver.  Each overlay is its own discrete function so
 *     callers depend only on what they actually invoke.
 *
 * [DIP] Dependency Inversion Principle:
 *   - Grid dimensions come from Utils_GetGridColumns / Utils_GetGridRows
 *     abstractions.  The renderer does not reach into SCREEN_WIDTH /
 *     SQUARE_SIZE arithmetic directly — it depends on the Utils interface.
 *   - Screen-centre positioning uses SCREEN_WIDTH / SCREEN_HEIGHT constants
 *     (low-level stable values), not on GameState, keeping the renderer
 *     decoupled from high-level game logic.
 * ============================================================================
 */

#include "snake_game.h"

// ============================================================================
// GRID RENDERING
// [SRP] Renderer_DrawGrid draws the background grid only.
// [DIP] Grid size is derived from Utils_* abstractions, not raw constants.
// ============================================================================

/*
 * Draw the game grid.
 * Renders vertical and horizontal lines to create a grid pattern.
 *
 * @param gridOffset - Offset for grid positioning on screen
 */
void Renderer_DrawGrid(Vector2 gridOffset)
{
    // [DIP] Depend on utility abstractions for layout dimensions
    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    // [SRP] Vertical lines — one loop, one responsibility
    for (int i = 0; i <= cols; i++)
    {
        DrawLineV(
            { gridOffset.x + static_cast<float>(i * SQUARE_SIZE), gridOffset.y },
            { gridOffset.x + static_cast<float>(i * SQUARE_SIZE),
              gridOffset.y + static_cast<float>(rows * SQUARE_SIZE) },
            LIGHTGRAY
        );
    }

    // [SRP] Horizontal lines — separate loop, same single concern
    for (int i = 0; i <= rows; i++)
    {
        DrawLineV(
            { gridOffset.x,
              gridOffset.y + static_cast<float>(i * SQUARE_SIZE) },
            { gridOffset.x + static_cast<float>(cols * SQUARE_SIZE),
              gridOffset.y + static_cast<float>(i * SQUARE_SIZE) },
            LIGHTGRAY
        );
    }
}

// ============================================================================
// PAUSE SCREEN OVERLAY
// [SRP] Renderer_DrawPauseScreen is responsible only for the pause banner.
// [OCP] Font size, text, and colour can be changed here independently of all
//       other renderer functions.
// ============================================================================

/*
 * Draw pause screen overlay.
 * Displayed when the game is paused.
 */
void Renderer_DrawPauseScreen(void)
{
    // [SRP] All layout constants are local — no shared mutable state
    const char* pauseText = "GAME PAUSED";
    constexpr int fontSize  = 40;

    const int textWidth = MeasureText(pauseText, fontSize);

    DrawText(
        pauseText,
        SCREEN_WIDTH  / 2 - textWidth / 2,
        SCREEN_HEIGHT / 2 - 40,
        fontSize,
        GRAY
    );
}

// ============================================================================
// FREEZE EFFECT OVERLAY
// [SRP] Renderer_DrawFreezeEffect solely renders the translucent red flash
//       that signals a crash is about to end the game.
// [OCP] Adjusting the colour or opacity requires only changing this function.
// ============================================================================

/*
 * Draw freeze effect overlay.
 * Red tint shown briefly after the snake crashes, before game over.
 */
void Renderer_DrawFreezeEffect(void)
{
    // [SRP] One rectangle covering the full screen — single visual concern
    DrawRectangle(
        0,
        0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        Fade(RED, 0.3f)
    );
}

// ============================================================================
// GAME OVER SCREEN
// [SRP] Renderer_DrawGameOver draws the full game-over UI: title, score, and
//       restart hint.  Score calculation is another module's responsibility;
//       this function only displays a value it receives.
// [DIP] Receives finalScore as a parameter — does not reach into GameState or
//       any global, keeping it decoupled from high-level game management.
// ============================================================================

/*
 * Draw game over screen.
 * Shows the final score and restart instructions.
 *
 * @param finalScore - Player's final score to display
 */
void Renderer_DrawGameOver(int finalScore)
{
    // ---- "GAME OVER!" title -----------------------------------------------
    // [SRP] Each text element is measured and drawn independently
    const char* gameOverText    = "GAME OVER!";
    constexpr int titleFontSize = 40;
    const int     titleWidth    = MeasureText(gameOverText, titleFontSize);

    DrawText(
        gameOverText,
        SCREEN_WIDTH  / 2 - titleWidth / 2,
        SCREEN_HEIGHT / 2 - 80,
        titleFontSize,
        RED
    );

    // ---- Final score display -----------------------------------------------
    // [DIP] TextFormat is a raylib abstraction — no direct sprintf dependency
    const char* scoreText      = TextFormat("FINAL SCORE: %d", finalScore);
    constexpr int scoreFontSize = 30;
    const int     scoreWidth    = MeasureText(scoreText, scoreFontSize);

    DrawText(
        scoreText,
        SCREEN_WIDTH  / 2 - scoreWidth / 2,
        SCREEN_HEIGHT / 2 - 30,
        scoreFontSize,
        YELLOW
    );

    // ---- Restart instruction -----------------------------------------------
    const char* restartText      = "PRESS [ENTER] TO PLAY AGAIN";
    constexpr int restartFontSize = 20;
    const int     restartWidth    = MeasureText(restartText, restartFontSize);

    DrawText(
        restartText,
        SCREEN_WIDTH  / 2 - restartWidth / 2,
        SCREEN_HEIGHT / 2 + 20,
        restartFontSize,
        GRAY
    );
}

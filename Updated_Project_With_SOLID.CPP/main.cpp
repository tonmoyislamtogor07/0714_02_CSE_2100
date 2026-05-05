/*
 * main.cpp
 *
 * Main entry point for Snake Game (C++ Edition)
 * Handles window creation and the main game loop
 *
 * Course: Advanced Programming Lab
 * Date: February 2026
 *
 * ============================================================================
 * SOLID PRINCIPLES APPLIED IN THIS FILE
 * ============================================================================
 *
 * [SRP] Single Responsibility Principle:
 *   - main() has exactly one job: bootstrap the window, run the loop, and
 *     tear down. All game logic is delegated to Game_* functions defined in
 *     game.cpp, keeping this file thin and focused.
 *
 * [DIP] Dependency Inversion Principle:
 *   - main() calls only the abstract Game interface (Game_Initialize,
 *     Game_UpdateAndDraw, Game_Cleanup) — it has zero knowledge of Snake,
 *     Food, Renderer, or any other concrete subsystem.
 * ============================================================================
 */

#include "snake_game.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

/*
 * [SRP] Program main entry point.
 *       Responsible solely for window lifecycle and the top-level game loop.
 *       All update / render logic is delegated to Game_* abstractions.
 *
 * [DIP] Depends on the Game interface (high-level abstraction), not on any
 *       concrete module like Snake or Renderer.
 */
int main(void)
{
    // -------------------------------------------------------------------
    // Window initialisation — responsibility of main, not of Game modules
    // -------------------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Classic Game: Snake - C++ SOLID Edition");

    // -------------------------------------------------------------------
    // [DIP] Delegate all game state setup to the Game abstraction layer
    // -------------------------------------------------------------------
    Game_Initialize();

#if defined(PLATFORM_WEB)
    // Web platform: emscripten drives the loop
    emscripten_set_main_loop(Game_UpdateAndDraw, 60, 1);
#else
    SetTargetFPS(TARGET_FPS);

    // [SRP] The loop has one responsibility: call the combined update-draw
    //       tick until the user requests to close the window.
    while (!WindowShouldClose())
    {
        // [DIP] Game_UpdateAndDraw is called through the Game interface;
        //       main() does not know how updating or rendering is implemented.
        Game_UpdateAndDraw();
    }
#endif

    // -------------------------------------------------------------------
    // Cleanup: delegate to Game, then close window
    // -------------------------------------------------------------------
    Game_Cleanup();
    CloseWindow();

    return 0;
}

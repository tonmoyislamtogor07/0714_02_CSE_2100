/*
 * game.cpp
 *
 * Core game logic and state management (C++ Edition)
 * Coordinates all subsystems: snake, food, collision, and rendering
 *
 * Course: Advanced Programming Lab
 * Date: February 2026
 *
 * ============================================================================
 * SOLID PRINCIPLES APPLIED IN THIS FILE
 * ============================================================================
 *
 * [SRP] Single Responsibility Principle:
 *   - Game_Initialize  -> resets all state to a clean start — nothing else.
 *   - Game_Update      -> advances exactly one logical frame — no drawing.
 *   - Game_Render      -> draws exactly one frame — no logic or state changes.
 *   - Game_Cleanup     -> releases resources — nothing else.
 *   - Game_UpdateAndDraw -> delegates to the two above — is itself thin.
 *   The game module coordinates subsystems; it does not implement any of
 *   their individual behaviours (those live in snake.cpp, food.cpp, etc.).
 *
 * [OCP] Open/Closed Principle:
 *   - New entities (obstacles, power-ups) can be added by calling new
 *     module functions from Game_Update / Game_Render without modifying the
 *     existing snake/food/collision logic.
 *   - Game difficulty modes can be introduced by adding new branches in
 *     Game_Initialize without touching the subsystem implementations.
 *
 * [LSP] Liskov Substitution Principle:
 *   - Game_Update calls only through the public function interfaces of each
 *     module (Snake_*, Food_*, Collision_*, Renderer_*).  Swapping any
 *     module for a test-double that honours the same function signatures
 *     will not break Game_Update's logic.
 *
 * [ISP] Interface Segregation Principle:
 *   - Game_Update uses only the update/logic API of each module.
 *   - Game_Render uses only the render API of each module.
 *   - Neither function carries dependencies on the other module's API
 *     surface — e.g. Game_Render never calls Snake_ProcessInput.
 *
 * [DIP] Dependency Inversion Principle:
 *   - Game_Update and Game_Render depend on module-level function
 *     abstractions (Snake_*, Food_*, Renderer_*, Collision_*) — never on
 *     the raw struct fields of Snake or Food directly.
 *   - GridOffset is computed via Utils_CalculateGridOffset() abstraction
 *     rather than inline arithmetic, keeping the game layer decoupled from
 *     screen-layout details.
 * ============================================================================
 */

#include "snake_game.h"
#include <cassert>

// ============================================================================
// GLOBAL GAME STATE
// [SRP] Module-level statics confine state ownership to this translation
//       unit — no other module can mutate these directly.
// ============================================================================

static GameState gameState = { 0 };   // [SRP] game counters/flags only
static Snake     playerSnake = { 0 }; // [SRP] snake entity only
static Food      gameFruit   = { 0 }; // [SRP] food entity only

// ============================================================================
// GAME INITIALIZATION
// [SRP] Game_Initialize resets all subsystems to a valid starting state.
//       It does not update positions, process input, or draw anything.
// [DIP] Delegates entity setup to Snake_Initialize and Food_Initialize —
//       high-level init depends on module abstractions, not raw struct writes.
// ============================================================================

/*
 * Initialize all game systems and reset game state.
 * Called at game start and when restarting after game over.
 */
void Game_Initialize(void)
{
    // [SRP] Reset only game-level counters and flags — entities reset below
    gameState.framesCounter = 0;
    gameState.playerScore   = 0;
    gameState.isGameOver    = false;
    gameState.isPaused      = false;
    gameState.freezeCounter = 0;

    // [DIP] Grid centering is delegated to the Utils abstraction layer
    gameState.gridOffset = Utils_CalculateGridOffset();

    // [DIP] Entity initialisation delegated to their respective modules
    Snake_Initialize(&playerSnake, gameState.gridOffset, gameState.gridOffset);
    Food_Initialize(&gameFruit);
}

// ============================================================================
// GAME UPDATE LOGIC
// [SRP] Game_Update advances the game by one logical tick.
//       It never draws anything — rendering is Game_Render's responsibility.
// [DIP] All per-subsystem logic is invoked through module-level function
//       abstractions (Snake_ProcessInput, Food_Spawn, etc.).
// [OCP] Adding a new subsystem (e.g. obstacle) means calling new module
//       functions here, not editing existing snake/food/collision branches.
// ============================================================================

/*
 * Main game update function.
 * Called once per frame to advance game state.
 */
void Game_Update(void)
{
    if (!gameState.isGameOver)
    {
        // ---- Pause toggle --------------------------------------------------
        // [SRP] Toggling pause is a single state mutation — isolated here
        if (IsKeyPressed('P'))
        {
            gameState.isPaused = !gameState.isPaused;
        }

        if (!gameState.isPaused)
        {
            // ---- Freeze countdown ------------------------------------------
            // [SRP] Freeze countdown is its own state transition — it does
            //       not mix with normal movement or collision logic.
            if (gameState.freezeCounter > 0)
            {
                gameState.freezeCounter--;
                if (gameState.freezeCounter == 0)
                {
                    gameState.isGameOver = true;
                }
                return;  // Skip all other logic during the freeze window
            }

            // ---- Input processing ------------------------------------------
            // [DIP] Delegates to Snake module abstraction — game.cpp does not
            //       know which keys are pressed or how they map to directions.
            Snake_ProcessInput(&playerSnake);

            // ---- Movement update -------------------------------------------
            // [DIP] Movement arithmetic lives in snake.cpp, not here
            Snake_UpdatePosition(&playerSnake, gameState.framesCounter);

            // ---- Wrap-around handling --------------------------------------
            // [SRP] Boundary wrap is a distinct concern from movement update
            Snake_HandleWrapAround(&playerSnake, gameState.gridOffset);

            // ---- Self-collision check --------------------------------------
            // [DIP] Delegates to Snake module — game.cpp only reacts to result
            if (Snake_CheckSelfCollision(&playerSnake))
            {
                gameState.freezeCounter = FREEZE_DURATION;
            }

            // ---- Food spawn ------------------------------------------------
            // [OCP] Adding "bonus food" means calling a new Food_SpawnBonus
            //       here — Food_Spawn itself is not modified.
            if (!gameFruit.active)
            {
                Food_Spawn(&gameFruit, &playerSnake, gameState.gridOffset);
            }

            // ---- Food collision reaction -----------------------------------
            // [DIP] Detection delegated to Collision module; growth/score
            //       mutations happen here (coordinator responsibility only).
            // [SRP] Score increment is in game.cpp — snake and food modules
            //       do not know about the score.
            if (Collision_CheckSnakeWithFood(&playerSnake, &gameFruit))
            {
                Snake_Grow(&playerSnake);
                gameFruit.active = false;
                gameState.playerScore++;
            }

            gameState.framesCounter++;
        }
    }
    else
    {
        // ---- Game over restart input --------------------------------------
        // [SRP] Only responsibility here: listen for restart and re-init
        if (IsKeyPressed(KEY_ENTER))
        {
            // [DIP] Full reset delegated to Game_Initialize abstraction
            Game_Initialize();
        }
    }
}

// ============================================================================
// GAME RENDERING
// [SRP] Game_Render draws the current frame.
//       It does not modify any game state — it only reads and displays.
// [DIP] Delegates every draw call to Renderer_* and entity Render functions;
//       game.cpp does not call raylib draw primitives directly.
// [ISP] Render functions are called only here — Game_Update never draws.
// ============================================================================

/*
 * Main rendering function.
 * Draws all game elements for the current frame.
 */
void Game_Render(void)
{
    BeginDrawing();
    ClearBackground(BLACK);

    if (!gameState.isGameOver)
    {
        // [DIP] Grid drawing delegated to Renderer module abstraction
        Renderer_DrawGrid(gameState.gridOffset);

        // [DIP] Entity rendering delegated to each entity's own render fn
        Snake_Render(&playerSnake);
        Food_Render(&gameFruit);

        // ---- Conditional overlays -----------------------------------------
        // [SRP] Each overlay is an independent draw call based on one flag
        if (gameState.isPaused)
        {
            Renderer_DrawPauseScreen();
        }

        if (gameState.freezeCounter > 0)
        {
            Renderer_DrawFreezeEffect();
        }
    }
    else
    {
        // [DIP] Game-over UI delegated to Renderer; score passed as parameter
        //       so Renderer does not need to know about GameState struct.
        Renderer_DrawGameOver(gameState.playerScore);
    }

    EndDrawing();
}

// ============================================================================
// GAME CLEANUP
// [SRP] Game_Cleanup is responsible only for releasing resources.
//       Currently no dynamic memory is used; the body is reserved for future
//       resource management (textures, audio, etc.).
// ============================================================================

/*
 * Cleanup game resources.
 * Called before program exit.
 */
void Game_Cleanup(void)
{
    // [SRP] Placeholder for future resource deallocation (textures, sounds).
    // No dynamic allocations in current implementation.
}

// ============================================================================
// COMBINED UPDATE AND DRAW
// [SRP] Game_UpdateAndDraw is a thin coordinator: it calls update then render.
//       All logic lives in the two delegated functions.
// [DIP] Depends only on the Game_Update and Game_Render abstractions — used
//       by both the desktop loop in main.cpp and emscripten on the web.
// ============================================================================

/*
 * Combined update and draw function.
 * Used for the main game loop and the web (emscripten) platform.
 */
void Game_UpdateAndDraw(void)
{
    Game_Update();
    Game_Render();
}

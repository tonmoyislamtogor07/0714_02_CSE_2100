/*
 * controller.cpp
 *
 * ============================================================================
 * MVC ROLE: CONTROLLER
 * ============================================================================
 *
 * The Controller is the coordinator. It:
 *   - Is the ONLY place that reads raw keyboard input (IsKeyPressed)
 *   - Translates key presses into InputCommand structs for the Model
 *   - Drives the game loop: each frame = PollInput → Model_Update → View_Render
 *   - Owns window setup and teardown (window is infrastructure, not game logic)
 *   - Never draws anything (that is View's job)
 *   - Never stores game data (that is Model's job)
 *
 * WHY THIS IS THE CONTROLLER:
 *   In your original code, input was handled inside snake.cpp (Snake_ProcessInput)
 *   which meant the Snake module knew about KEY_RIGHT, KEY_LEFT, etc. — a
 *   violation of separation of concerns. In MVC, ONLY the Controller knows
 *   which keys do what. It translates them into neutral commands (Direction::Right)
 *   and sends those commands to the Model.
 *
 *   The Controller also knows about BOTH Model and View, but it never lets
 *   them know about each other — it is the bridge between them.
 *
 * ============================================================================
 */

#include "snake_game.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

// ============================================================================
// INPUT TRANSLATION
// ============================================================================

/*
 * Controller_PollInput — translate raw key state into an InputCommand.
 *
 * MVC Note:
 *   This is the ONLY function in the entire project that calls IsKeyPressed.
 *   The Model and View never touch input — they only receive an InputCommand.
 *
 *   The Controller reads current snake speed/direction from the Model (via
 *   read-only accessor) to build the command, but it does NOT enforce the
 *   180-degree rule itself — that is game logic belonging to the Model.
 */
InputCommand Controller_PollInput(const Snake* snake, const GameState* state)
{
    InputCommand cmd;
    cmd.moveDirection = Direction::None;
    cmd.togglePause   = false;
    cmd.restart       = false;

    // ---- Pause toggle -------------------------------------------------------
    if (IsKeyPressed('P'))
        cmd.togglePause = true;

    // ---- Restart (only meaningful on game-over screen) ---------------------
    if (IsKeyPressed(KEY_ENTER))
        cmd.restart = true;

    // ---- Movement direction -------------------------------------------------
    // The Controller maps physical keys → neutral Direction enum values.
    // It does NOT enforce the "no 180-degree reverse" rule — that belongs in
    // the Model because it is a game rule, not an input concern.
    if (IsKeyPressed(KEY_RIGHT)) cmd.moveDirection = Direction::Right;
    if (IsKeyPressed(KEY_LEFT))  cmd.moveDirection = Direction::Left;
    if (IsKeyPressed(KEY_UP))    cmd.moveDirection = Direction::Up;
    if (IsKeyPressed(KEY_DOWN))  cmd.moveDirection = Direction::Down;

    return cmd;
}

// ============================================================================
// SINGLE-FRAME TICK
// ============================================================================

/*
 * Controller_Tick — execute one complete frame of the game.
 *
 * Order is always: Poll Input → Update Model → Render View.
 * The View always sees a fully-updated Model; the Model never sees the View.
 */
void Controller_Tick(void)
{
    // 1. Read current Model state (read-only)
    const Snake*     snake = Model_GetSnake();
    const Food*      food  = Model_GetFood();
    const GameState* state = Model_GetGameState();

    // 2. Translate user input into a neutral command
    InputCommand cmd = Controller_PollInput(snake, state);

    // 3. Feed the command to the Model — it updates all game data
    Model_Update(cmd);

    // 4. Tell the View to render the (now-updated) Model state
    //    View receives const pointers — it can only read, never write
    View_Render(
        Model_GetSnake(),
        Model_GetFood(),
        Model_GetGameState()
    );
}

// ============================================================================
// MAIN GAME LOOP
// ============================================================================

/*
 * Controller_Run — own the full application lifecycle.
 *
 * Responsibilities:
 *   1. Create and configure the window (infrastructure, not game logic)
 *   2. Ask the Model to initialise game data
 *   3. Run the game loop (desktop or web/emscripten)
 *   4. Destroy the window on exit
 *
 * It does NOT contain any game logic — it only orchestrates.
 */
void Controller_Run(void)
{
    // -- Window setup --------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Classic Game: Snake - MVC Edition");

    // -- Model initialisation ------------------------------------------------
    // The Controller tells the Model to get ready; it does not set up data
    // itself (that would mix Controller and Model responsibilities).
    Model_Initialize();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(Controller_Tick, 60, 1);
#else
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
        Controller_Tick();
#endif

    // -- Cleanup -------------------------------------------------------------
    CloseWindow();
}

/*
 * model.cpp
 *
 * ============================================================================
 * MVC ROLE: MODEL
 * ============================================================================
 *
 * The Model is the heart of the game. It:
 *   - Owns ALL game data (Snake, Food, GameState) as private module-statics
 *   - Contains ALL game logic: movement, collision, food spawning, scoring
 *   - Accepts commands via Model_Update(InputCommand) — it NEVER reads input
 *   - Exposes read-only accessors so View and Controller can observe state
 *     without being able to corrupt it
 *   - Has ZERO knowledge of raylib draw calls, key polling, or the View
 *
 * WHY THIS IS THE MODEL:
 *   Your original code mixed logic and state across game.cpp, snake.cpp, and
 *   food.cpp with no clear owner. Here everything that is "what the game IS"
 *   lives in one place: the Model. Anything that is "what the game LOOKS LIKE"
 *   belongs in the View; anything that is "what the user DID" belongs in the
 *   Controller.
 *
 * ============================================================================
 */

#include "snake_game.h"
#include <cassert>

// ============================================================================
// PRIVATE MODEL STATE
// Only model.cpp can write to these. All other modules read via accessors.
// ============================================================================

static GameState s_state  = { 0 };
static Snake     s_snake  = { 0 };
static Food      s_food   = { 0 };

// ============================================================================
// PRIVATE HELPERS
// ============================================================================

static void Snake_Initialize(Snake* snake, Vector2 gridOffset)
{
    assert(snake != nullptr);
    snake->length    = 1;
    snake->allowMove = false;

    for (int i = 0; i < MAX_SNAKE_LENGTH; i++)
    {
        snake->segments[i].position = { gridOffset.x, gridOffset.y };
        snake->segments[i].size     = { (float)SQUARE_SIZE, (float)SQUARE_SIZE };
        snake->segments[i].speed    = { (float)SQUARE_SIZE, 0.0f };
        snake->segments[i].color    = (i == 0) ? BLUE : SKYBLUE;
        snake->segmentPositions[i]  = { 0.0f, 0.0f };
    }
}

static void Food_Initialize(Food* food)
{
    assert(food != nullptr);
    food->size     = { (float)SQUARE_SIZE, (float)SQUARE_SIZE };
    food->color    = YELLOW;
    food->active   = false;
    food->position = { 0.0f, 0.0f };
}

static void Food_Spawn(Food* food, const Snake* snake, Vector2 gridOffset)
{
    food->active = true;

    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    if (snake->length >= cols * rows)
    {
        food->active = false;
        return;
    }

    int  rx, ry;
    bool valid;
    do {
        valid = true;
        rx = GetRandomValue(0, cols - 1);
        ry = GetRandomValue(0, rows - 1);
        food->position = {
            gridOffset.x + (float)(rx * SQUARE_SIZE),
            gridOffset.y + (float)(ry * SQUARE_SIZE)
        };
        for (int i = 0; i < snake->length; i++)
        {
            if (food->position.x == snake->segments[i].position.x &&
                food->position.y == snake->segments[i].position.y)
            {
                valid = false;
                break;
            }
        }
    } while (!valid);
}

static void Snake_Grow(Snake* snake)
{
    assert(snake->length < MAX_SNAKE_LENGTH);
    snake->segments[snake->length].position =
        snake->segmentPositions[snake->length - 1];
    snake->length++;
}

static bool CheckSnakeEatsFood(const Snake* snake, const Food* food)
{
    if (!food->active) return false;
    return snake->segments[0].position.x == food->position.x &&
           snake->segments[0].position.y == food->position.y;
}

static bool CheckSnakeSelfCollision(const Snake* snake)
{
    for (int i = 1; i < snake->length; i++)
    {
        if (snake->segments[0].position.x == snake->segments[i].position.x &&
            snake->segments[0].position.y == snake->segments[i].position.y)
            return true;
    }
    return false;
}

static void ApplyDirection(Snake* snake, Direction dir)
{
    /*
     * The Model applies a direction command from the Controller.
     * It enforces the "no 180-degree turn" rule itself — this is game LOGIC,
     * not input logic, so it belongs in the Model.
     */
    if (!snake->allowMove) return;

    SnakeSegment* head = &snake->segments[0];

    switch (dir)
    {
        case Direction::Right:
            if (head->speed.x == 0.0f) {
                head->speed = { (float)SQUARE_SIZE, 0.0f };
                snake->allowMove = false;
            }
            break;
        case Direction::Left:
            if (head->speed.x == 0.0f) {
                head->speed = { -(float)SQUARE_SIZE, 0.0f };
                snake->allowMove = false;
            }
            break;
        case Direction::Up:
            if (head->speed.y == 0.0f) {
                head->speed = { 0.0f, -(float)SQUARE_SIZE };
                snake->allowMove = false;
            }
            break;
        case Direction::Down:
            if (head->speed.y == 0.0f) {
                head->speed = { 0.0f, (float)SQUARE_SIZE };
                snake->allowMove = false;
            }
            break;
        default:
            break;
    }
}

static void UpdateSnakePosition(Snake* snake, int framesCounter)
{
    for (int i = 0; i < snake->length; i++)
        snake->segmentPositions[i] = snake->segments[i].position;

    if ((framesCounter % MOVE_FRAME_DELAY) == 0)
    {
        for (int i = 0; i < snake->length; i++)
        {
            if (i == 0)
            {
                snake->segments[0].position.x += snake->segments[0].speed.x;
                snake->segments[0].position.y += snake->segments[0].speed.y;
                snake->allowMove = true;
            }
            else
            {
                snake->segments[i].position = snake->segmentPositions[i - 1];
            }
        }
    }
}

static void HandleWrapAround(Snake* snake, Vector2 gridOffset)
{
    const int cols = Utils_GetGridColumns();
    const int rows = Utils_GetGridRows();

    const float minX = gridOffset.x;
    const float minY = gridOffset.y;
    const float maxX = gridOffset.x + (float)((cols - 1) * SQUARE_SIZE);
    const float maxY = gridOffset.y + (float)((rows - 1) * SQUARE_SIZE);

    if      (snake->segments[0].position.x > maxX) snake->segments[0].position.x = minX;
    else if (snake->segments[0].position.x < minX) snake->segments[0].position.x = maxX;

    if      (snake->segments[0].position.y > maxY) snake->segments[0].position.y = minY;
    else if (snake->segments[0].position.y < minY) snake->segments[0].position.y = maxY;
}

// ============================================================================
// PUBLIC MODEL API
// ============================================================================

/*
 * Model_Initialize — reset all game data to a clean starting state.
 * Called at game start and on restart.
 */
void Model_Initialize(void)
{
    s_state.framesCounter = 0;
    s_state.playerScore   = 0;
    s_state.isGameOver    = false;
    s_state.isPaused      = false;
    s_state.freezeCounter = 0;
    s_state.gridOffset    = Utils_CalculateGridOffset();

    Snake_Initialize(&s_snake, s_state.gridOffset);
    Food_Initialize(&s_food);
}

/*
 * Model_Update — advance the game by one logical tick.
 *
 * Receives an InputCommand from the Controller.
 * ALL game logic runs here. The Model never reads keys directly.
 *
 * MVC Note: This is the only public write entry-point on the Model.
 *           The Controller calls this once per frame; the View never calls it.
 */
void Model_Update(const InputCommand& cmd)
{
    if (!s_state.isGameOver)
    {
        // ---- Pause toggle (driven by Controller command) ------------------
        if (cmd.togglePause)
            s_state.isPaused = !s_state.isPaused;

        if (!s_state.isPaused)
        {
            // ---- Freeze countdown -----------------------------------------
            if (s_state.freezeCounter > 0)
            {
                s_state.freezeCounter--;
                if (s_state.freezeCounter == 0)
                    s_state.isGameOver = true;
                return;
            }

            // ---- Apply direction from Controller --------------------------
            // The Model enforces the 180-degree rule; the Controller just
            // says "the user pressed RIGHT" — it does not know whether that
            // is legal yet.
            ApplyDirection(&s_snake, cmd.moveDirection);

            // ---- Movement -------------------------------------------------
            UpdateSnakePosition(&s_snake, s_state.framesCounter);
            HandleWrapAround(&s_snake, s_state.gridOffset);

            // ---- Self-collision -------------------------------------------
            if (CheckSnakeSelfCollision(&s_snake))
                s_state.freezeCounter = FREEZE_DURATION;

            // ---- Food spawn -----------------------------------------------
            if (!s_food.active)
                Food_Spawn(&s_food, &s_snake, s_state.gridOffset);

            // ---- Eat food -------------------------------------------------
            if (CheckSnakeEatsFood(&s_snake, &s_food))
            {
                Snake_Grow(&s_snake);
                s_food.active = false;
                s_state.playerScore++;
            }

            s_state.framesCounter++;
        }
    }
    else
    {
        // ---- Restart (driven by Controller command) -----------------------
        if (cmd.restart)
            Model_Initialize();
    }
}

// ============================================================================
// READ-ONLY ACCESSORS
// The View and Controller call these to observe state without mutating it.
// ============================================================================

const Snake*     Model_GetSnake(void)     { return &s_snake; }
const Food*      Model_GetFood(void)      { return &s_food;  }
const GameState* Model_GetGameState(void) { return &s_state; }

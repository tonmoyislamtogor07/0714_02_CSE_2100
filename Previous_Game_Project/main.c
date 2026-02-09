#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

#define SNAKE_LENGTH   400
#define SQUARE_SIZE     31

typedef struct Snake {
    Vector2 position;
    Vector2 size;
    Vector2 speed;
    Color color;
} Snake;

typedef struct Food {
    Vector2 position;
    Vector2 size;
    bool active;
    Color color;
} Food;

// Global Variables Declaration
static const int screenWidth = 800;
static const int screenHeight = 450;

static int framesCounter = 0;
static int score = 0;
static bool gameOver = false;
static bool pause = false;

static Food fruit = { 0 };
static Snake snake[SNAKE_LENGTH] = { 0 };
static Vector2 snakePosition[SNAKE_LENGTH] = { 0 };
static bool allowMove = false;
static Vector2 offset = { 0 };
static int counterTail = 0;

static int freezeCounter = 0;   // NEW: delay before game over

// Module Functions Declaration (local)
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

// Program main entry point
int main(void)
{
    InitWindow(screenWidth, screenHeight, "classic game: snake");

    InitGame();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(30);
    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }
#endif

    UnloadGame();
    CloseWindow();

    return 0;
}

// Initialize game variables
void InitGame(void)
{
    framesCounter = 0;
    score = 0;
    gameOver = false;
    pause = false;
    freezeCounter = 0;

    counterTail = 1;
    allowMove = false;

    int cols = screenWidth / SQUARE_SIZE;
    int rows = screenHeight / SQUARE_SIZE;

    offset.x = (float)((screenWidth - cols * SQUARE_SIZE) / 2);
    offset.y = (float)((screenHeight - rows * SQUARE_SIZE) / 2);

    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snake[i].position = (Vector2){ offset.x, offset.y };
        snake[i].size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
        snake[i].speed = (Vector2){ SQUARE_SIZE, 0 };
        snake[i].color = (i == 0) ? BLUE : SKYBLUE;
        snakePosition[i] = (Vector2){ 0.0f, 0.0f };
    }

    fruit.size = (Vector2){ SQUARE_SIZE, SQUARE_SIZE };
    fruit.color = YELLOW;
    fruit.active = false;
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            if (freezeCounter > 0)
            {
                freezeCounter--;
                if (freezeCounter == 0)
                {
                    gameOver = true;
                }
                return;  
            }

            // Player control
            if (IsKeyPressed(KEY_RIGHT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ SQUARE_SIZE, 0 };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && (snake[0].speed.x == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ -SQUARE_SIZE, 0 };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_UP) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ 0, -SQUARE_SIZE };
                allowMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && (snake[0].speed.y == 0) && allowMove)
            {
                snake[0].speed = (Vector2){ 0, SQUARE_SIZE };
                allowMove = false;
            }

            // Snake movement
            for (int i = 0; i < counterTail; i++) snakePosition[i] = snake[i].position;

            if ((framesCounter % 5) == 0)
            {
                for (int i = 0; i < counterTail; i++)
                {
                    if (i == 0)
                    {
                        snake[0].position.x += snake[0].speed.x;
                        snake[0].position.y += snake[0].speed.y;
                        allowMove = true;
                    }
                    else snake[i].position = snakePosition[i - 1];
                }
            }

            // Grid bounds
            int cols = screenWidth / SQUARE_SIZE;
            int rows = screenHeight / SQUARE_SIZE;
            float minX = offset.x;
            float minY = offset.y;
            float maxX = offset.x + (cols - 1) * SQUARE_SIZE;
            float maxY = offset.y + (rows - 1) * SQUARE_SIZE;

            // Wrap-around logic
            if (snake[0].position.x > maxX) snake[0].position.x = minX;
            else if (snake[0].position.x < minX) snake[0].position.x = maxX;
            if (snake[0].position.y > maxY) snake[0].position.y = minY;
            else if (snake[0].position.y < minY) snake[0].position.y = maxY;

            // Self Collision Detection 
            for (int i = 1; i < counterTail; i++)
            {
                if ((snake[0].position.x == snake[i].position.x) &&
                    (snake[0].position.y == snake[i].position.y))
                {
                    freezeCounter = 60; 
                }
            }

            // Fruit position calculation
            if (!fruit.active)
            {
                fruit.active = true;
                int fx, fy;
                int cols = screenWidth / SQUARE_SIZE;
                int rows = screenHeight / SQUARE_SIZE;
                fx = GetRandomValue(0, cols - 1);
                fy = GetRandomValue(0, rows - 1);
                fruit.position = (Vector2){ offset.x + fx * SQUARE_SIZE, offset.y + fy * SQUARE_SIZE };

                if (counterTail >= cols * rows)
                {
                    fruit.active = false;
                }
                else
                {
                    bool collide;
                    do
                    {
                        collide = false;

                        for (int i = 0; i < counterTail; i++)
                        {
                            if ((fruit.position.x == snake[i].position.x) && (fruit.position.y == snake[i].position.y))
                            {
                                fx = GetRandomValue(0, cols - 1);
                                fy = GetRandomValue(0, rows - 1);
                                fruit.position = (Vector2){ offset.x + fx * SQUARE_SIZE, offset.y + fy * SQUARE_SIZE };
                                collide = true;
                                break;
                            }
                        }
                    } while (collide);
                }
            }

            // Collision with fruit
            if ((int)(snake[0].position.x == fruit.position.x) &&
                (int)(snake[0].position.y == fruit.position.y))
            {
                snake[counterTail].position = snakePosition[counterTail - 1];
                counterTail++;
                fruit.active = false;
                score++;
            }


            framesCounter++;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            InitGame();
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    BeginDrawing();

    ClearBackground(BLACK);

    if (!gameOver)
    {
        int cols = screenWidth / SQUARE_SIZE;
        int rows = screenHeight / SQUARE_SIZE;

        for (int i = 0; i <= cols; i++)
        {
            DrawLineV((Vector2){ offset.x + i * SQUARE_SIZE, offset.y },
                (Vector2){ offset.x + i * SQUARE_SIZE, offset.y + rows * SQUARE_SIZE }, LIGHTGRAY);
        }

        for (int i = 0; i <= rows; i++)
        {
            DrawLineV((Vector2){ offset.x, offset.y + i * SQUARE_SIZE },
                (Vector2){ offset.x + cols * SQUARE_SIZE, offset.y + i * SQUARE_SIZE }, LIGHTGRAY);
        }

        for (int i = 0; i < counterTail; i++) DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);
        DrawRectangleV(fruit.position, fruit.size, fruit.color);

        if (pause) DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);

        // Red tint when snake crashed
        if (freezeCounter > 0)
        {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(RED, 0.3f));
        }
    }
    else
    {
        DrawText("GAME OVER!", GetScreenWidth() / 2 - MeasureText("GAME OVER!", 40) / 2, GetScreenHeight() / 2 - 80, 40, RED);
        DrawText(TextFormat("FINAL SCORE: %d", score),
            GetScreenWidth() / 2 - MeasureText(TextFormat("FINAL SCORE: %d", score), 30) / 2,
            GetScreenHeight() / 2 - 30, 30, YELLOW);
        DrawText("PRESS [ENTER] TO PLAY AGAIN",
            GetScreenWidth() / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
            GetScreenHeight() / 2 + 20, 20, GRAY);
    }

    EndDrawing();
}

// Unload game variables
void UnloadGame(void)
{
    // Nothing to unload in this version
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

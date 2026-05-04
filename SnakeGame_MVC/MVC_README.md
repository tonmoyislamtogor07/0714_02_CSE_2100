# Snake Game — MVC Edition

## Architecture Overview

This project refactors the original SOLID-based Snake game into a strict
**Model-View-Controller (MVC)** architecture.

```
┌─────────────────────────────────────────────────────────────┐
│                        main.cpp                             │
│              (bootstrap only — calls Controller_Run)        │
└─────────────────────────┬───────────────────────────────────┘
                          │
          ┌───────────────▼───────────────┐
          │        controller.cpp         │
          │  - Owns the game loop         │
          │  - Polls keyboard input       │
          │  - Builds InputCommand        │
          │  - Calls Model then View      │
          └──────┬────────────────┬───────┘
                 │                │
    ┌────────────▼───┐    ┌───────▼────────────┐
    │   model.cpp    │    │     view.cpp        │
    │                │    │                    │
    │ - All game data│    │ - All draw calls   │
    │ - All logic    │◄───│ - Reads Model data │
    │ - Accessors    │    │   (read-only)      │
    └────────────────┘    └────────────────────┘
```

---

## File Responsibilities

| File | MVC Role | Responsibility |
|------|----------|----------------|
| `model.cpp` | **Model** | Owns Snake, Food, GameState. Contains ALL game logic (movement, collision, spawning, scoring). Never reads input or draws anything. |
| `view.cpp` | **View** | All `DrawRectangle`, `DrawText`, `DrawLine` calls. Receives const pointers from Controller. Never modifies data. |
| `controller.cpp` | **Controller** | The ONLY file that calls `IsKeyPressed`. Translates keys → `InputCommand`. Drives the loop: Poll → Update → Render. |
| `main.cpp` | Entry point | Calls `Controller_Run()` and nothing else. |
| `utils.cpp` | Shared | Pure grid-calculation helpers used by all layers. |
| `snake_game.h` | Header | Shared types, constants, and function declarations. |

---

## Key MVC Rules Enforced

### 1. Model ← Controller only
`Model_Update()` is the **only write entry-point** on the Model.
The View never calls it. The Model never calls `IsKeyPressed`.

### 2. Input → InputCommand (decoupling)
The Controller translates raw keys into a neutral `InputCommand` struct:
```cpp
struct InputCommand {
    Direction moveDirection;  // Up/Down/Left/Right/None
    bool      togglePause;
    bool      restart;
};
```
The Model receives a command — it has no idea what key was pressed.

### 3. View is read-only
`View_Render(const Snake*, const Food*, const GameState*)` — all `const`.
The View physically cannot modify game state.

### 4. One frame = three steps, always in order
```cpp
// controller.cpp — Controller_Tick()
InputCommand cmd = Controller_PollInput(snake, state);  // 1. Input
Model_Update(cmd);                                       // 2. Logic
View_Render(Model_GetSnake(), ...);                      // 3. Draw
```

---

## Comparison: Original vs MVC

| Concern | Original | MVC |
|---------|----------|-----|
| Input handling | `snake.cpp` (`Snake_ProcessInput`) | `controller.cpp` only |
| Game logic | `game.cpp` | `model.cpp` only |
| Rendering | spread across `snake.cpp`, `food.cpp`, `renderer.cpp`, `game.cpp` | `view.cpp` only |
| Data ownership | globals in `game.cpp` | private statics in `model.cpp` |

---

## Build

```bash
make
./snake_mvc
```

# 🐍 Snake Game — MVC Refactor (C++ / Raylib)

> A classic Snake game built in C++ with Raylib, refactored from a SOLID-principles codebase into a clean **Model-View-Controller (MVC)** architecture.

---

## 📌 What This Project Is

This repository contains two versions of the same Snake game:

| Version | Folder | Architecture |
|---|---|---|
| Original | `Updated_Project_With_SOLID.CPP/` | SOLID Principles |
| Refactored | `SnakeGame_MVC/` | MVC (Model-View-Controller) |

The game itself is unchanged — same snake movement, food spawning, wrap-around, self-collision, freeze effect, and scoring. What changed is **how the code is organized**.

---

## 🎮 Game Features

- Classic grid-based snake movement
- Wrap-around edges (snake exits one side, enters the other)
- Self-collision with a freeze effect before game over
- Random food spawning (guaranteed not to overlap the snake)
- Live score display + final score on game over
- Pause / unpause with `P`
- Restart with `Enter` after game over

**Controls:**  
`↑ ↓ ← →` — Move &nbsp;|&nbsp; `P` — Pause &nbsp;|&nbsp; `Enter` — Restart

---

## 🏗️ What is MVC and Why Use It?

**MVC (Model-View-Controller)** is an architectural pattern that separates a program into three distinct layers:

```
┌──────────────────────────────────────────────────────────┐
│                        main.cpp                          │
│             (bootstrap only — calls Controller)          │
└──────────────────────┬───────────────────────────────────┘
                       │
       ┌───────────────▼───────────────┐
       │         controller.cpp        │
       │  • Owns the game loop         │
       │  • Polls keyboard input ONLY  │
       │  • Builds InputCommand        │
       │  • Calls Model, then View     │
       └──────┬───────────────┬────────┘
              │               │
   ┌──────────▼────┐   ┌──────▼──────────────┐
   │   model.cpp   │   │      view.cpp        │
   │               │   │                     │
   │  All game     │   │  All draw calls     │
   │  data +       │◄──│  (read-only access  │
   │  all logic    │   │   to model data)    │
   └───────────────┘   └─────────────────────┘
```

| Layer | File | Responsibility |
|---|---|---|
| **Model** | `model.cpp` | Owns Snake, Food, GameState. Contains ALL game logic — movement, collision detection, scoring, food spawning. Never reads input, never draws. |
| **View** | `view.cpp` | Every single `DrawRectangle`, `DrawText`, `DrawLine` call. Receives const (read-only) pointers. Never modifies data. |
| **Controller** | `controller.cpp` | The ONLY place `IsKeyPressed` is called. Translates keys into neutral `InputCommand` structs. Drives the loop: Poll → Update → Render. |

---

## 🔄 Before vs After: What Changed

### File Structure

| Original (SOLID) | MVC Refactor | Notes |
|---|---|---|
| `game.cpp` | `model.cpp` + `controller.cpp` | Logic and coordination split into proper layers |
| `snake.cpp` | `model.cpp` | Snake data + movement logic moved into Model |
| `food.cpp` | `model.cpp` | Food data + spawn logic moved into Model |
| `collision.cpp` | `model.cpp` | Collision detection is game logic → belongs in Model |
| `renderer.cpp` + `Snake_Render()` + `Food_Render()` | `view.cpp` | ALL rendering unified into one View file |
| `main.cpp` | `main.cpp` | Now calls only `Controller_Run()` |
| `utils.cpp` | `utils.cpp` | Unchanged — pure helpers shared across all layers |
| `snake_game.h` | `snake_game.h` | Extended with `InputCommand` and MVC function signatures |

### The Biggest Structural Changes

**1. Input was inside the wrong layer**

In the original, `Snake_ProcessInput()` lived in `snake.cpp` and called `IsKeyPressed` directly. This meant the Snake entity knew about keyboard keys — mixing input concern with game entity logic.

```cpp
// ❌ ORIGINAL — input buried inside snake.cpp
void Snake_ProcessInput(Snake* snake) {
    if (IsKeyPressed(KEY_RIGHT) && ...) { ... }
}
```

```cpp
// ✅ MVC — controller.cpp is the ONLY file that reads input
InputCommand Controller_PollInput(...) {
    if (IsKeyPressed(KEY_RIGHT)) cmd.moveDirection = Direction::Right;
    return cmd;  // neutral command sent to Model
}
```

**2. The Model never sees keys — it sees commands**

A new `InputCommand` struct decouples the Controller from the Model:

```cpp
struct InputCommand {
    Direction moveDirection;  // Up / Down / Left / Right / None
    bool      togglePause;
    bool      restart;
};
```

The Model receives intent, not keycode. This means the game logic could be reused with a gamepad, touch input, or AI player — just produce a different `InputCommand`.

**3. Rendering was fragmented**

Original rendering was spread across four files (`game.cpp`, `snake.cpp`, `food.cpp`, `renderer.cpp`). In MVC, the View is a single cohesive layer — if you want to change how anything looks, you open `view.cpp` and only `view.cpp`.

**4. Data ownership is clear**

Original game data lived as globals in `game.cpp`. In MVC, all data lives as private statics in `model.cpp`. No other file can write to them — only read via `const` accessors:

```cpp
const Snake*     Model_GetSnake(void);
const Food*      Model_GetFood(void);
const GameState* Model_GetGameState(void);
```

---

## 📁 MVC Project Structure

```
SnakeGame_MVC/
├── snake_game.h      # Shared types, constants, MVC function declarations
├── model.cpp         # MODEL   — all game data and logic
├── view.cpp          # VIEW    — all rendering
├── controller.cpp    # CONTROLLER — input polling, game loop
├── utils.cpp         # Shared pure helper functions
├── main.cpp          # Entry point (calls Controller_Run only)
└── Makefile
```

---

## 🧱 SOLID vs MVC — Are They Different Things?

Yes — they complement each other, not compete:

| | SOLID | MVC |
|---|---|---|
| **What it is** | Design principles for writing clean individual classes/functions | Architectural pattern for organizing entire layers of an application |
| **Scale** | Function / class level | Application / module level |
| **Goal** | Make each piece of code maintainable and extensible | Make the overall structure of the app clear and separated |

The MVC version still respects SOLID principles internally. MVC is the **macro-level structure**; SOLID describes how each function within that structure is written.

---

## 🛠️ Build & Run

**Requirements:** [Raylib](https://www.raylib.com/) installed on your system.

```bash
cd SnakeGame_MVC
make
./snake_mvc
```

To clean:
```bash
make clean
```

---

## 📚 Concepts Demonstrated

- **MVC architecture** in a C++ game context
- **Separation of concerns** — input, logic, and rendering in isolated layers
- **Command pattern** — `InputCommand` struct decouples input from game logic
- **Read-only interfaces** — `const` pointers prevent the View from mutating state
- **SOLID principles** preserved within each layer (SRP, OCP, DIP, ISP, LSP)

---

*Built with C++17 and [Raylib](https://www.raylib.com/)*

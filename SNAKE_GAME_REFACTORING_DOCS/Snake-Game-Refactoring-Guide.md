# 🐍 Snake Game — Code Refactoring & Software Engineering Standards Guide

**Course:** Advanced Programming Lab  
**Project:** Snake Game (C Language)  
**Purpose:** Improve code maintainability, readability, modularity, and scalability  
**Date:** February 2026

---

## 📑 Table of Contents

1. [Executive Summary](#executive-summary)
2. [Naming Conventions](#naming-conventions)
3. [Coding Style Guidelines](#coding-style-guidelines)
4. [Folder Structure](#folder-structure)
5. [Modular Design Principles](#modular-design-principles)
6. [Design Patterns for Snake Game](#design-patterns-for-snake-game)
7. [Error Handling & Memory Management](#error-handling--memory-management)
8. [Testing Strategy](#testing-strategy)
9. [Refactoring Roadmap](#refactoring-roadmap)
10. [Future Improvements](#future-improvements)

---

## Executive Summary

### Current Project Overview

The Snake Game project is a C-based interactive game featuring:

**Strengths:**
- Functional snake movement system
- Collision detection implemented
- Score tracking system
- Simple UI rendering (console/graphics depending on version)
- Real-time keyboard input handling

**Areas for Improvement:**
- Inconsistent naming conventions
- Excessive use of global variables
- Tight coupling between input, logic, and rendering
- Limited modular separation
- Difficult feature extensibility

### Refactoring Philosophy

> "Refactor incrementally without breaking gameplay functionality."

**Goals:**
- Cleaner code structure
- Easier debugging
- Better scalability
- Improved readability
- Professional coding standards

---

## Naming Conventions

### General Principles

- Use descriptive names
- Avoid cryptic abbreviations
- Maintain consistent casing
- Prefer domain-specific terminology

### Variable Naming Refactoring

#### Before Refactoring
```c
int x, y;
int snkLen;
int scr;
int dir;
```

**Issues:**
- Abbreviations reduce clarity
- Hard to understand intent
- Inconsistent naming

#### After Refactoring
```c
int snakeHeadX;
int snakeHeadY;
int snakeLength;
int playerScore;
int snakeDirection;
```

**Benefits:**
- Self-documenting variables
- Easier debugging
- Improved readability

### Function Naming Refactoring

#### Before
```c
void mv();
void chk();
void drw();
```

#### After
```c
void Snake_Move();
bool Collision_Check();
void Renderer_DrawFrame();
```

**Why This Matters:**
- Verb-based naming clarifies purpose
- Supports modular development
- Enhances maintainability

### Constants Naming Refactoring

#### Before
```c
#define W 40
#define H 20
#define SPD 150
```

#### After
```c
#define BOARD_WIDTH 40
#define BOARD_HEIGHT 20
#define GAME_SPEED_DEFAULT 150
```

**Improvement:**
- Eliminates magic numbers
- Makes configuration easier

### Structure Naming Refactoring

#### Before
```c
struct snk {
    int x[100];
    int y[100];
};
```

#### After
```c
typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position body[MAX_SNAKE_LENGTH];
    int length;
    int direction;
} Snake;
```

**Advantages:**
- Encapsulation
- Clear abstraction
- Better data organization

---

## Coding Style Guidelines

### Indentation

- **4 spaces** preferred
- Avoid mixing tabs and spaces

**Example:**
```c
if (collisionDetected) {
    Game_End();
}
```

### Line Length

- Recommended maximum ≈ **100 characters**
- Break long expressions logically

### Comment Quality Improvement

#### Before
```c
int score = 0; // initialize score
```

#### After
```c
/*
 * Tracks player's score.
 * Updated whenever the snake consumes food.
 */
int playerScore = 0;
```

**Principle:**  
Comments should explain **why**, not just **what**.

---

## Folder Structure

### Before Refactoring
```
snake-game/
├── main.c
├── snake.c
├── food.c
└── utils.c
```

**Problems:**
- Flat organization
- Hard to scale
- Difficult navigation

### Recommended Structure (After Refactoring)
```
snake-game/
├── src/
│   ├── main/
│   │   └── main.c
│   ├── core/
│   │   ├── snake.c
│   │   ├── food.c
│   │   └── collision.c
│   ├── input/
│   ├── rendering/
│   └── utils/
├── docs/
├── assets/
├── build/
└── README.md
```

**Benefits:**
- Separation of concerns
- Easier maintenance
- Professional project layout

---

## Modular Design Principles

### Before Modularization
```c
void UpdateGame() {
    moveSnake();
    drawSnake();
    checkCollision();
    generateFood();
}
```

**Problems:**
- Mixed responsibilities
- Difficult debugging
- Hard feature expansion

### After Modularization
```c
void Input_Process();
void Snake_UpdatePosition();
void Collision_Check();
void Food_Update();
void Renderer_DrawFrame();
```

### Standard Game Loop
```c
while (gameRunning) {
    Input_Process();
    Game_Update();
    Renderer_DrawFrame();
}
```

**Advantages:**
- Clear responsibility separation
- Better testability
- Easier scaling

---

## Design Patterns for Snake Game

### Game Loop Pattern
```c
while (gameRunning) {
    Input_Process();
    Game_Update();
    Renderer_Draw();
}
```

Ensures smooth gameplay flow.

### Observer Pattern (Optional)

**Useful for:**
- Score updates
- UI refresh
- Sound effects

**Example:**
- Collision Event → Game Over Screen
- Score Change → UI Update

### Command Pattern (Advanced)
```c
typedef struct {
    Direction moveDirection;
} MoveCommand;
```

**Applications:**
- Replay system
- AI simulation
- Undo feature

---

## Error Handling & Memory Management

### Safe Allocation
```c
Snake* snake = malloc(sizeof(Snake));
if (!snake) {
    fprintf(stderr, "Memory allocation failed\n");
}
```

### Ownership Documentation
```c
/*
 * Caller must free returned pointer.
 */
Snake* Snake_Create(void);
```

### Assertions
```c
assert(snake != NULL);
```

Used for development-time validation.

---

## Testing Strategy

### Functional Tests
- Snake movement accuracy
- Food spawning correctness
- Collision detection reliability

### Boundary Tests
- Wall collisions
- Maximum snake length
- Rapid direction changes

### Regression Testing
Ensure refactoring doesn't break gameplay.

---

## Refactoring Roadmap

### Phase 1 — Cleanup
- Fix naming conventions
- Reduce global variables
- Improve documentation

### Phase 2 — Modularization
- Separate input, logic, rendering
- Introduce header files

### Phase 3 — Optimization
- Optimize collision checks
- Improve rendering efficiency

### Phase 4 — Feature Expansion
- Levels and difficulty modes
- Sound effects
- High-score persistence

---

## Future Improvements

### Gameplay Features
- Multiple difficulty levels
- Obstacles
- AI-controlled snake
- Power-ups

### Technical Enhancements
- Graphics library integration
- Cross-platform compatibility
- Save/load game state

### Software Engineering Improvements
- Unit testing framework
- Continuous integration pipeline
- Automatic code formatting

---

## Final Note

This refactoring guide aims to:

✅ Improve code quality  
✅ Enhance maintainability  
✅ Support future expansion  
✅ Promote professional coding practices

A well-structured Snake Game project demonstrates not only functional gameplay but also strong software engineering discipline, maintainable architecture, and scalability for future enhancements.

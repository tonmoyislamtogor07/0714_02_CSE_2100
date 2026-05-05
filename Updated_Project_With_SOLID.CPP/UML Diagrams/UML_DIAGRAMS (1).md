# 📐 UML Diagrams - C++ SOLID Snake Game

## 🎯 Overview

This document contains UML diagrams showing the architecture of the SOLID Snake Game in C++.

---

## 📊 Class Diagram - Complete System

```
┌─────────────────────────────────────────────────────────────────────┐
│                        INTERFACE SEGREGATION                         │
│                    (Separate, Focused Interfaces)                    │
└─────────────────────────────────────────────────────────────────────┘

┌──────────────────┐    ┌──────────────────┐    ┌──────────────────┐
│  <<interface>>   │    │  <<interface>>   │    │  <<interface>>   │
│   IUpdatable     │    │   IRenderable    │    │   ICollidable    │
├──────────────────┤    ├──────────────────┤    ├──────────────────┤
│ + update(int)    │    │ + render(): void │    │ + getPosition()  │
│   : void         │    │   : const        │    │ + getSize()      │
└──────────────────┘    └──────────────────┘    │ + isActive()     │
                                                 └──────────────────┘

                              ▲
                              │ implements
                              │
                    ┌─────────┴──────────┐
                    │                    │
              ┌─────────────────────────────────────────┐
              │     <<abstract>>  GameEntity            │
              │  (Base class for polymorphism)          │
              ├─────────────────────────────────────────┤
              │ + update(int framesCounter): void = 0   │ [OCP] Pure virtual
              │ + render(): void = 0                    │ [OCP] Pure virtual
              │ + getPosition(): Vector2 = 0            │ [ISP] From interface
              │ + getSize(): Vector2 = 0                │ [ISP] From interface
              │ + isActive(): bool = 0                  │ [ISP] From interface
              │ + getType(): string = 0                 │ [SRP] Type info
              └─────────────────────────────────────────┘
                              ▲
                              │ inherits
                ┌─────────────┴─────────────┐
                │                           │
      ┌─────────────────┐         ┌─────────────────┐
      │     Snake       │         │      Food       │
      ├─────────────────┤         ├─────────────────┤
      │ - segments      │         │ - position      │ [SRP] Data
      │ - length        │         │ - size          │ [SRP] Data
      │ - allowMove     │         │ - active        │ [SRP] Data
      │ - gridOffset    │         │ - color         │ [SRP] Data
      ├─────────────────┤         ├─────────────────┤
      │ + update()      │         │ + update()      │ [LSP] Override
      │ + render()      │         │ + render()      │ [LSP] Override
      │ + processInput()│         │ + spawn()       │ [SRP] Specific
      │ + grow()        │         │ + deactivate()  │ [SRP] Specific
      │ + checkSelf     │         │                 │
      │   Collision()   │         │                 │
      └─────────────────┘         └─────────────────┘

┌─────────────────────────────────────────────────────────────────────┐
│                      DEPENDENCY INVERSION                            │
│              (Depend on abstractions, not concretions)               │
└─────────────────────────────────────────────────────────────────────┘

                    ┌──────────────────────┐
                    │        Game          │
                    ├──────────────────────┤
                    │ - state: GameState   │ [SRP] Game state
                    │ - entities:          │ [DIP] ▼ Abstraction!
                    │   vector<unique_ptr  │
                    │   <GameEntity>>      │
                    │ - playerSnake: Snake*│ [DIP] For game logic
                    │ - gameFruit: Food*   │ [DIP] For game logic
                    ├──────────────────────┤
                    │ + initialize()       │ [SRP] Setup
                    │ + update()           │ [OCP] Polymorphic loop
                    │ + render()           │ [OCP] Polymorphic loop
                    │ + cleanup()          │ [SRP] Cleanup
                    │ + addEntity()        │ [OCP] Add any entity
                    └──────────────────────┘
                              │
                              │ uses
                              ▼
              ┌──────────────────────────────┐
              │   CollisionManager           │
              │   (Static service class)     │
              ├──────────────────────────────┤
              │ + checkCollision()           │ [SRP] Collision only
              │ + checkSnakeWithFood()       │ [SRP] Specific check
              └──────────────────────────────┘

              ┌──────────────────────────────┐
              │   Renderer                   │
              │   (Static service class)     │
              ├──────────────────────────────┤
              │ + drawGrid()                 │ [SRP] Render only
              │ + drawPauseScreen()          │ [ISP] Focused
              │ + drawGameOver()             │ [ISP] Focused
              │ + drawFreezeEffect()         │ [ISP] Focused
              └──────────────────────────────┘

              ┌──────────────────────────────┐
              │   Utils                      │
              │   (Static utility class)     │
              ├──────────────────────────────┤
              │ + getGridColumns()           │ [SRP] Grid calc
              │ + getGridRows()              │ [SRP] Grid calc
              │ + calculateGridOffset()      │ [SRP] Grid calc
              └──────────────────────────────┘

```

---

## 🔄 Sequence Diagram - Game Loop (Open/Closed Principle)

```
main                Game              GameEntity*         Snake/Food
 │                   │                    │                  │
 │ initialize()      │                    │                  │
 │──────────────────>│                    │                  │
 │                   │ new Snake()        │                  │
 │                   │───────────────────────────────────────>│
 │                   │                    │                  │
 │                   │ new Food()         │                  │
 │                   │───────────────────────────────────────>│
 │                   │                    │                  │
 │                   │ addEntity(snake)   │                  │
 │                   │──────────────────> │                  │
 │                   │                    │                  │
 │                   │ addEntity(food)    │                  │
 │                   │──────────────────> │                  │
 │                   │                    │                  │
 │                   │                    │                  │
 │ Game Loop:        │                    │                  │
 │ ┌──────────────┐  │                    │                  │
 │ │ update()     │  │                    │                  │
 │ └──────────────┘  │                    │                  │
 │ updateAndDraw()   │                    │                  │
 │──────────────────>│                    │                  │
 │                   │                    │                  │
 │                   │ [OCP] Polymorphic loop:               │
 │                   │ for each entity    │                  │
 │                   │──────────────────> │                  │
 │                   │                    │                  │
 │                   │ entity->update()   │                  │
 │                   │                    │ [LSP] virtual    │
 │                   │                    │  dispatch        │
 │                   │                    │─────────────────>│
 │                   │                    │                  │
 │                   │                    │  update()        │
 │                   │                    │<─────────────────│
 │                   │                    │                  │
 │                   │ [Next entity...]   │                  │
 │                   │                    │                  │
 │                   │ render()           │                  │
 │                   │──────────────────> │                  │
 │                   │                    │                  │
 │                   │ entity->render()   │                  │
 │                   │                    │─────────────────>│
 │                   │                    │                  │
 │                   │                    │  render()        │
 │                   │                    │<─────────────────│
 │                   │<─────────────────  │                  │
 │<──────────────────│                    │                  │
 │                   │                    │                  │

[OCP] Adding Obstacle:
  - Create Obstacle class (inherits GameEntity)
  - Call addEntity(obstacle)
  - NO modification to this loop!
```

---

## 🏗️ Component Diagram - Module Organization

```
┌────────────────────────────────────────────────────────────┐
│                     Main Application                        │
│                      (main.cpp)                            │
│  [DIP] Depends on Game abstraction only                   │
└────────────────────────────────────────────────────────────┘
                           │
                           │ depends on
                           ▼
┌────────────────────────────────────────────────────────────┐
│                     Game Controller                         │
│                      (Game.cpp)                            │
│  [SRP] Coordinates game loop                              │
│  [OCP] Uses polymorphic entity system                     │
│  [DIP] Depends on GameEntity*, not concrete types         │
└────────────────────────────────────────────────────────────┘
                           │
            ┌──────────────┼──────────────┐
            │              │              │
            ▼              ▼              ▼
┌──────────────┐  ┌──────────────┐  ┌──────────────┐
│    Snake     │  │     Food     │  │   Obstacle   │
│  (Snake.cpp) │  │  (Food.cpp)  │  │(Obstacle.cpp)│
│              │  │              │  │              │
│ [SRP] Snake  │  │ [SRP] Food   │  │ [SRP] Block  │
│  behavior    │  │  behavior    │  │  behavior    │
│              │  │              │  │              │
│ [LSP] Can    │  │ [LSP] Can    │  │ [LSP] Can    │
│  replace     │  │  replace     │  │  replace     │
│  GameEntity  │  │  GameEntity  │  │  GameEntity  │
└──────────────┘  └──────────────┘  └──────────────┘

┌────────────────────────────────────────────────────────────┐
│                     Service Layer                          │
│  [SRP] Each service has one responsibility                │
└────────────────────────────────────────────────────────────┘
            │              │              │
            ▼              ▼              ▼
┌──────────────┐  ┌──────────────┐  ┌──────────────┐
│  Collision   │  │   Renderer   │  │    Utils     │
│  Manager     │  │              │  │              │
│              │  │              │  │              │
│ [SRP] Only   │  │ [SRP] Only   │  │ [SRP] Only   │
│  collision   │  │  rendering   │  │  grid calc   │
└──────────────┘  └──────────────┘  └──────────────┘
```

---

## 🎨 Package Diagram - Layer Architecture

```
┌────────────────────────────────────────────────────────────┐
│                    Presentation Layer                       │
│                    (User Interface)                        │
├────────────────────────────────────────────────────────────┤
│  main.cpp                                                  │
│  - Window initialization                                   │
│  - Game loop                                               │
│  [DIP] Depends on Game abstraction                        │
└────────────────────────────────────────────────────────────┘
                           │
                           │ depends on
                           ▼
┌────────────────────────────────────────────────────────────┐
│                    Game Logic Layer                         │
│                  (Core Game Rules)                         │
├────────────────────────────────────────────────────────────┤
│  Game.cpp                                                  │
│  - Game state management                                   │
│  - Entity coordination                                     │
│  [OCP] Polymorphic entity loops                           │
│  [DIP] Depends on GameEntity* abstraction                 │
└────────────────────────────────────────────────────────────┘
                           │
                           │ depends on
                           ▼
┌────────────────────────────────────────────────────────────┐
│                     Entity Layer                            │
│                 (Game Objects)                             │
├────────────────────────────────────────────────────────────┤
│  GameEntity (abstract base)                                │
│  Snake.cpp      - Snake behavior                           │
│  Food.cpp       - Food behavior                            │
│  Obstacle.cpp   - Obstacle behavior (extensible)           │
│                                                            │
│  [SRP] Each entity manages its own state                  │
│  [LSP] All substitutable for GameEntity                   │
└────────────────────────────────────────────────────────────┘
                           │
                           │ depends on
                           ▼
┌────────────────────────────────────────────────────────────┐
│                    Service Layer                            │
│              (Shared Services)                             │
├────────────────────────────────────────────────────────────┤
│  CollisionManager.cpp - Collision detection                │
│  Renderer.cpp         - Drawing operations                 │
│  Utils.cpp            - Grid calculations                  │
│                                                            │
│  [SRP] Each service has one responsibility                │
│  [ISP] Focused, minimal interfaces                        │
└────────────────────────────────────────────────────────────┘
                           │
                           │ depends on
                           ▼
┌────────────────────────────────────────────────────────────┐
│                   Infrastructure Layer                      │
│                  (External Libraries)                      │
├────────────────────────────────────────────────────────────┤
│  Raylib - Graphics, Input, Audio                           │
│  STL    - Containers, Smart Pointers                       │
└────────────────────────────────────────────────────────────┘
```

---

## 🔀 State Diagram - Game States

```
                    ┌──────────────┐
                    │    START     │
                    └──────┬───────┘
                           │
                           ▼
                    ┌──────────────┐
              ┌────>│   PLAYING    │<────┐
              │     └──────┬───────┘     │
              │            │             │
              │            │ P pressed   │
              │            ▼             │
              │     ┌──────────────┐     │
              │     │    PAUSED    │     │
              │     └──────┬───────┘     │
              │            │             │
              │            │ P pressed   │
              │            └─────────────┘
              │            │
              │            │ Self-collision
              │            ▼
              │     ┌──────────────┐
              │     │   FREEZING   │
              │     └──────┬───────┘
              │            │
              │            │ Freeze timer ends
              │            ▼
              │     ┌──────────────┐
              └─────│  GAME OVER   │
        Enter       └──────────────┘
        pressed
```

---

## 🧩 Object Diagram - Runtime Entity System

```
Runtime State (During Gameplay):

┌────────────────────────────────────────────────────────┐
│                     game: Game                          │
├────────────────────────────────────────────────────────┤
│ state.framesCounter = 450                              │
│ state.playerScore = 5                                  │
│ state.isGameOver = false                               │
│                                                        │
│ entities: vector<unique_ptr<GameEntity>>               │
│ ┌────────────────────────────────────────┐             │
│ │ [0] ──> Snake object (length=6)        │             │
│ │         - segments[6]                  │             │
│ │         - position: (100, 150)         │             │
│ │         - speed: (31, 0)               │             │
│ │                                        │             │
│ │ [1] ──> Food object                    │             │
│ │         - position: (250, 200)         │             │
│ │         - active: true                 │             │
│ │                                        │             │
│ │ [2] ──> Obstacle object (if added)     │             │
│ │         - position: (300, 100)         │             │
│ │         - size: (31, 31)               │             │
│ │                                        │             │
│ │ [OCP] Can add more entities here!      │             │
│ │       No modification to Game needed!  │             │
│ └────────────────────────────────────────┘             │
│                                                        │
│ playerSnake ──> points to entities[0].data            │
│ gameFruit   ──> points to entities[1].data            │
└────────────────────────────────────────────────────────┘

Polymorphic Update Loop:
for (auto& entity : entities) {
    entity->update(framesCounter);  // ← Virtual dispatch
}                                   // Calls correct method!
```

---

## 📋 Deployment Diagram - Build Structure

```
┌──────────────────────────────────────────────────────────┐
│                    Development Machine                    │
│                                                          │
│  ┌────────────────────────────────────────────┐          │
│  │           Source Files (.cpp, .hpp)        │          │
│  │  - SnakeGame.hpp                           │          │
│  │  - main.cpp, Game.cpp, Snake.cpp, etc.     │          │
│  └────────────────┬───────────────────────────┘          │
│                   │ compile                              │
│                   ▼                                      │
│  ┌────────────────────────────────────────────┐          │
│  │           Object Files (.o)                │          │
│  │  - main.o, Game.o, Snake.o, Food.o, etc.   │          │
│  └────────────────┬───────────────────────────┘          │
│                   │ link                                 │
│                   ▼                                      │
│  ┌────────────────────────────────────────────┐          │
│  │        Executable (snake_game)             │          │
│  │  + Raylib library (dynamically linked)     │          │
│  │  + STL (statically linked)                 │          │
│  └────────────────┬───────────────────────────┘          │
│                   │ run                                  │
│                   ▼                                      │
│  ┌────────────────────────────────────────────┐          │
│  │          Running Game Process              │          │
│  │  - Loads Raylib.dll/.so                    │          │
│  │  - Creates window                          │          │
│  │  - Runs game loop                          │          │
│  └────────────────────────────────────────────┘          │
└──────────────────────────────────────────────────────────┘
```

---

## 🎯 SOLID Principles Mapping

```
┌────────────────────────────────────────────────────────────┐
│  SOLID Principle         │  Implementation in UML          │
├──────────────────────────┼─────────────────────────────────┤
│ [S] Single               │ Each class has one box          │
│     Responsibility       │ with focused responsibilities   │
│                          │ Snake = snake only              │
│                          │ Food = food only                │
├──────────────────────────┼─────────────────────────────────┤
│ [O] Open/Closed          │ GameEntity = abstract base      │
│                          │ Snake/Food = inherit            │
│                          │ Add Obstacle = inherit          │
│                          │ Game doesn't change!            │
├──────────────────────────┼─────────────────────────────────┤
│ [L] Liskov               │ Snake, Food both shown as       │
│     Substitution         │ GameEntity* in entity array     │
│                          │ All interchangeable             │
├──────────────────────────┼─────────────────────────────────┤
│ [I] Interface            │ IUpdatable, IRenderable,        │
│     Segregation          │ ICollidable shown separately    │
│                          │ Focused interfaces              │
├──────────────────────────┼─────────────────────────────────┤
│ [D] Dependency           │ Game depends on GameEntity*     │
│     Inversion            │ NOT on Snake/Food directly      │
│                          │ Abstraction dependency shown    │
└──────────────────────────┴─────────────────────────────────┘
```

---

## 📝 Notes on UML Notation

- **Solid arrows (──>)**: Dependency/Association
- **Hollow triangle (▲)**: Inheritance
- **Dashed arrows (- ->)**: Implementation (interface)
- **+ symbol**: Public member
- **- symbol**: Private member
- **= 0**: Pure virtual function (abstract)
- **<<interface>>**: Interface class
- **<<abstract>>**: Abstract base class

---

This UML documentation shows the complete architecture and how SOLID principles are applied throughout the design! 🎓

# 🐍 Snake Game - SOLID Principles Implementation

## 📋 Project Overview

This project demonstrates the application of **SOLID principles** in both **C** and **C++** for a classic Snake game. The project was refactored from a monolithic design to a modular, maintainable architecture following professional software engineering best practices.

**Course:** Advanced Programming Lab  
**Date:** February 2026  
**Language:** C and C++  
**Graphics Library:** Raylib

---

## 🎯 Project Goals

1. **Apply SOLID Principles** in real-world game development
2. **Demonstrate polymorphism** in both C (function pointers) and C++ (virtual functions)
3. **Achieve Open/Closed Principle** - add new features without modifying existing code
4. **Practice professional code organization** and documentation
5. **Compare C vs C++** approaches to object-oriented design

---

## 🏗️ Architecture Overview

### **Before Refactoring**
```
❌ Monolithic Design (330+ lines in one file)
- 11 global variables
- Cryptic names (scr, snkLen, x, y)
- Mixed responsibilities
- Hard to extend
- Violates Open/Closed Principle
```

### **After Refactoring**
```
✅ Modular SOLID Design
- Entity system with polymorphism
- Clear separation of concerns
- Easy to extend (add obstacles/powerups)
- Follows all 5 SOLID principles
- Professional code structure
```

---

## 📚 SOLID Principles Applied

### **[S] Single Responsibility Principle**
Each module/class has ONE reason to change:
- `Snake` - Manages snake behavior only
- `Food` - Manages food behavior only
- `Game` - Coordinates game loop only
- `Renderer` - Handles rendering only
- `CollisionManager` - Detects collisions only

### **[O] Open/Closed Principle** ⭐ **Key Achievement**
System is **OPEN for extension**, **CLOSED for modification**:
- Add obstacles: Create new entity, NO modification to game loop
- Add powerups: Create new entity, NO modification to existing code
- Achieved through polymorphism (function pointers in C, virtual functions in C++)

### **[L] Liskov Substitution Principle**
Any derived class can replace its base class:
- `Snake` and `Food` both work as `GameEntity`
- Polymorphic container holds any entity type
- No breaking of interface contract

### **[I] Interface Segregation Principle**
Interfaces are minimal and focused:
- `IUpdatable` - Only update behavior
- `IRenderable` - Only render behavior
- `ICollidable` - Only collision data
- Classes implement only needed interfaces

### **[D] Dependency Inversion Principle**
High-level modules depend on abstractions:
- Game depends on `GameEntity*`, not concrete `Snake`/`Food`
- Main depends on `Game`, not individual entities
- Follows: "Depend on abstractions, not concretions"

---

## 📁 Project Structure

### **C++ Version** (OOP with Virtual Functions)
```
snake-game-cpp/
├── SnakeGame.hpp          # All class declarations, interfaces
├── main.cpp               # Entry point
├── Game.cpp               # Game class implementation
├── Snake.cpp              # Snake class (inherits GameEntity)
├── Food.cpp               # Food class (inherits GameEntity)
├── CollisionManager.cpp   # Collision service
├── Renderer.cpp           # Renderer service
├── Utils.cpp              # Utility functions
├── Makefile               # C++ build configuration
└── CPP_SOLID_GUIDE.md     # Complete C++ SOLID tutorial
```

### **C Version** (Function Pointers for Polymorphism)
```
snake-game-c/
├── snake_game.h           # All declarations, GameEntity interface
├── main.c                 # Entry point
├── game.c                 # Game loop with entity system
├── snake.c                # Snake implementation + factory
├── food.c                 # Food implementation + factory
├── collision.c            # Collision detection
├── renderer.c             # Rendering functions
├── utils.c                # Utility functions
├── Makefile               # C build configuration
└── C_SOLID_GUIDE.md       # Complete C SOLID tutorial
```

---

## 🔑 Key Features

### **Polymorphism Implementation**

**C++ Version:**
```cpp
class GameEntity {
public:
    virtual void update(int frames) = 0;  // Pure virtual
    virtual void render() const = 0;
};

class Snake : public GameEntity {
    void update(int frames) override { /* ... */ }
    void render() const override { /* ... */ }
};

// Polymorphic container
std::vector<std::unique_ptr<GameEntity>> entities;
for (auto& entity : entities) {
    entity->update(frames);  // Virtual function call
}
```

**C Version:**
```c
typedef struct GameEntity {
    void (*update)(GameEntity* entity, int frames);
    void (*render)(const GameEntity* entity);
    void* data;  // Opaque pointer to actual entity
} GameEntity;

// Polymorphic array
GameEntity* entities[10];
for (int i = 0; i < count; i++) {
    entities[i]->update(entities[i], frames);  // Function pointer call
}
```

### **Entity Factory Pattern**

**C++ Version:**
```cpp
auto snake = std::make_unique<Snake>(gridOffset);
auto food = std::make_unique<Food>();
entities.push_back(std::move(snake));
entities.push_back(std::move(food));
```

**C Version:**
```c
GameEntity* snake = Entity_CreateSnake(gridOffset);
GameEntity* food = Entity_CreateFood();
Game_AddEntity(snake);
Game_AddEntity(food);
```

---

## 🚀 How to Build and Run

### **C++ Version**

**Prerequisites:**
- C++ compiler (g++, clang++)
- Raylib library
- Make

**Build:**
```bash
cd snake-game-cpp
make
./snake_game
```

**Controls:**
- Arrow Keys: Move snake
- P: Pause/Resume
- Enter: Restart (after game over)

### **C Version**

**Prerequisites:**
- C compiler (gcc, clang)
- Raylib library
- Make

**Build:**
```bash
cd snake-game-c
make
./snake_game
```

---

## 📊 Code Metrics Comparison

| Metric | Before | After (C) | After (C++) |
|--------|--------|-----------|-------------|
| **Files** | 1 file | 8 files | 8 files |
| **Lines (main)** | 330+ | 60 | 50 |
| **Global Vars** | 11 | 0 | 0 |
| **Modularity** | ❌ None | ✅ High | ✅ High |
| **Extensibility** | ❌ Hard | ✅ Easy | ✅ Easy |
| **OCP Compliance** | ❌ No | ✅ Yes | ✅ Yes |
| **Polymorphism** | ❌ No | ✅ Function Ptrs | ✅ Virtual Funcs |

---

## 🎓 Educational Value

### **What You'll Learn**

1. **SOLID Principles in Practice**
   - Real-world application of all 5 principles
   - Not just theory, but actual implementation
   - Professional code organization

2. **Polymorphism Techniques**
   - C: Function pointers for polymorphism
   - C++: Virtual functions and inheritance
   - When to use each approach

3. **Design Patterns**
   - Factory Pattern (entity creation)
   - Entity-Component System
   - Dependency Injection

4. **Code Organization**
   - Module separation
   - Header file design
   - Interface vs Implementation

5. **C vs C++ Comparison**
   - OOP without classes (C)
   - OOP with classes (C++)
   - Trade-offs and use cases

---

## 📖 Documentation

### **Comprehensive Guides Included**

1. **CPP_SOLID_GUIDE.md**
   - Complete C++ SOLID tutorial
   - All 5 principles with examples
   - How to extend (add obstacles)
   - Before/after comparisons

2. **C_SOLID_GUIDE.md**
   - Complete C SOLID tutorial
   - Function pointer polymorphism
   - Entity system implementation
   - Memory management

3. **SOLID_EXPLANATION.md**
   - Before/after comparison
   - What changed and why
   - Key achievements
   - Benefits analysis

4. **Source Code Comments**
   - Every file heavily commented
   - SOLID principle annotations
   - Explains WHY, not just WHAT

---

## 🔧 How to Extend

### **Adding Obstacles (Example)**

**Step 1: Create Obstacle Class/Module**

C++:
```cpp
class Obstacle : public GameEntity {
private:
    Vector2 position;
public:
    void update(int frames) override { /* stationary */ }
    void render() const override { /* draw gray block */ }
};
```

C:
```c
GameEntity* Entity_CreateObstacle(Vector2 pos) {
    GameEntity* entity = malloc(sizeof(GameEntity));
    // Set up function pointers...
    return entity;
}
```

**Step 2: Add to Game**
```cpp
// C++
entities.push_back(std::make_unique<Obstacle>(pos));

// C
Game_AddEntity(Entity_CreateObstacle(pos));
```

**✅ That's it! No modification to:**
- Game update loop
- Game render loop
- Main function
- Existing entity classes

**This demonstrates Open/Closed Principle perfectly!**

---

## 🎯 Design Decisions

### **Why Entity System?**

**Problem with Original Approach:**
```c
// Adding obstacles requires modifying game.c
static Snake playerSnake;
static Food gameFruit;
static Obstacle gameObstacle;  // ← New code!

void Game_Update() {
    Snake_Update(&playerSnake);
    Food_Update(&gameFruit);
    Obstacle_Update(&gameObstacle);  // ← New code!
}
```

**Solution with Entity System:**
```c
// Adding obstacles requires NO modification to game.c
GameEntity* entities[10];

void Game_Update() {
    for (int i = 0; i < entityCount; i++) {
        entities[i]->update(entities[i], frames);  // ✅ Works for any entity!
    }
}
```

### **Why Function Pointers in C?**

C doesn't have built-in polymorphism, but we can achieve it through:
1. **Function pointers** in structs (manual vtable)
2. **Opaque pointers** for encapsulation (`void* data`)
3. **Factory functions** for object creation

This gives us OOP benefits in pure C!

### **Why Smart Pointers in C++?**

C++ version uses `std::unique_ptr` for:
- **Automatic memory management** (no leaks)
- **Clear ownership semantics** (who owns what)
- **Exception safety** (RAII principle)
- **Modern C++ best practices**

---

## 📈 Performance Considerations

### **Function Pointer Overhead**

**C Version:**
- Small overhead from indirect function calls
- Negligible for game loop (30 FPS)
- Benefits outweigh costs

**C++ Version:**
- Virtual function calls have similar overhead
- Compiler optimizations help
- Cache-friendly with proper design

### **Memory Layout**

Both versions use:
- Contiguous entity arrays (cache-friendly)
- Minimal allocations per frame
- Static game state where possible

---

## 🧪 Testing Strategy

### **Unit Testing**

Each module can be tested independently:

```c
// Test snake movement
void test_snake_movement() {
    Snake snake;
    Snake_Initialize(&snake, gridOffset);
    Snake_Update(&snake, 0);
    assert(snake.segments[0].position.x == expected);
}

// Test collision detection
void test_collision() {
    assert(Collision_CheckSnakeWithFood(&snake, &food) == true);
}
```

### **Integration Testing**

Test entity system:
```c
void test_entity_system() {
    GameEntity* snake = Entity_CreateSnake(offset);
    assert(snake->update != NULL);
    snake->update(snake, 0);  // Should not crash
}
```

---

## 🐛 Known Issues / Future Improvements

### **Current Limitations**
- Fixed entity array size (10 entities max)
- No entity removal during gameplay
- Single food item only

### **Potential Enhancements**
- Dynamic entity allocation (vector in C++, dynamic array in C)
- Multiple food items with different values
- Obstacle entities
- Powerup system (speed boost, invincibility)
- Particle effects
- Sound system
- High score persistence

**All can be added without modifying core game loop! (Open/Closed Principle)**

---

## 📜 License

This is an educational project for Advanced Programming Lab course.  
Free to use for learning purposes.

---

## 👥 Contributors

**Student:** [Your Name]  
**Course:** Advanced Programming Lab  
**Institution:** [Your Institution]  
**Date:** February 2026

---

## 🙏 Acknowledgments

- **Raylib** - Simple and easy-to-use graphics library
- **SOLID Principles** - Robert C. Martin (Uncle Bob)
- **Design Patterns** - Gang of Four
- **Clean Code** - Robert C. Martin

---

## 📚 References

1. **SOLID Principles**
   - Martin, R.C. (2000). Design Principles and Design Patterns
   - https://en.wikipedia.org/wiki/SOLID

2. **Design Patterns**
   - Gamma, E., et al. (1994). Design Patterns: Elements of Reusable Object-Oriented Software

3. **Raylib Documentation**
   - https://www.raylib.com/

4. **Modern C++ Guidelines**
   - https://isocpp.github.io/CppCoreGuidelines/

---

## 🎯 Summary

This project successfully demonstrates:

✅ **All 5 SOLID principles** in real code  
✅ **Polymorphism in C and C++** (different approaches)  
✅ **Open/Closed Principle** (extend without modifying)  
✅ **Professional code organization** (modular, documented)  
✅ **Practical software engineering** (not just theory)  

**This is how professional game development is done!** 🚀

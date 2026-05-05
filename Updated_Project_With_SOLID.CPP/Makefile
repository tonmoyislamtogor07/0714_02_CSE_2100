# Makefile for Snake Game (Windows MinGW + raylib)

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -IC:/raylib/raylib/src

# Linker flags
LDFLAGS = -LC:/raylib/raylib/src -lraylib -lopengl32 -lgdi32 -lwinmm

# Target executable
TARGET = snake_game.exe

# Source files
SOURCES = main.cpp game.cpp snake.cpp food.cpp collision.cpp renderer.cpp utils.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADER = snake_game.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
%.o: %.cpp $(HEADER)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files (Windows)
clean:
	@if exist *.o del /F /Q *.o
	@if exist $(TARGET) del /F /Q $(TARGET)
	@echo "Clean complete"

# Rebuild from scratch
rebuild: clean all

# Run the game
run: $(TARGET)
	.\$(TARGET)

.PHONY: all clean rebuild run
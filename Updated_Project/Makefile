# Makefile for Snake Game (Refactored)
# Course: Advanced Programming Lab
# Date: February 2026

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lraylib -lm -lpthread -ldl

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    LDFLAGS += -lopengl32 -lgdi32 -lwinmm
    TARGET = snake_game.exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS += -lGL -lrt -lX11
    endif
    ifeq ($(UNAME_S),Darwin)
        LDFLAGS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
    endif
    TARGET = snake_game
endif

# Source files
SOURCES = main.c game.c snake.c food.c collision.c renderer.c utils.c
OBJECTS = $(SOURCES:.c=.o)
HEADER = snake_game.h

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete"

# Rebuild from scratch
rebuild: clean all

# Run the game
run: $(TARGET)
	./$(TARGET)

# Help target
help:
	@echo "Snake Game Makefile"
	@echo "-------------------"
	@echo "Targets:"
	@echo "  all      - Build the game (default)"
	@echo "  clean    - Remove build files"
	@echo "  rebuild  - Clean and build"
	@echo "  run      - Build and run the game"
	@echo "  help     - Show this help message"

.PHONY: all clean rebuild run help

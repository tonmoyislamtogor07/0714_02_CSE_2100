# Makefile — Snake Game MVC Edition

CC      = g++
CFLAGS  = -Wall -Wextra -std=c++17 -O2
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET  = snake_mvc
SRCS    = main.cpp model.cpp view.cpp controller.cpp utils.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

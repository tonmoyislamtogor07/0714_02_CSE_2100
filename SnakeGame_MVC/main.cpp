/*
 * main.cpp
 *
 * Entry point for Snake Game - MVC Edition
 *
 * ============================================================================
 * MVC NOTE
 * ============================================================================
 *
 * main() has a single job: hand control to the Controller.
 * In MVC the Controller owns the application lifecycle, so main() is just a
 * thin bootstrap that calls Controller_Run() and nothing else.
 *
 * There is NO game logic, NO rendering, and NO input handling here.
 *
 * ============================================================================
 */

#include "snake_game.h"

int main(void)
{
    // The Controller owns the window, the game loop, and the shutdown sequence.
    // main() simply delegates everything to it.
    Controller_Run();
    return 0;
}
